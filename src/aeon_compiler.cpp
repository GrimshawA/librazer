#include "aeon_compiler.h"
#include "aeon_bytecode.h"
#include "aeon_vm.h"
#include "aeon_context.h"
#include "aeon_expression.h"

#include <cassert>

int32_t aeon_compiler::cursor()
{
	return currOffset;
}

void aeon_compiler::emit_instruction(aeon_instruction instr)
{
	mod->instructions.push_back(instr);
	currOffset++;
}

uint32_t aeon_compiler::emit_instruction(uint8_t opcode, int8_t arg0, int8_t arg1, int8_t arg2)
{
	aeon_instruction instr;
	instr.opcode = opcode;
	instr.arg0 = arg0;
	instr.arg1 = arg1;
	mod->instructions.push_back(instr);
	return currOffset++;
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

void aeon_compiler::emit_local_destruct(LocalObjectInstance& var)
{
	emit_instruction(OP_MOV, AEK_ESP, var.type->getSize());
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
			compile_function(static_cast<aeon_ast_function*>(root->items[i]));
		}
		else if (root->items[i]->type == aeon_ast_node::ClassDecl)
		{
			compile_class(static_cast<ast_class*>(root->items[i]));
		}
		else if (root->items[i]->type == aeon_ast_node::Namespace)
		{
			compile_namespace(static_cast<ast_namespace*>(root->items[i]));
		}
		else if (root->items[i]->type == aeon_ast_node::VarExpr)
		{
			compile_global_var(static_cast<ast_varexpr*>(root->items[i]));
		}
	}
}

void aeon_compiler::compile_namespace(ast_namespace* namespace_node)
{
	for (std::size_t i = 0; i < namespace_node->items.size(); ++i)
	{
			if (namespace_node->items[i]->type == aeon_ast_node::FuncDecl)
			{
				compile_function(static_cast<aeon_ast_function*>(namespace_node->items[i]));
			}
			else if (namespace_node->items[i]->type == aeon_ast_node::ClassDecl)
			{
				compile_class(static_cast<ast_class*>(namespace_node->items[i]));
			}
			else if (namespace_node->items[i]->type == aeon_ast_node::Namespace)
			{
				compile_namespace(static_cast<ast_namespace*>(namespace_node->items[i]));
			}
			else if (namespace_node->items[i]->type == aeon_ast_node::VarExpr)
			{
				compile_global_var(static_cast<ast_varexpr*>(namespace_node->items[i]));
			}
	 }
}

void aeon_compiler::compile_global_var(ast_varexpr* global_var)
{
		aeon_vm::SFLocalVar gvar;
		vm->push(0);
		gvar = vm->m_stk.esp;
		vm->localsStack.push_back(gvar);

		FunctionLocalObjectInfo objinfo;
		objinfo.name = global_var->Name;
		mFunctionLocals.push_back(objinfo);

	//	Log("Global %s ready for use in the vm", global_var->Name.c_str());
}

void aeon_compiler::compile_class(ast_class* clss)
{
	if (clss->parents.size() > 1 && !env->m_config.allowMultipleInheritance)
	{
		throwError("The class '" + clss->Name + "' can't inherit from multiple classes.");
		return;
	}

	aeon_type* typeInfo = new aeon_type();
	typeInfo->m_name = clss->Name;
	typeInfo->m_module = mod;
	env->typedb.push_back(typeInfo);
	mod->types.push_back(*typeInfo);
	aeon_context::object_heap objectHeap;
	objectHeap.type = typeInfo;
	env->object_heaps.push_back(objectHeap);

	printf("Registered type %s\n", typeInfo->m_name.c_str());

	for (auto pnt : clss->parents)
	{
		aeon_type* info = env->getTypeInfo(pnt.parentClass);
		if (!info)
		{
			printf("Compiler Error C0001: Can't inherit from '%s'. No such type.\n", pnt.parentClass.c_str());
		}
	}

	for (std::size_t i = 0; i < clss->items.size(); ++i)
	{
		if (clss->items[i]->type == aeon_ast_node::FuncDecl)
		{
			compile_function(static_cast<aeon_ast_function*>(clss->items[i]));
		}
		else if (clss->items[i]->type == aeon_ast_node::ClassDecl)
		{
			compile_class(static_cast<ast_class*>(clss->items[i]));
		}
		else if (clss->items[i]->type == aeon_ast_node::VarExpr)
		{
			ast_varexpr* varDecl = (ast_varexpr*)clss->items[i];

			aeon_type* fieldType = env->getTypeInfo(varDecl->VarType->name);
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
}

void aeon_compiler::throwError(const std::string& message)
{
	printf("CXXX: %s\n", message.c_str());
}

void aeon_compiler::compile_if(ast_ifbranch* cond)
{
	aeon_expression* test_expr = cond->expr;
	ast_codeblock* nested_code = cond->block;

	// The expression must evaluate first
	compile_expreval(test_expr);

	// Now that the expression is evaluated and stored in a register, let's set the jmp
	aeon_instruction jmptest;
	setopcode(jmptest, EOpCodes::jz);
	setinst_a(jmptest, 0);
	emit_instruction(jmptest);
	int jmptestpc = currOffset - 1;

	// Now let's define the actual block
	compile_block(nested_code);

	// make sure the if jmp goes to the end of the block if it fails
	setinst_a(mod->instructions[jmptestpc], (currOffset - 1) - jmptestpc);
}

int aeon_compiler::findLocalObject(const std::string& refname)
{
	for (std::size_t i = 0; i < mFunctionLocals.size(); ++i)
	{
		if (mFunctionLocals[i].name == refname)
			return i;
	}
	return -1;
}

void aeon_compiler::compile_expreval(aeon_expression* expr)
{
	if (expr->type == aeon_ast_node::VarExpr)
	{
		// push var contents to stack
		ast_varexpr* varexpr = static_cast<ast_varexpr*>(expr);

		aeon_instruction loadlocal;
		setopcode(loadlocal, EOpCodes::iload);
		setinst_a(loadlocal, findLocalObject(varexpr->Name));
		emit_instruction(loadlocal);

		//Log("iload compiled");
	}
	else if (expr->type == aeon_ast_node::FuncCall)
	{
		compile_functioncall(static_cast<ast_funccall*>(expr));
	}
	else if (expr->type == aeon_ast_node::IntExpr)
	{
		// push the constant to stack
		aeon_instruction loadk;
		setopcode(loadk, EOpCodes::loadk);
		setinst_a(loadk, expr->as_int());
		emit_instruction(loadk);
	}
	else if (expr->type == aeon_ast_node::StringExpr)
	{
		aeon_instruction loadk;
		setopcode(loadk, EOpCodes::loadk);
		setinst_a(loadk, mod->storeString(expr->as_string()));
		emit_instruction(loadk);
			//Log("Compiled loadk instruction for %d", vm->storeString(expr->as_string()));
	}
	else if (expr->type == aeon_ast_node::BinaryOperator)
	{
		ast_binaryop* binaryop = static_cast<ast_binaryop*>(expr);
		int pc = currOffset - 1;		
		if (binaryop->oper == "*")
		{
			aeon_instruction mul;
			setopcode(mul, EOpCodes::mul);
			emit_instruction(mul);
		}
		else if (binaryop->oper == "/")
		{
			aeon_instruction mul;
			setopcode(mul, EOpCodes::OP_DIV);
			emit_instruction(mul);
		}
		else if (binaryop->oper == "%")
		{
			aeon_instruction mul;
			setopcode(mul, EOpCodes::mod);
			emit_instruction(mul);
		}
		else if (binaryop->oper == "+")
		{
			compile_expreval(binaryop->operandA);
			compile_expreval(binaryop->operandB);

			aeon_instruction mul;
			setopcode(mul, EOpCodes::add);
			emit_instruction(mul);
		}
		else if (binaryop->oper == "-")
		{
			aeon_instruction mul;
			setopcode(mul, EOpCodes::sub);
			emit_instruction(mul);
		}
		else if (binaryop->oper == "<")
		{
			compile_expreval(binaryop->operandA);
			compile_expreval(binaryop->operandB);

			aeon_instruction cmplt;
			setopcode(cmplt, EOpCodes::lt);
			emit_instruction(cmplt);
			//Log("< compiled");
		}
		else if (binaryop->oper == ">")
		{
			aeon_instruction cmplt;
			setopcode(cmplt, EOpCodes::gt);
			emit_instruction(cmplt);
			//Log("> compiled");
		}
		else if (binaryop->oper == "=")
		{
			emitAssignOp(binaryop);
		}
	}
}

void aeon_compiler::emitAssignOp(ast_binaryop* assign_expr)
{
	// Define a strategy to define A by B, in A = B

	// We load the left side as a ref to assign to later
	// Then we resolve the right side and finally set
	ast_varexpr* toBeAssigned = static_cast<ast_varexpr*>(assign_expr->operandA);

	/// Load the address to assign to
	emit_instruction(iloadaddr, findLocalObject(toBeAssigned->Name));

	/// Get the RHS on stack
	emitExpressionEval(assign_expr->operandB, AE_RVALUE);

	/// This instruction simply copies N bytes to the loaded addr
	emit_instruction(OP_SET, sizeof(int), 1);
}

void aeon_compiler::emitPrefixIncrOp(ast_unaryop* expr)
{

}

void aeon_compiler::emitExpressionEval(aeon_expression* expression, int flag)
{

}

void aeon_compiler::emitForLoop(ast_for* forloop)
{
	push_scope();

	// Emit the code that initializes the control vars
	// They are automatically scoped and are destructed in the end of the loop
	for (auto& initExpr : forloop->initExpressions)
	{
		compile_expreval(initExpr);
		emit_instruction(OP_DTEST, 4);
	}

	int pc_expreval = cursor(); // track the first instruction of this while, to jump back to evaluation each iteration

	compile_expreval(forloop->expr);

	// Evaluate the expression
	int jmptestpc = emit_instruction(jz, 0, 0, 0);

	compile_block(forloop->block);

	// After the code executes, apply the update expression
	compile_expreval(forloop->incrExpr);

	// After the block terminates normally, we always go back up to expression evaluation
	// when the expression fails, it will jump right away to after this
	emit_instruction(OP_JMP, pc_expreval);

	// if the expression evaluates false, jump to after the for
	setinst_a(mod->instructions[jmptestpc], (cursor() - 1) - jmptestpc);

	pop_scope();
}


void aeon_compiler::emitWhileLoop(ast_while* whileloop)
{
	int pc_expreval = cursor();

	compile_expreval(whileloop->expr);

	int jmptestpc = emit_instruction(jz, 0, 0, 0);

	compile_block(whileloop->block);

	// After the block terminates normally, we always go back up to expression evaluation
	// when the expression fails, it will jump right away to after this
	emit_instruction(OP_JMP, pc_expreval - 1);

	// if the expression evaluates false, jump to after the while
	setinst_a(mod->instructions[jmptestpc], (cursor() - 1) - jmptestpc);
}

void aeon_compiler::compile_function(aeon_ast_function* func)
{

	aeon_function functiondata;
	functiondata.offset = currOffset;
	functiondata.prototype = func->m_name;
	mod->functions.push_back(functiondata);

	// let's just generate code for the executable block
	compile_block(func->m_block);

	emit_instruction(OP_RETURN);
}

void aeon_compiler::compile_functioncall(ast_funccall* funccall)
{
		// Each child of the funccall is one argument to generate code for
		int j = funccall->args.size() - 1;
		while (j >= 0)
		{
			aeon_expression* expr = static_cast<aeon_expression*>(funccall->args[j]);
			compile_expreval(expr);

			--j;
		}

		int funcId = env->getNativeFunctionIndex(funccall->funcName);
		if (funcId >= 0)
		{
			// Now the actual call
			aeon_instruction fcallinst;
			setopcode(fcallinst, EOpCodes::OP_CALLNATIVE);
			setinst_a(fcallinst, funcId);
			setinst_b(fcallinst, funccall->args.size());
			emit_instruction(fcallinst);
		}
		else
		{
			if (mod->getFunctionIndexByName(funccall->funcName) < 0)
			{
				throwError("Function '" + funccall->funcName + "' not found!");
			}
			else
			{
				// we'll need to call a nested func
				aeon_instruction fcallinst;
				setopcode(fcallinst, EOpCodes::OP_CALL);
				setinst_a(fcallinst, mod->getFunctionIndexByName(funccall->funcName));
				setinst_b(fcallinst, funccall->items.size());
				emit_instruction(fcallinst);
			}
		}
}

void aeon_compiler::compile_block(ast_codeblock* codeblock)
{
		BlockUnwindData blockUnwindData;

		for (std::size_t i = 0; i < codeblock->items.size(); ++i)
		{
			switch (codeblock->items[i]->type)
			{
			case aeon_ast_node::FuncCall:
			{
											compile_functioncall(static_cast<ast_funccall*>(codeblock->items[i]));
											break;
			}

			case aeon_ast_node::IfBranch:
			{
											compile_if(static_cast<ast_ifbranch*>(codeblock->items[i]));
											break;
			}

			case aeon_ast_node::WhileLoop:
			{
											 emitWhileLoop(static_cast<ast_while*>(codeblock->items[i]));
											 break;
			}

			case aeon_ast_node::ForLoop:
			{

				emitForLoop(static_cast<ast_for*>(codeblock->items[i]));
				break;
			}

			case aeon_ast_node::VarExpr:
			{
										   ast_varexpr* var = static_cast<ast_varexpr*>(codeblock->items[i]);
										   if (findLocalObject(var->Name) == -1)
										   {
											   FunctionLocalObjectInfo localInfo;
											   localInfo.name = var->Name;
											   mFunctionLocals.push_back(localInfo);

											   blockUnwindData.localIndex.push_back(mFunctionLocals.size() - 1);

											   aeon_instruction addlocal;
											   setopcode(addlocal, EOpCodes::ilocal);
											   emit_instruction(addlocal);

											  // Log("ilocal");
										   }
										   break;
			}

			case aeon_ast_node::BinaryOperator:
			{
												  ast_binaryop* binaryop = static_cast<ast_binaryop*>(codeblock->items[i]);
												  if (binaryop->oper == "=")
												  {
													  emitAssignOp(binaryop);
												  }
												  break;
			}

			}
		}

		// Emit the block ending code for releasing allocated locals
		for (auto it = blockUnwindData.localIndex.rbegin(); it != blockUnwindData.localIndex.rend(); ++it)
		{
			//mFunctionLocals.erase(mFunctionLocals.begin() + index);
			aeon_instruction inst = create_instruction(EOpCodes::poplocal);
			setinst_a(inst, 8);
			emit_instruction(inst);
		}
}

void aeon_compiler::compile_return(ast_return* ret)
{

}