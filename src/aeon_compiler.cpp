#include "aeon_compiler.h"
#include "aeon_bytecode.h"
#include "aeon_vm.h"
#include "aeon_context.h"

void aeon_compiler::emit_instruction(aeon_instruction instr)
{
	mod->instructions.push_back(instr);
	currOffset++;
}

void aeon_compiler::emit_scope_begin()
{

}

void aeon_compiler::emit_scope_end()
{

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

void aeon_compiler::generate(atom_ast_node* root)
{
	for (std::size_t i = 0; i < root->items.size(); ++i)
	{
		if (root->items[i]->type == atom_ast_node::FuncDecl)
		{
			compile_function(static_cast<ast_func*>(root->items[i]));
		}
		else if (root->items[i]->type == atom_ast_node::ClassDecl)
		{
			compile_class(static_cast<ast_class*>(root->items[i]));
		}
		else if (root->items[i]->type == atom_ast_node::Namespace)
		{
			compile_namespace(static_cast<ast_namespace*>(root->items[i]));
		}
		else if (root->items[i]->type == atom_ast_node::VarExpr)
		{
			compile_global_var(static_cast<ast_varexpr*>(root->items[i]));
		}
	}
}

void aeon_compiler::compile_namespace(ast_namespace* namespace_node)
{
	for (std::size_t i = 0; i < namespace_node->items.size(); ++i)
	{
			if (namespace_node->items[i]->type == atom_ast_node::FuncDecl)
			{
				compile_function(static_cast<ast_func*>(namespace_node->items[i]));
			}
			else if (namespace_node->items[i]->type == atom_ast_node::ClassDecl)
			{
				compile_class(static_cast<ast_class*>(namespace_node->items[i]));
			}
			else if (namespace_node->items[i]->type == atom_ast_node::Namespace)
			{
				compile_namespace(static_cast<ast_namespace*>(namespace_node->items[i]));
			}
			else if (namespace_node->items[i]->type == atom_ast_node::VarExpr)
			{
				compile_global_var(static_cast<ast_varexpr*>(namespace_node->items[i]));
			}
	 }
}

	void aeon_compiler::compile_global_var(ast_varexpr* global_var)
	{
		aeon_vm::SFLocalVar gvar;
		vm->push(0);
		gvar = vm->sp;
		vm->localsStack.push_back(gvar);

		FunctionLocalObjectInfo objinfo;
		objinfo.name = global_var->Name;
		mFunctionLocals.push_back(objinfo);

	//	Log("Global %s ready for use in the vm", global_var->Name.c_str());
	}

void aeon_compiler::compile_class(ast_class* clss)
{
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
		if (clss->items[i]->type == atom_ast_node::FuncDecl)
		{
			compile_function(static_cast<ast_func*>(clss->items[i]));
		}
		else if (clss->items[i]->type == atom_ast_node::ClassDecl)
		{
			compile_class(static_cast<ast_class*>(clss->items[i]));
		}
	}
}

	void aeon_compiler::compile_if(ast_ifbranch* cond)
	{
		ast_expr* test_expr = cond->expr;
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

	void aeon_compiler::compile_expreval(ast_expr* expr)
	{
		if (expr->type == atom_ast_node::VarExpr)
		{
			// push var contents to stack
			ast_varexpr* varexpr = static_cast<ast_varexpr*>(expr);

			aeon_instruction loadlocal;
			setopcode(loadlocal, EOpCodes::iload);
			setinst_a(loadlocal, findLocalObject(varexpr->Name));
			emit_instruction(loadlocal);

			//Log("iload compiled");
		}
		else if (expr->type == atom_ast_node::FuncCall)
		{
			compile_functioncall(static_cast<ast_funccall*>(expr));
		}
		else if (expr->type == atom_ast_node::IntExpr)
		{
			// push the constant to stack
			aeon_instruction loadk;
			setopcode(loadk, EOpCodes::loadk);
			setinst_a(loadk, expr->as_int());
			emit_instruction(loadk);
		}
		else if (expr->type == atom_ast_node::StringExpr)
		{
			aeon_instruction loadk;
			setopcode(loadk, EOpCodes::loadk);
			setinst_a(loadk, mod->storeString(expr->as_string()));
			emit_instruction(loadk);

			//Log("Compiled loadk instruction for %d", vm->storeString(expr->as_string()));
		}
		else if (expr->type == atom_ast_node::BinaryOperator)
		{
			ast_binaryop* binaryop = static_cast<ast_binaryop*>(expr);
			int pc = currOffset - 1;
			compile_expreval(binaryop->operandA);
			compile_expreval(binaryop->operandB);

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
				aeon_instruction assignop;
				setopcode(assignop, EOpCodes::set);
				emit_instruction(assignop);

				// super hack
				setopcode(mod->instructions[pc + 2], EOpCodes::iloadaddr);

				//Log("Compiled an = instruction");
			}
		}
	}

	void aeon_compiler::compile_assignment(ast_binaryop* assign_expr)
	{
		// We load the left side as a ref to assign to later
		// Then we resolve the right side and finally set

		ast_varexpr* toBeAssigned = static_cast<ast_varexpr*>(assign_expr->operandA);

		compile_expreval(assign_expr->operandB);

		aeon_instruction loadlocal;
		setopcode(loadlocal, EOpCodes::iloadaddr);
		setinst_a(loadlocal, findLocalObject(toBeAssigned->Name));
		emit_instruction(loadlocal);

		aeon_instruction assignop;
		setopcode(assignop, EOpCodes::set);
		emit_instruction(assignop);
	}

	void aeon_compiler::compile_while(ast_while* whileloop)
	{
		int pc_expreval = currOffset; // track the first instruction of this while, to jump back to evaluation each iteration

		compile_expreval(whileloop->expr);

		aeon_instruction jmptest;
		setopcode(jmptest, EOpCodes::jz);
		setinst_a(jmptest, 0);
		emit_instruction(jmptest);
		int jmptestpc = currOffset - 1;

		compile_block(whileloop->block);

		// After the block terminates normally, we always go back up to expression evaluation
		// when the expression fails, it will jump right away to after this
		aeon_instruction backtoloopeval;
		setopcode(backtoloopeval, EOpCodes::jmp);
		setinst_a(backtoloopeval, pc_expreval - 1);
		emit_instruction(backtoloopeval);

		// if the expression evaluates false, jump to after the while
		setinst_a(mod->instructions[jmptestpc], (currOffset - 1) - jmptestpc);
	}

	void aeon_compiler::compile_function(ast_func* func)
	{
		aeon_function functiondata;
		functiondata.offset = currOffset;
		functiondata.prototype = func->Name;
		mod->functions.push_back(functiondata);

		// let's just generate code for the executable block
		compile_block(func->block);

		aeon_instruction retinstr;
		setopcode(retinstr, EOpCodes::ret);
		emit_instruction(retinstr);
	}

	void aeon_compiler::compile_functioncall(ast_funccall* funccall)
	{
		// Each child of the funccall is one argument to generate code for
		int j = funccall->args.size() - 1;
		while (j >= 0)
		{
			ast_expr* expr = static_cast<ast_expr*>(funccall->args[j]);
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
			// we'll need to call a nested func
			aeon_instruction fcallinst;
			setopcode(fcallinst, EOpCodes::OP_CALL);
			setinst_a(fcallinst, mod->getFunctionIndexByName(funccall->funcName));
			setinst_b(fcallinst, funccall->items.size());
			emit_instruction(fcallinst);
		}
	}

	void aeon_compiler::compile_block(ast_codeblock* codeblock)
	{
		BlockUnwindData blockUnwindData;

		for (std::size_t i = 0; i < codeblock->items.size(); ++i)
		{
			switch (codeblock->items[i]->type)
			{
			case atom_ast_node::FuncCall:
			{
											compile_functioncall(static_cast<ast_funccall*>(codeblock->items[i]));
											break;
			}

			case atom_ast_node::IfBranch:
			{
											compile_if(static_cast<ast_ifbranch*>(codeblock->items[i]));
											break;
			}

			case atom_ast_node::WhileLoop:
			{
											 compile_while(static_cast<ast_while*>(codeblock->items[i]));
											 break;
			}

			case atom_ast_node::VarExpr:
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

			case atom_ast_node::BinaryOperator:
			{
												  ast_binaryop* binaryop = static_cast<ast_binaryop*>(codeblock->items[i]);
												  if (binaryop->oper == "=")
												  {
													  compile_assignment(binaryop);
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

	void aeon_compiler::compile_blockend()
	{

	}

	void aeon_compiler::compile_return(ast_return* ret)
	{

	}