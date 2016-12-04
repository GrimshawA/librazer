#include "VariantOps.h"

void AEVirtualMachine::execute(AEVmStack& threadInfo)
{
	threadInfo.cl = &m_stk.frames[m_stk.frames.size() - 1];

	for (; threadInfo.cl->pc < threadInfo.cl->module->m_code.size(); ++threadInfo.cl->pc)
	{
		AEInstruction& inst = threadInfo.cl->module->m_code[threadInfo.cl->pc];
#ifdef TRACE_VM
		printf(" vm %s\n", inst_opcode_str(inst).c_str());
#endif

		switch (inst.opcode)
		{
			vm_start(OP_NEW)
				DoNewObject(this, inst.arg0, inst.arg1);
			vm_end

			vm_start(OP_PREPARE)
				AEFunction* functionData = m_ctx->m_functionTable[inst.arg0];
			m_stk.esp -= functionData->returnValueSize;
			vm_end

				vm_start(OP_CALL)
				DoCall(this, inst.arg0);
			vm_end

				vm_start(OP_DCALL)
				DoDynamicCall(this, inst.arg0);
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
				AEGeneric g; g.m_vm = this;
			m_ctx->m_functionTable[inst.arg0]->fn(g);
			vm_end

				vm_start(OP_RETURN)
			if (DoReturn(this))
				return;
			vm_end

				vm_start(OP_JZ)
				DoJumpIfZero(this, inst.arg0);
			vm_end

				vm_start(OP_JMP)
				DoJump(this, inst.arg0);
			vm_end

				vm_start(OP_LOAD)
				DoLoad(this, inst.arg0, inst.arg1, inst.arg2);
			vm_end

				vm_start(OP_LOADADDR)
				DoLoadAddr(this, inst.arg0, inst.arg1, inst.arg2);
			vm_end

				vm_start(OP_LOADK)
				DoLoadConstant(this, inst.arg0, inst.arg1, inst.arg2);
			vm_end

				vm_start(OP_LOADENUM)
				vm_value v;
			v.i32 = inst.arg0;
			m_stk.push_value(v);
			vm_end

				vm_start(OP_LT)
				DoLessThan(this, (AeonPrimitiveType)inst.arg0);
			vm_end

				vm_start(OP_LTE)

				vm_end

				vm_start(OP_GT)
				DoGreaterThan(this, (AeonPrimitiveType)inst.arg0);
			vm_end

				vm_start(OP_GTE)

				vm_end

				vm_start(OP_EQ)
				DoEquals(this, (AeonPrimitiveType)inst.arg0);
			vm_end

				vm_start(OP_NEQ)
				DoNotEquals(this, (AeonPrimitiveType)inst.arg0);
			vm_end

				vm_start(OP_NOT)
				DoLogicalNot(this);
			vm_end

				vm_start(OP_SET)
				DoAssign(this, inst.arg0, inst.arg1, inst.arg2);
			vm_end

				vm_start(OP_MUL)
				DoMul(this, (AeonPrimitiveType)inst.arg0);
			vm_end

				vm_start(OP_DIV)
				DoDiv(this, (AeonPrimitiveType)inst.arg0);
			vm_end

				vm_start(OP_MOD)
				DoMod(this, (AeonPrimitiveType)inst.arg0);
			vm_end

				vm_start(OP_ADD)
				DoAdd(this, (AeonPrimitiveType)inst.arg0);
			vm_end

				vm_start(OP_SUB)
				DoSub(this, (AeonPrimitiveType)inst.arg0);
			vm_end

				vm_start(OP_CONV)
				DoConversion(this, (AeonPrimitiveType)inst.arg0, (AeonPrimitiveType)inst.arg1);
			vm_end

				vm_start(OP_PUSHTHIS)
				vm_value v = m_stk.getThisPtr();
			m_stk.push_value(v);
			vm_end

				vm_start(OP_SIZEOF)

				vm_end

				vm_start(OP_TYPEINFO)
				int index = inst.arg0;
			m_stk.push_value(vm_value::make_ptr(m_ctx->typedb[index]));
			printf("LOADED TYPE INFO %s\n", m_ctx->typedb[index]->getName().c_str());
			vm_end

				vm_start(OP_DELETEOBJECT)

				vm_end

				vm_start(OP_THREAD_RUN)

				vm_end

				vm_start(OP_MOV)
				m_stk.esp += inst.arg1;
			vm_end

				vm_start(OP_BREAKPOINT)
				__asm int 3;
			vm_end

				vm_start(OP_DTEST)
				printf("TEST VALUE %d\n", inst.arg0);
			vm_end

				vm_start(OP_DEBUG)
				vm_log("DEBUG", 7, m_ctx->string_literals[inst.arg1].c_str());
			vm_end

				vm_start(OP_PUSHVAR)
#if defined TRACE_VM
			printf("Pushed var to stack from offset %d\n", inst.arg0);
#endif
			RzValue* referredValue = reinterpret_cast<RzValue*>(m_stk.ebp - inst.arg0 - sizeof(RzValue));
			m_stk.pushVariant(*referredValue);

			vm_end

				vm_start(OP_POPVAR)
				RzValue v;
				m_stk.popVariant(v);
				vm_end

				vm_start(OP_VARCALL)
				RzValue ptr;
				m_stk.popVariant(ptr);
			std::string methodName = m_stk.cl->module->m_identifierPool[inst.arg0];
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
				DoVarStore(this, inst.arg0, inst.arg1, inst.arg2);					
			vm_end

				vm_start(OP_VARLOAD)
				DoVarLoad(this, inst.arg0);												
			vm_end

				vm_start(OP_VARLOADREF)
				DoVarLoadRef(this, inst.arg0, inst.arg1);
			vm_end

		} // end of switch
	}
}