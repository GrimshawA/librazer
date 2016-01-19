#include "aeon_compiler.h"
#include "aeon_bytecode.h"
#include "aeon_vm.h"
#include "aeon_context.h"
#include "aeon_expression.h"
#include "nodes/aeNodeClass.h"
#include "nodes/aeNodeNamespace.h"

#include <cassert>

aeon_compiler::aeon_compiler()
{
	m_OffsetFromBasePtr = 0;
	m_caller = nullptr;
}

int32_t aeon_compiler::cursor()
{
	return m_cursor;
}

void aeon_compiler::emitDebugPrint(const std::string& message)
{
	emitInstruction(OP_DEBUG, 0, m_env->getStringLiteral(message));
}

uint32_t aeon_compiler::emitInstruction(aeon_instruction instr)
{
	m_module->instructions.push_back(instr);
	return m_cursor++;
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
		destructLocalVar(*it);
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
		aeNodeClass* classNode = m_classes[m_classes.size() - 1];
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

void aeon_compiler::destructLocalVar(VariableStorageInfo& var)
{
	if (var.type->is_native)
	{

	}
	emitDebugPrint("Destroying a " + var.name);
	emitInstruction(OP_MOV, AEK_ESP, sizeof(vm_value));
}

void aeon_compiler::emit_local_construct_pod(int32_t size)
{

}

void aeon_compiler::emit_local_construct_object(int32_t size)
{

}

void aeon_compiler::generate(aeNodeBase* root)
{
	for (std::size_t i = 0; i < root->m_items.size(); ++i)
	{
		if (root->m_items[i]->m_type == AEN_FUNCTION)
		{
			emitFunctionCode(static_cast<aeNodeFunction*>(root->m_items[i]));
		}
		else if (root->m_items[i]->m_type == AEN_CLASS)
		{
			emitClassCode(static_cast<aeNodeClass*>(root->m_items[i]));
		}
		else if (root->m_items[i]->m_type == AEN_NAMESPACE)
		{
			emitNamespaceCode(static_cast<aeNodeNamespace*>(root->m_items[i]));
		}
		else if (root->m_items[i]->m_type == VarExpr)
		{
			emitGlobalVarCode(static_cast<aeNodeVarRef*>(root->m_items[i]));
		}
	}
}

void aeon_compiler::emitNamespaceCode(aeNodeNamespace* namespace_node)
{
	for (std::size_t i = 0; i < namespace_node->m_items.size(); ++i)
	{
			if (namespace_node->m_items[i]->m_type == AEN_FUNCTION)
			{
				emitFunctionCode(static_cast<aeNodeFunction*>(namespace_node->m_items[i]));
			}
			else if (namespace_node->m_items[i]->m_type == AEN_CLASS)
			{
				emitClassCode(static_cast<aeNodeClass*>(namespace_node->m_items[i]));
			}
			else if (namespace_node->m_items[i]->m_type == AEN_NAMESPACE)
			{
				emitNamespaceCode(static_cast<aeNodeNamespace*>(namespace_node->m_items[i]));
			}
			else if (namespace_node->m_items[i]->m_type == VarExpr)
			{
				emitGlobalVarCode(static_cast<aeNodeVarRef*>(namespace_node->m_items[i]));
			}
	 }
}

void aeon_compiler::emitGlobalVarCode(aeNodeVarRef* global_var)
{
	
}

void aeon_compiler::emitClassCode(aeNodeClass* clss)
{
	m_classes.push_back(clss);

	if (clss->parents.size() > 1 && !m_env->m_config.allowMultipleInheritance)
	{
		throwError("The class '" + clss->m_name + "' can't inherit from multiple classes.");
		return;
	}

	aeon_type* typeInfo = new aeon_type();
	typeInfo->m_name = clss->m_name;
	typeInfo->m_module = m_module;
	m_env->typedb.push_back(typeInfo);
	m_module->types.push_back(*typeInfo);
	aeon_context::object_heap objectHeap;
	objectHeap.type = typeInfo;
	m_env->object_heaps.push_back(objectHeap);

	clss->m_typeInfo = typeInfo;

	printf("Compiled type %s\n", typeInfo->m_name.c_str());

	for (auto pnt : clss->parents)
	{
		aeon_type* info = m_env->getTypeInfo(pnt.parentClass);
		if (!info)
		{
			printf("Compiler Error C0001: Can't inherit from '%s'. No such type.\n", pnt.parentClass.c_str());
		}
	}

	for (std::size_t i = 0; i < clss->m_items.size(); ++i)
	{
		if (clss->m_items[i]->m_type == AEN_FUNCTION)
		{
			emitFunctionCode(static_cast<aeNodeFunction*>(clss->m_items[i]));
		}
		else if (clss->m_items[i]->m_type == AEN_CLASS)
		{
			emitClassCode(static_cast<aeNodeClass*>(clss->m_items[i]));
		}
		else if (clss->m_items[i]->m_type == VarExpr)
		{
			aeNodeVarRef* varDecl = (aeNodeVarRef*)clss->m_items[i];

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

void aeon_compiler::emitBranchCode(aeNodeBranch* cond)
{
	/*aeNodeExpr* test_expr = cond->m_expression;
	aeNodeBlock* nested_code = cond->m_block;

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
	setinst_a(m_module->instructions[jmptestpc], (m_cursor - 1) - jmptestpc);*/
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

void aeon_compiler::emitPrefixIncrOp(aeNodeUnaryOperator* expr)
{

}

void aeon_compiler::emitForLoop(aeNodeFor* forloop)
{
	/*push_scope();

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

	pop_scope();*/
}


void aeon_compiler::emitWhileLoop(aeNodeWhile* whileloop)
{
	/*int pc_expreval = cursor();

	ExpressionEvalContext ectx;
	emitExpressionEval(whileloop->expr, ectx);

	int jmptestpc = emitInstruction(OP_JZ, 0, 0, 0);

	emitScopeCode(whileloop->block);

	// After the block terminates normally, we always go back up to expression evaluation
	// when the expression fails, it will jump right away to after this
	emitInstruction(OP_JMP, pc_expreval - 1);

	// if the expression evaluates false, jump to after the while
	setinst_a(m_module->instructions[jmptestpc], (cursor() - 1) - jmptestpc);*/
}

void aeon_compiler::emitFunctionCode(aeNodeFunction* func)
{
	m_OffsetFromBasePtr = 0;
	m_caller = func;

	// If we are a method, we reserve space for the THIS ptr
	if (func->isNonStaticMethod())
	{
		m_OffsetFromBasePtr += sizeof(vm_value);
		printf("Compiling non static method %s\n", func->m_name.c_str());
	}

	aeon_function functiondata;
	functiondata.offset = m_cursor;
	functiondata.prototype = func->m_name;
	m_module->functions.push_back(functiondata);

	aeFunctionData g_functionData;
	g_functionData.index = m_cursor;
	g_functionData.module = m_module;
	m_env->m_function_table.push_back(g_functionData);

	// let's just generate code for the executable block
	emitScopeCode(func->m_block.get());

	emitReturnCode(nullptr);
	m_caller = nullptr;
}

void aeon_compiler::emitScopeCode(aeNodeBlock* codeblock)
{
	push_scope();

	for (std::size_t i = 0; i < codeblock->m_items.size(); ++i)
	{
		if (codeblock->m_items[i]->m_type == FuncCall)
		{
			emitFunctionCall(static_cast<aeNodeFunctionCall*>(codeblock->m_items[i]), ExpressionEvalContext());
		}
		else if (codeblock->m_items[i]->m_type == IfBranch)
		{
			emitBranchCode(static_cast<aeNodeBranch*>(codeblock->m_items[i]));
		}
		else if (codeblock->m_items[i]->m_type == WhileLoop)
		{
			emitWhileLoop(static_cast<aeNodeWhile*>(codeblock->m_items[i]));
		}
		else if (codeblock->m_items[i]->m_type == ForLoop)
		{
			emitForLoop(static_cast<aeNodeFor*>(codeblock->m_items[i]));
		}
		else if (codeblock->m_items[i]->m_type == AEN_VARDECL)
		{
			emitVarDecl(static_cast<aeNodeVarDecl*>(codeblock->m_items[i]));
		}
		else
		{
			// might be buggy to assume this
			emitExpressionEval(static_cast<aeNodeExpr*>(codeblock->m_items[i]), ExpressionEvalContext());
		}
	}

	pop_scope();
}

void aeon_compiler::emitVarDecl(aeNodeVarDecl* varDecl)
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
	m_OffsetFromBasePtr += sizeof(vm_value);

	emitDebugPrint("Constructing " + varDecl->type_name);
	// Emit the stack allocation code
	emitInstruction(OP_MOV, AEK_ESP, -sizeof(vm_value));

	// Now the variable is initialized and part of its scope, initialize it
	if (varDecl->init_expr)
	{
		emitExpressionEval(varDecl->init_expr.get(), ExpressionEvalContext());
	}
}

void aeon_compiler::emitReturnCode(aeNodeReturn* ret)
{
	emitInstruction(OP_RETURN);
}

aeon_type* aeon_compiler::evaluateType(aeNodeExpr* expr)
{
	aeNodeVarRef* var = (aeNodeVarRef*)expr;
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
		aeNodeClass* container_class = m_classes[m_classes.size() - 1];
		aeon_type* class_type = evaluateType(container_class);

		// todo
	}

	/// Can still be a primitive
	aeon_type* type = m_env->getTypeInfo(type_name);

	/// Cannot evaluate.
	return type;
}

aeon_type* aeon_compiler::evaluateType(aeNodeClass* class_node)
{
	return class_node->m_typeInfo;
}