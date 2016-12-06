#include <RazerVM/VirtualMachine.h>
#include <RazerVM/ThreadContext.h>
#include <RazerVM/ThreadHandler.h>

#include <RazerVM/Executor/AEVmCalls.h>
#include <RazerVM/Executor/VariantOps.h>
#include <RazerVM/Executor/VmInstructions.h>

#define vm_start(x) case x:{
#define vm_end break;}

#define LOGVM(x, a, b, c) printf("[vm] %s %d %d %d\n", #x, a, b, c);

#define VMLOGLEVEL 5
#define vm_log(tag, level, STR) if(level > VMLOGLEVEL) printf("%s: %s\n", tag, STR);


void dispatch_execute(RzThreadContext& ctx)
{
	ctx.cl = &ctx.frames[ctx.frames.size() - 1];

	for (; ctx.cl->pc < ctx.cl->module->m_code.size(); ++ctx.cl->pc)
	{
		RzInstruction& inst = ctx.cl->module->m_code[ctx.cl->pc];
#ifdef TRACE_VM
		printf(" vm %s\n", inst_opcode_str(inst).c_str());
#endif

		switch (inst.opcode)
		{
			vm_start(OP_NEW)
				DoNewObject(ctx, inst.arg0, inst.arg1);
			vm_end

				vm_start(OP_PREPARE)
				AEFunction* functionData = ctx.engine->m_functionTable[inst.arg0];
			ctx.esp -= functionData->returnValueSize;
			vm_end

				vm_start(OP_CALL)
				DoCall(ctx, inst.arg0);
			vm_end

				vm_start(OP_DCALL)
				DoDynamicCall(ctx, inst.arg0);
			vm_end

				vm_start(OP_CALLVIRTUAL)
				// todo
				vm_end

				vm_start(OP_CALLNATIVE)
				/*uint32_t funcid = getinst_a(inst);
				uint32_t params = getinst_b(inst);

				atom_generic* gen = new atom_generic();
				gen->vm = this;
				// forward to generic
				gen->args.resize(params);
				for (int k = 0; k < params; ++k)
				gen->args[k]._u64 = pop();
				//Log("Stored %d in the generic before calling", params);

				// C Call
				ctx->native_functions[funcid].f(gen);

				// always push the return type
				if (gen->hasRet)
				push(gen->retarg._u64);

				delete gen;*/
				vm_end

				vm_start(OP_CALLMETHOD_NAT)
				AEGeneric g; //g.m_vm = this;
			ctx.engine->m_functionTable[inst.arg0]->fn(g);
			vm_end

				vm_start(OP_RETURN)
			if (DoReturn(ctx))
				return;
			vm_end

				vm_start(OP_JZ)
				DoJumpIfZero(ctx, inst.arg0);
			vm_end

				vm_start(OP_JMP)
				DoJump(ctx, inst.arg0);
			vm_end

				vm_start(OP_LOAD)
				DoLoad(ctx, inst.arg0, inst.arg1, inst.arg2);
			vm_end

				vm_start(OP_LOADADDR)
				DoLoadAddr(ctx, inst.arg0, inst.arg1, inst.arg2);
			vm_end

				vm_start(OP_LOADK)
				DoLoadConstant(ctx, inst.arg0, inst.arg1, inst.arg2);
			vm_end

				vm_start(OP_LOADENUM)
				RzStackValue v;
			v.i32 = inst.arg0;
			ctx.push_value(v);
			vm_end

				vm_start(OP_LT)
				DoLessThan(ctx, (AeonPrimitiveType)inst.arg0);
			vm_end

				vm_start(OP_LTE)

				vm_end

				vm_start(OP_GT)
				DoGreaterThan(ctx, (AeonPrimitiveType)inst.arg0);
			vm_end

				vm_start(OP_GTE)

				vm_end

				vm_start(OP_EQ)
				DoEquals(ctx, (AeonPrimitiveType)inst.arg0);
			vm_end

				vm_start(OP_NEQ)
				DoNotEquals(ctx, (AeonPrimitiveType)inst.arg0);
			vm_end

				vm_start(OP_NOT)
				DoLogicalNot(ctx);
			vm_end

				vm_start(OP_SET)
				DoAssign(ctx, inst.arg0, inst.arg1, inst.arg2);
			vm_end

				vm_start(OP_MUL)
				DoMul(ctx, (AeonPrimitiveType)inst.arg0);
			vm_end

				vm_start(OP_DIV)
				DoDiv(ctx, (AeonPrimitiveType)inst.arg0);
			vm_end

				vm_start(OP_MOD)
				DoMod(ctx, (AeonPrimitiveType)inst.arg0);
			vm_end

				vm_start(OP_ADD)
				DoAdd(ctx, (AeonPrimitiveType)inst.arg0);
			vm_end

				vm_start(OP_SUB)
				DoSub(ctx, (AeonPrimitiveType)inst.arg0);
			vm_end

				vm_start(OP_CONV)
				DoConversion(ctx, (AeonPrimitiveType)inst.arg0, (AeonPrimitiveType)inst.arg1);
			vm_end

				vm_start(OP_PUSHTHIS)
				RzStackValue v = ctx.getThisPtr();
			ctx.push_value(v);
			vm_end

				vm_start(OP_SIZEOF)

				vm_end

				vm_start(OP_TYPEINFO)
				int index = inst.arg0;
			ctx.push_value(RzStackValue::make_ptr(ctx.engine->typedb[index]));
			printf("LOADED TYPE INFO %s\n", ctx.engine->typedb[index]->getName().c_str());
			vm_end

				vm_start(OP_DELETEOBJECT)

				vm_end

				vm_start(OP_THREAD_RUN)

				vm_end

				vm_start(OP_MOV)
				ctx.esp += inst.arg1;
			vm_end

				vm_start(OP_BREAKPOINT)
				__asm int 3;
			vm_end

				vm_start(OP_DTEST)
				printf("TEST VALUE %d\n", inst.arg0);
			vm_end

				vm_start(OP_DEBUG)
				vm_log("DEBUG", 7, ctx.engine->string_literals[inst.arg1].c_str());
			vm_end

				vm_start(OP_PUSHVAR)
#if defined TRACE_VM
				printf("Pushed var to stack from offset %d\n", inst.arg0);
#endif
			RzValue* referredValue = reinterpret_cast<RzValue*>(ctx.ebp - inst.arg0 - sizeof(RzValue));
			ctx.pushVariant(*referredValue);

			vm_end

				vm_start(OP_POPVAR)
				RzValue v;
			ctx.popVariant(v);
			vm_end

				vm_start(OP_VARCALL)
				RzValue ptr;
			ctx.popVariant(ptr);
			std::string methodName = ctx.cl->module->m_identifierPool[inst.arg0];
			RzValue fnValue = ptr.property(methodName);

#if defined TRACE_VM
			printf("Calling late bound method\n");
#endif
			if (!fnValue.isUndefined())
			{
				fnValue.call();
				printf("CALLING VAR METHOD %s\n", methodName.c_str());
			}
			else
			{
				printf("No such property\n");
			}
			vm_end

				vm_start(OP_VARSTORE)
				DoVarStore(ctx, inst.arg0, inst.arg1, inst.arg2);
			vm_end

				vm_start(OP_VARLOAD)
				DoVarLoad(ctx, inst.arg0);
			vm_end

				vm_start(OP_VARLOADREF)
				DoVarLoadRef(ctx, inst.arg0, inst.arg1);
			vm_end

		} // end of switch
	}
}

