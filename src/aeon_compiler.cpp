#include "aeon_compiler.h"
#include "aeon_bytecode.h"
#include "aeon_vm.h"
#include "aeon_context.h"
#include "aeon_expression.h"

#include <cassert>

aeon_compiler::aeon_compiler()
{
	m_OffsetFromBasePtr = 0;
}

int32_t aeon_compiler::cursor()
{
	return m_cursor;
}

void aeon_compiler::emitInstruction(aeon_instruction instr)
{
	m_module->instructions.push_back(instr);
	m_cursor++;
}

uint32_t aeon_compiler::emitInstruction(uint8_t opcode, int8_t arg0, int8_t arg1, int8_t arg2)
{
	aeon_instruction instr;
	instr.opcode = opcode;
	instr.arg0 = arg0;
	instr.arg1 = arg1;
	instr.arg2 = arg2;
	m_module->instructions.push_back(instr);
	return m_cursor++;
}

void aeon_compiler::push_scope()
{
	ScopeLocalData newScope;
	m_scopes.push_back(newScope);
}

void aeon_compiler::pop_scope()
{
	//assert(m_scopes.size() == 0);

	ScopeLocalData& top = m_scopes.back();
	for (auto& it = top.locals.rbegin(); it != top.locals.rend(); ++it)
	{
		emit_local_destruct(*it);
	}
	m_scopes.pop_back();
}

VariableStorageInfo aeon_compiler::getVariable(std::string name)
{
	// Let's see if this is a local variable
	for (auto it = m_scopes.rbegin(); it != m_scopes.rend(); ++it)
	{
		// Is the variable we reference here?
		for (auto var_it = (*it).locals.rbegin(); var_it != (*it).locals.rend(); ++var_it)
		{
			if ((*var_it).name == name)
			{
				return (*var_it);
			}
		}
	}

	// Let's see if this is a local variable
	if (m_classes.size() > 0)
	{
		ast_class* classNode = m_classes[m_classes.size() - 1];
		aeon_type* type = evaluateType(classNode);

		if (type->getField(name))
		{
			// The variable we are referencing is a member of 'this'
			VariableStorageInfo varStorage;
			varStorage.type = type;
			varStorage.mode = AE_VAR_FIELD;
			varStorage.offset = type->getField(name)->offset;
			varStorage.name = name;
			return varStorage;
		}
	}
	
	return VariableStorageInfo();
}

void aeon_compiler::emit_local_destruct(VariableStorageInfo& var)
{
	emitInstruction(OP_MOV, AEK_ESP, var.type->getSize());
}

void aeon_compiler::emit_local_construct_pod(int32_t size)
{

}

void aeon_compiler::emit_local_destruct_pod(int32_t size)
{

}

void aeon_compiler::emit_local_construct_object(int32_t size)
{

}

void aeon_compiler::emit_local_destruct_object(int32_t size)
{

}

void aeon_compiler::generate(aeon_ast_node* root)
{
	for (std::size_t i = 0; i < root->items.size(); ++i)
	{
		if (root->items[i]->type == aeon_ast_node::FuncDecl)
		{
			emitFunctionCode(static_cast<aeon_ast_function*>(root->items[i]));
		}
		else if (root->items[i]->type == aeon_ast_node::ClassDecl)
		{
			emitClassCode(static_cast<ast_class*>(root->items[i]));
		}
		else if (root->items[i]->type == aeon_ast_node::Namespace)
		{
			emitNamespaceCode(static_cast<ast_namespace*>(root->items[i]));
		}
		else if (root->items[i]->type == aeon_ast_node::VarExpr)
		{
			emitGlobalVarCode(static_cast<ast_varexpr*>(root->items[i]));
		}
	}
}

void aeon_compiler::emitNamespaceCode(ast_namespace* namespace_node)
{
	for (std::size_t i = 0; i < namespace_node->items.size(); ++i)
	{
			if (namespace_node->items[i]->type == aeon_ast_node::FuncDecl)
			{
				emitFunctionCode(static_cast<aeon_ast_function*>(namespace_node->items[i]));
			}
			else if (namespace_node->items[i]->type == aeon_ast_node::ClassDecl)
			{
				emitClassCode(static_cast<ast_class*>(namespace_node->items[i]));
			}
			else if (namespace_node->items[i]->type == aeon_ast_node::Namespace)
			{
				emitNamespaceCode(static_cast<ast_namespace*>(namespace_node->items[i]));
			}
			else if (namespace_node->items[i]->type == aeon_ast_node::VarExpr)
			{
				emitGlobalVarCode(static_cast<ast_varexpr*>(namespace_node->items[i]));
			}
	 }
}

void aeon_compiler::emitGlobalVarCode(ast_varexpr* global_var)
{
	
}

void aeon_compiler::emitClassCode(ast_class* clss)
{
	m_classes.push_back(clss);

	if (clss->parents.size() > 1 && !m_env->m_config.allowMultipleInheritance)
	{
		throwError("The class '" + clss->Name + "' can't inherit from multiple classes.");
		return;
	}

	aeon_type* typeInfo = new aeon_type();
	typeInfo->m_name = clss->Name;
	typeInfo->m_module = m_module;
	m_env->typedb.push_back(typeInfo);
	m_module->types.push_back(*typeInfo);
	aeon_context::object_heap objectHeap;
	objectHeap.type = typeInfo;
	m_env->object_heaps.push_back(objectHeap);

	clss->typeInfo = typeInfo;

	printf("Compiled type %s\n", typeInfo->m_name.c_str());

	for (auto pnt : clss->parents)
	{
		aeon_type* info = m_env->getTypeInfo(pnt.parentClass);
		if (!info)
		{
			printf("Compiler Error C0001: Can't inherit from '%s'. No such type.\n", pnt.parentClass.c_str());
		}
	}

	for (std::size_t i = 0; i < clss->items.size(); ++i)
	{
		if (clss->items[i]->type == aeon_ast_node::FuncDecl)
		{
			emitFunctionCode(static_cast<aeon_ast_function*>(clss->items[i]));
		}
		else if (clss->items[i]->type == aeon_ast_node::ClassDecl)
		{
			emitClassCode(static_cast<ast_class*>(clss->items[i]));
		}
		else if (clss->items[i]->type == aeon_ast_node::VarExpr)
		{
			ast_varexpr* varDecl = (ast_varexpr*)clss->items[i];

			aeon_type* fieldType = m_env->getTypeInfo(varDecl->VarType->name);
			if (!fieldType)
			{
				throwError("Class has variables with unknown type '" + varDecl->VarType->name + "'");			
			}
			else
			{
				aeon_type::FieldInfo fieldInfo;
				fieldInfo.name = varDecl->Name;
				fieldInfo.size = 4;
				typeInfo->createField(fieldInfo);
			}
		}
	}

	m_classes.pop_back();
}

void aeon_compiler::throwError(const std::string& message)
{
	printf("CXXX: %s\n", message.c_str());
}

void aeon_compiler::emitBranchCode(ast_ifbranch* cond)
{
	aeon_expression* test_expr = cond->expr;
	ast_codeblock* nested_code = cond->block;

	// The expression must evaluate first
	emitExpressionEval(test_expr, ExpressionEvalContext());

	// Now that the expression is evaluated and stored in a register, let's set the jmp
	aeon_instruction jmptest;
	setopcode(jmptest, EOpCodes::OP_JZ);
	setinst_a(jmptest, 0);
	emitInstruction(jmptest);
	int jmptestpc = m_cursor - 1;

	// Now let's define the actual block
	emitScopeCode(nested_code);

	// make sure the if jmp goes to the end of the block if it fails
	setinst_a(m_module->instructions[jmptestpc], (m_cursor - 1) - jmptestpc);
}

int aeon_compiler::findLocalObject(const std::string& refname)
{
	/*for (std::size_t i = 0; i < mFunctionLocals.size(); ++i)
	{
		if (mFunctionLocals[i].name == refname)
			return i;
	}*/
	return -1;
}

void aeon_compiler::emitPrefixIncrOp(ast_unaryop* expr)
{

}

void aeon_compiler::emitForLoop(ast_for* forloop)
{
	push_scope();

	// Emit the code that initializes the control vars
	// They are automatically scoped and are destructed in the end of the loop
	for (auto& initExpr : forloop->initExpressions)
	{
		ExpressionEvalContext ectx;
		emitExpressionEval(initExpr, ectx);
		emitInstruction(OP_DTEST, 4);
	}

	int pc_expreval = cursor(); // track the first instruction of this while, to jump back to evaluation each iteration

	ExpressionEvalContext ectx;
	emitExpressionEval(forloop->expr, ectx);

	// Evaluate the expression
	int jmptestpc = emitInstruction(OP_JZ, 0, 0, 0);

	emitScopeCode(forloop->block);

	// After the code executes, apply the update expression
	emitExpressionEval(forloop->incrExpr, ectx);

	// After the block terminates normally, we always go back up to expression evaluation
	// when the expression fails, it will jump right away to after this
	emitInstruction(OP_JMP, pc_expreval);

	// if the expression evaluates false, jump to after the for
	setinst_a(m_module->instructions[jmptestpc], (cursor() - 1) - jmptestpc);

	pop_scope();
}


void aeon_compiler::emitWhileLoop(ast_while* whileloop)
{
	int pc_expreval = cursor();

	ExpressionEvalContext ectx;
	emitExpressionEval(whileloop->expr, ectx);

	int jmptestpc = emitInstruction(OP_JZ, 0, 0, 0);

	emitScopeCode(whileloop->block);

	// After the block terminates normally, we always go back up to expression evaluation
	// when the expression fails, it will jump right away to after this
	emitInstruction(OP_JMP, pc_expreval - 1);

	// if the expression evaluates false, jump to after the while
	setinst_a(m_module->instructions[jmptestpc], (cursor() - 1) - jmptestpc);
}

void aeon_compiler::emitFunctionCode(aeon_ast_function* func)
{
	m_OffsetFromBasePtr = 0;

	// If we are a method, we reserve space for the THIS ptr
	if (m_classes.size() > 0)
	{
		m_OffsetFromBasePtr += sizeof(void*);
	}

	aeon_function functiondata;
	functiondata.offset = m_cursor;
	functiondata.prototype = func->m_name;
	m_module->functions.push_back(functiondata);

	// let's just generate code for the executable block
	emitScopeCode(func->m_block);

	emitReturnCode(nullptr);
}

void aeon_compiler::emitFunctionCall(ast_funccall* funccall, ExpressionEvalContext exprCtx)
{
		// Each child of the funccall is one argument to generate code for
		int j = funccall->args.size() - 1;
		while (j >= 0)
		{
			aeon_expression* expr = static_cast<aeon_expression*>(funccall->args[j]);
			ExpressionEvalContext ectx;
			emitExpressionEval(expr, ectx);

			--j;
		}

		int funcId = m_env->getNativeFunctionIndex(funccall->funcName);
		if (funcId >= 0)
		{
			// Now the actual call
			aeon_instruction fcallinst;
			setopcode(fcallinst, EOpCodes::OP_CALLNATIVE);
			setinst_a(fcallinst, funcId);
			setinst_b(fcallinst, funccall->args.size());
			emitInstruction(fcallinst);
		}
		else
		{
			if (m_module->getFunctionIndexByName(funccall->funcName) < 0)
			{
				throwError("Function '" + funccall->funcName + "' not found!");
			}
			else
			{
				// we'll need to call a nested func
				aeon_instruction fcallinst;
				setopcode(fcallinst, EOpCodes::OP_CALL);
				setinst_a(fcallinst, m_module->getFunctionIndexByName(funccall->funcName));
				setinst_b(fcallinst, funccall->items.size());
				emitInstruction(fcallinst);
			}
		}
}

void aeon_compiler::emitScopeCode(ast_codeblock* codeblock)
{
	push_scope();

	for (std::size_t i = 0; i < codeblock->items.size(); ++i)
	{
		if (codeblock->items[i]->type == aeon_ast_node::FuncCall)
		{
			emitFunctionCall(static_cast<ast_funccall*>(codeblock->items[i]), ExpressionEvalContext());
		}
		else if (codeblock->items[i]->type == aeon_ast_node::IfBranch)
		{
			emitBranchCode(static_cast<ast_ifbranch*>(codeblock->items[i]));
		}
		else if (codeblock->items[i]->type == aeon_ast_node::WhileLoop)
		{
			emitWhileLoop(static_cast<ast_while*>(codeblock->items[i]));
		}
		else if (codeblock->items[i]->type == aeon_ast_node::ForLoop)
		{
			emitForLoop(static_cast<ast_for*>(codeblock->items[i]));
		}
		else if (codeblock->items[i]->type == aeon_ast_node::VarDecl)
		{
			emitVarDecl(static_cast<aeon_stmt_vardecl*>(codeblock->items[i]));
		}
		else
		{
			// might be buggy to assume this
			emitExpressionEval(static_cast<aeon_expression*>(codeblock->items[i]), ExpressionEvalContext());
		}
	}

	pop_scope();
}

void aeon_compiler::emitVarDecl(aeon_stmt_vardecl* varDecl)
{
	aeon_type* varType = evaluateType(varDecl->type_name);
	if (!varType)
	{
		throwError("No such type '" + varDecl->type_name+"'");
		return;
	}

	auto& scope = m_scopes[m_scopes.size() - 1]; 

	VariableStorageInfo localObject;
	localObject.type = varType;
	localObject.name = varDecl->name;
	localObject.offset = scope.offset;
	localObject.offset_bp = m_OffsetFromBasePtr;
	localObject.mode = AE_VAR_LOCAL;
	scope.locals.push_back(localObject);

	scope.offset += localObject.type->getSize();
	m_OffsetFromBasePtr += localObject.type->getSize();

	// Emit the stack allocation code
	emitInstruction(OP_MOV, AEK_ESP, -varType->getSize());

	// Now the variable is initialized and part of its scope, initialize it
	if (varDecl->init_expr)
	{
		emitExpressionEval(varDecl->init_expr, ExpressionEvalContext());
	}
} 

void aeon_compiler::emitReturnCode(ast_return* ret)
{
	emitInstruction(OP_RETURN);
}

aeon_type* aeon_compiler::evaluateType(aeon_expression* expr)
{
	ast_varexpr* var = (ast_varexpr*)expr;
	return evaluateType(var->Name);
}

aeon_type* aeon_compiler::evaluateType(const std::string& type_name)
{
	// Walk the scope stack from current to top level to search the decl
	for (auto it = m_scopes.rbegin(); it != m_scopes.rend(); ++it)
	{
		// Is the variable we reference here?
		for (auto var_it = (*it).locals.rbegin(); var_it != (*it).locals.rend(); ++var_it)
		{
			if ((*var_it).name == type_name)
			{
				return (*var_it).type;
			}
		}
	}

	// Are we compiling a method currently?
	// In that case we probably want a member variable
	if (!m_classes.empty())
	{
		ast_class* container_class = m_classes[m_classes.size() - 1];
		aeon_type* class_type = evaluateType(container_class);

		// todo
	}

	/// Can still be a primitive
	aeon_type* type = m_env->getTypeInfo(type_name);

	/// Cannot evaluate.
	return type;
}

aeon_type* aeon_compiler::evaluateType(ast_class* class_node)
{
	return class_node->typeInfo;
}