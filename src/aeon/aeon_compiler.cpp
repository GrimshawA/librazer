#include "aeon_compiler.h"
#include "aeon_bytecode.h"
#include <aeon/aeVM.h>
#include "aeon_context.h"
#include "nodes/aeNodeNew.h"
#include "nodes/aeNodeClass.h"
#include "nodes/aeNodeNamespace.h"

#include <cassert>

aeon_compiler::aeon_compiler()
: m_currentFunction(nullptr)
{
	m_logAllocs = true;
	m_logExprStmt = true;
	m_logExprOps = true;

	m_OffsetFromBasePtr = 0;
	m_caller = nullptr;

	m_typeSystem.init();
}

int32_t aeon_compiler::cursor()
{ 
	return m_cursor;
}

aeQualType aeon_compiler::buildQualifiedType(const std::string& type)
{
	aeQualType qtype;
	qtype.m_type = m_env->getTypeInfo(type);
	return qtype;
}

aeQualType aeon_compiler::buildQualifiedType(aeNodeExpr* e)
{
	return e->getQualifiedType(this);
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

aeNodeClass* aeon_compiler::getTopClassNode()
{
	if (m_classes.size() > 0)
		return m_classes[m_classes.size() - 1];
	return nullptr;
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
	for (auto it = top.locals.rbegin(); it != top.locals.rend(); ++it)
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
		aeNodeClass* classNode = getTopClassNode();
		aeType* type = evaluateType(classNode);

		if (type->getField(name))
		{
			// The variable we are referencing is a member of 'this'
			VariableStorageInfo varStorage;
			varStorage.type = type->getField(name)->type;
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
	if (var.type.getType()->is_native)
	{
		//emitDebugPrint("Destroying a " + var.name);
		//emitInstrugction(OP_LOAD, AEK_ESP);
		//emitInstruction(OP_CALLMETHOD_NAT, m_env->getNativeBehaviorIndex(var.type.getName(), "~f"));
		emitInstruction(OP_MOV, AEK_ESP, var.type.getSize());
	}
	else
	{
		emitInstruction(OP_MOV, AEK_ESP, var.type.getSize());
	}
}

void aeon_compiler::emitBreakpoint()
{
	emitInstruction(OP_BREAKPOINT);
}

void aeon_compiler::emit_local_construct_pod(int32_t size)
{

}

void aeon_compiler::emit_local_construct_object(int32_t size)
{

}

void aeon_compiler::generate(aeNodeBase* root)
{
	// Entry point, let's set a proper offset
	m_cursor = m_module->instructions.size();

	for (std::size_t i = 0; i < root->m_items.size(); ++i)
	{
		if (root->m_items[i]->m_nodeType == AEN_FUNCTION)
		{
			emitFunction(static_cast<aeNodeFunction*>(root->m_items[i]));
		}
		else if (root->m_items[i]->m_nodeType == AEN_CLASS)
		{
			emitClassCode(static_cast<aeNodeClass*>(root->m_items[i]));
		}
		else if (root->m_items[i]->m_nodeType == AEN_NAMESPACE)
		{
			emitNamespaceCode(static_cast<aeNodeNamespace*>(root->m_items[i]));
		}
		else if (root->m_items[i]->m_nodeType == AEN_REF)
		{
			emitGlobalVarCode(static_cast<aeNodeRef*>(root->m_items[i]));
		}
	}
}

void aeon_compiler::emitNamespaceCode(aeNodeNamespace* namespace_node)
{
	for (std::size_t i = 0; i < namespace_node->m_items.size(); ++i)
	{
			if (namespace_node->m_items[i]->m_nodeType == AEN_FUNCTION)
			{
				emitFunction(static_cast<aeNodeFunction*>(namespace_node->m_items[i]));
			}
			else if (namespace_node->m_items[i]->m_nodeType == AEN_CLASS)
			{
				emitClassCode(static_cast<aeNodeClass*>(namespace_node->m_items[i]));
			}
			else if (namespace_node->m_items[i]->m_nodeType == AEN_NAMESPACE)
			{
				emitNamespaceCode(static_cast<aeNodeNamespace*>(namespace_node->m_items[i]));
			}
			else if (namespace_node->m_items[i]->m_nodeType == AEN_REF)
			{
				emitGlobalVarCode(static_cast<aeNodeRef*>(namespace_node->m_items[i]));
			}
	 }
}

void aeon_compiler::emitGlobalVarCode(aeNodeRef* global_var)
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

	aeType* typeInfo = new aeType();
	typeInfo->m_name = clss->m_name;
	typeInfo->m_absoluteName = clss->m_name;
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
		aeType* info = m_env->getTypeInfo(pnt.parentClass);
		if (!info)
		{
			printf("Compiler Error C0001: Can't inherit from '%s'. No such type.\n", pnt.parentClass.c_str());
		}
	}

	for (std::size_t i = 0; i < clss->m_items.size(); ++i)
	{
		if (clss->m_items[i]->m_nodeType == AEN_FUNCTION)
		{
			emitFunction(static_cast<aeNodeFunction*>(clss->m_items[i]));
		}
		else if (clss->m_items[i]->m_nodeType == AEN_CLASS)
		{
			emitClassCode(static_cast<aeNodeClass*>(clss->m_items[i]));
		}
		else if (clss->m_items[i]->m_nodeType == AEN_VARDECL)
		{
			aeNodeVarDecl* varDecl = (aeNodeVarDecl*)clss->m_items[i];

			for (auto i : varDecl->m_decls)
			{
				aeField fieldInfo;
				fieldInfo.name = i.m_name;
				fieldInfo.size = 4;
				fieldInfo.type = varDecl->m_type;
				typeInfo->createField(fieldInfo);

				printf("REGISTERING TYPE VAR %s\n", i.m_name.c_str());
			}

		}
	}

	m_classes.pop_back();
}

void aeon_compiler::throwError(const std::string& message)
{
	m_reporter.emitLog(message);

	if (m_currentFunction)
	{
		m_currentFunction->m_compiled = false; // Compilation failed
	}
}

void aeon_compiler::emitBranchCode(aeNodeBranch* cond)
{
	aeNodeExpr* test_expr = cond->m_expression.get();
	aeNodeBlock* nested_code = cond->m_block.get();

	// The expression must evaluate first
	aeExprContext exprContext;
	exprContext.expectedResult = aeQualType(m_env->getTypeInfo("bool"));
	emitExpressionEval(test_expr, aeExprContext());

	// Now that the expression is evaluated and stored in a register, let's set the jmp
	emitInstruction(OP_JZ, 0, 0);
	int jmptestpc = m_cursor - 1;

	// Now let's define the actual block
	emitBlock(nested_code);

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

void aeon_compiler::emitPrefixIncrOp(aeNodeUnaryOperator* expr)
{

}

void aeon_compiler::emitForLoop(aeNodeFor* forloop)
{
	push_scope();

	// Emit the code that initializes the control vars
	// They are automatically scoped and are destructed in the end of the loop
	emitStatement(forloop->initStatement.get());

	int pc_expreval = cursor(); // track the first instruction of this while, to jump back to evaluation each iteration

	aeExprContext ectx;
	ectx.must_be_rvalue = true;
	emitExpressionEval(forloop->expr.get(), ectx);

	// Evaluate the expression
	int jzInstrIndex = emitInstruction(OP_JZ, 0, 0, 0);

	emitBlock(forloop->block.get());

	// After the code executes, apply the update expression
	emitExpressionEval(forloop->incrExpr.get(), ectx);

	// After the block terminates normally, we always go back up to expression evaluation
	// when the expression fails, it will jump right away to after this
	emitInstruction(OP_JMP, pc_expreval - 1);

	// if the expression evaluates false, jump to after the for
	setinst_a(m_module->instructions[jzInstrIndex], cursor() - jzInstrIndex - 1);

	pop_scope();
}


void aeon_compiler::emitWhileLoop(aeNodeWhile* whileloop)
{
	int pc_expreval = cursor();

	aeExprContext ectx;
	ectx.must_be_rvalue = true;
	emitExpressionEval(whileloop->expr.get(), ectx);

	int jmptestpc = emitInstruction(OP_JZ, 0, 0, 0);

	emitBlock(whileloop->block.get());

	// After the block terminates normally, we always go back up to expression evaluation
	// when the expression fails, it will jump right away to after this
	emitInstruction(OP_JMP, pc_expreval - 1);

	// if the expression evaluates false, jump to after the while
	setinst_a(m_module->instructions[jmptestpc], (cursor() - 1) - jmptestpc);
}

void aeon_compiler::emitFunction(aeNodeFunction* func)
{
	std::string symbol_prefix;
	auto topClass = getTopClassNode();
	if (topClass)
	{
		symbol_prefix = topClass->m_name + ".";
	}

	aeFunction* function = m_env->createFunction(symbol_prefix + func->m_name);
	// Compiled unless some error is thrown
	m_currentFunction = function;
	function->m_compiled = true;

	//m_module->functions.push_back(function);

	aeQualType returnType = func->getReturnType();

	m_OffsetFromBasePtr = 0;

	// The function must be aware how much time it needs for the return value
	function->returnValueSize = returnType.getSize();

	m_caller = func;

	// If we are a method, we reserve space for the THIS ptr
	if (func->isNonStaticMethod())
	{
		m_OffsetFromBasePtr += sizeof(vm_value);
		printf("Compiling non static method %s\n", function->getSymbolName().c_str());
	}

	function->offset = m_cursor;
	function->m_absoluteName = symbol_prefix + func->m_name;
	function->m_module = m_module;

	// let's just generate code for the executable block
	emitBlock(func->m_block.get());

	//emitBreakpoint();

	emitReturnCode(nullptr);
	m_caller = nullptr;
	m_currentFunction = nullptr;
}

void aeon_compiler::emitLambdaFunction(aeNodeFunction* function)
{
	// todo
}

void aeon_compiler::emitBlock(aeNodeBlock* codeblock)
{
	push_scope();

	for (std::size_t i = 0; i < codeblock->m_items.size(); ++i)
	{
		emitStatement(static_cast<aeNodeStatement*>(codeblock->m_items[i]));
	}
	
	pop_scope();
}

void aeon_compiler::emitStatement(aeNodeStatement* stmt)
{
	/*
		List of statements accepted as "standalone" within blocks of code
		- switch
		- for
		- while
		- if
		- assignment
		- function calls (f(x) or (...).f(X)
		- declaration statement
	*/

	printf("Compiling statement '%s'\n", stmt->str().c_str());

	if (stmt->m_nodeType == AEN_FUNCTIONCALL)
	{
		emitFunctionCall(aeQualType(), static_cast<aeNodeFunctionCall*>(stmt), aeExprContext());
	}
	else if (stmt->m_nodeType == AEN_BRANCH)
	{
		emitBranchCode(static_cast<aeNodeBranch*>(stmt));
	}
	else if (stmt->m_nodeType == AEN_WHILE)
	{
		emitWhileLoop(static_cast<aeNodeWhile*>(stmt));
	}
	else if (stmt->m_nodeType == AEN_FOR)
	{
		emitForLoop(static_cast<aeNodeFor*>(stmt));
	}
	else if (stmt->m_nodeType == AEN_VARDECL)
	{
		emitVarDecl(*static_cast<aeNodeVarDecl*>(stmt));
	}
	else if (stmt->m_nodeType == AEN_BINARYOP && ((aeNodeBinaryOperator*)stmt)->oper == "=")
	{
		if (m_logExprStmt)
			emitDebugPrint("Evaluating " + stmt->str());

		emitAssignOp(((aeNodeBinaryOperator*)stmt)->operandA, ((aeNodeBinaryOperator*)stmt)->operandB);
	}
	else if (stmt->m_nodeType == AEN_ACCESSOPERATOR && ((aeNodeAccessOperator*)stmt)->m_b->m_nodeType == AEN_FUNCTIONCALL)
	{
		if (m_logExprStmt)
			emitDebugPrint("Evaluating " + stmt->str());

		emitMemberOp(((aeNodeAccessOperator*)stmt));
	}
	else
	{
		printf("NOT compiling other expression '%s'\n", ((aeNodeExpr*)stmt)->str().c_str());
	}
}

void aeon_compiler::emitVarDecl(const aeNodeVarDecl& varDecl)
{
	aeType* varType = varDecl.m_type.m_type;
	auto& scope = m_scopes[m_scopes.size() - 1]; 

	VariableStorageInfo localObject;
	localObject.type = varDecl.m_type;
	localObject.name = varDecl.m_decls[0].m_name;
	localObject.offset = scope.offset;
	localObject.offset_bp = m_OffsetFromBasePtr;
	localObject.mode = AE_VAR_LOCAL;
	scope.locals.push_back(localObject);

	scope.offset += localObject.type.getSize();
	m_OffsetFromBasePtr += localObject.type.getSize();

	if (m_logAllocs)
		emitDebugPrint("Allocating " + localObject.name);

	emitInstruction(OP_MOV, AEK_ESP, -varType->getSize());

	// Now the variable is initialized and part of its scope, initialize it
	if (varDecl.m_decls[0].m_init)
	{
		if (m_logAllocs)
			emitDebugPrint("Evaluating " + varDecl.m_decls[0].m_init->str());

		emitExpressionEval(varDecl.m_decls[0].m_init, aeExprContext());
	}
	else
	{
		// Default initialization
		if (varType->is_native)
		{
			emitInstruction(OP_LOAD, AEK_ESP);
			emitInstruction(OP_CALLMETHOD_NAT, m_env->getNativeBehaviorIndex(varType->getName(), "f"));
		}
	}
}

void aeon_compiler::emitReturnCode(aeNodeReturn* ret)
{
	emitInstruction(OP_RETURN);
}

aeType* aeon_compiler::evaluateType(aeNodeExpr* expr)
{
	return nullptr;
}

aeType* aeon_compiler::evaluateType(const std::string& type_name)
{
	// Walk the scope stack from current to top level to search the decl
	for (auto it = m_scopes.rbegin(); it != m_scopes.rend(); ++it)
	{
		// Is the variable we reference here?
		for (auto var_it = (*it).locals.rbegin(); var_it != (*it).locals.rend(); ++var_it)
		{
			if ((*var_it).name == type_name)
			{
				return (*var_it).type.getType();
			}
		}
	}

	// Are we compiling a method currently?
	// In that case we probably want a member variable
	if (!m_classes.empty())
	{
		aeNodeClass* container_class = m_classes[m_classes.size() - 1];
		aeType* class_type = evaluateType(container_class);

		// todo
	}

	/// Can still be a primitive
	aeType* type = m_env->getTypeInfo(type_name);

	/// Cannot evaluate.
	return type;
}

aeType* aeon_compiler::evaluateType(aeNodeClass* class_node)
{
	return class_node->m_typeInfo;
}