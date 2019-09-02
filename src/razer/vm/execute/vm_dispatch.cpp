#include <razer/vm/VirtualMachine.h>
#include <razer/vm/ThreadContext.h>
#include <razer/vm/ThreadHandler.h>

#include <razer/vm/execute/vm_calls.cpp>
#include <razer/vm/execute/vm_dynamic.cpp>
#include <razer/vm/execute/vm_general.cpp>
#include <razer/vm/execute/vm_debug.cpp>

#define vm_start(x) case x:{
#define vm_end break;}

#ifdef ENABLE_VM_LOGS
#define RZLOGVM(x, a, b, c) RZLOG("[vm] %s %d %d %d\n", #x, a, b, c)
#else
#define RZLOGVM(x, a, b, c)
#endif


#define VMLOGLEVEL 5
#define vm_log(tag, level, STR) if(level > VMLOGLEVEL) RZLOG("%s: %s\n", tag, STR);


void dispatch_execute(RzThreadContext& ctx)
{
    ctx.cl = &ctx.frames[ctx.frames.size() - 1];

    for (; ctx.cl->pc < ctx.cl->module->m_code.size(); ++ctx.cl->pc)
    {
        RzInstruction& inst = ctx.cl->module->m_code[ctx.cl->pc];

        RZLOGVM(" vm %s\n", inst_opcode_str(inst).c_str(), 0, 0);

        switch (inst.opcode)
        {
        vm_start(OP_NEW)
                DoNewObject(ctx, inst.a.arg0, inst.a.arg1);
        vm_end

                vm_start(OP_CALL)
                DoCall(ctx, inst.a.arg0, inst.a.arg1, inst.a.arg2);
        vm_end

                vm_start(OP_DCALL)
                DoDynamicCall(ctx, inst.a.arg0);
        vm_end

                vm_start(OP_CALLVIRTUAL)
                // todo
                vm_end

        vm_start(OP_NATIVECALL)
            DoNativeCall(ctx, inst.a.arg0, inst.a.arg1, inst.a.arg2);
        vm_end



                vm_start(OP_RETURN)
                if (DoReturn(ctx))
                return;
        vm_end

                vm_start(OP_JZ)
                DoJumpIfZero(ctx, inst.a.arg0);
        vm_end

                vm_start(OP_JMP)
                DoJump(ctx, inst.a.arg0);
        vm_end

                vm_start(OP_LOAD)
                DoLoad(ctx, inst.a.arg0, inst.a.arg1, inst.a.arg2);
        vm_end

                vm_start(OP_LOADADDR)
                DoLoadAddr(ctx, inst.a.arg0, inst.a.arg1, inst.a.arg2);
        vm_end

        vm_start(OP_DEREF)
            DoDeref(ctx, inst.a.arg0, inst.a.arg1, inst.a.arg2);
        vm_end

                vm_start(OP_LOADK)
                DoLoadConstant(ctx, inst.a.arg0, inst.a.arg1, inst.a.arg2);
        vm_end

        vm_start(OP_ALLOC)
                DoAlloc(ctx, inst.b.arg0);
        vm_end

        vm_start(OP_DUP)
                DoDup(ctx);
        vm_end


                vm_start(OP_LOADENUM)
                RzStackValue v;
        v.i32 = inst.a.arg0;
        ctx.push_value(v);
        vm_end

                vm_start(OP_LT)
                DoLessThan(ctx, (AeonPrimitiveType)inst.a.arg0);
        vm_end

                vm_start(OP_LTE)

                vm_end

                vm_start(OP_GT)
                DoGreaterThan(ctx, (AeonPrimitiveType)inst.a.arg0);
        vm_end

                vm_start(OP_GTE)

                vm_end

                vm_start(OP_EQ)
                DoEquals(ctx, (AeonPrimitiveType)inst.a.arg0);
        vm_end

                vm_start(OP_NEQ)
                DoNotEquals(ctx, (AeonPrimitiveType)inst.a.arg0);
        vm_end

                vm_start(OP_NOT)
                DoLogicalNot(ctx);
        vm_end

        vm_start(OP_SET)
            DoAssign(ctx, inst.a.arg0, inst.a.arg1, inst.a.arg2);
        vm_end

                vm_start(OP_MUL)
                DoMul(ctx, (AeonPrimitiveType)inst.a.arg0);
        vm_end

                vm_start(OP_DIV)
                DoDiv(ctx, (AeonPrimitiveType)inst.a.arg0);
        vm_end

                vm_start(OP_MOD)
                DoMod(ctx, (AeonPrimitiveType)inst.a.arg0);
        vm_end

                vm_start(OP_ADD)
                DoAdd(ctx, (AeonPrimitiveType)inst.a.arg0);
        vm_end

                vm_start(OP_SUB)
                DoSub(ctx, (AeonPrimitiveType)inst.a.arg0);
        vm_end

                vm_start(OP_UNARYSUB)
                    DoUnarySub(ctx, (AeonPrimitiveType)inst.a.arg0);
                vm_end

			vm_start(OP_INCREMENT)
                DoIncrement(ctx, (AeonPrimitiveType)inst.a.arg0, inst.a.arg1);
			vm_end

                vm_start(OP_AND)
                    DoAnd(ctx, (AeonPrimitiveType)inst.a.arg0);
                vm_end

                        vm_start(OP_OR)
                            DoOr(ctx, (AeonPrimitiveType)inst.a.arg0);
                        vm_end

                vm_start(OP_CONV)
                DoConversion(ctx, (AeonPrimitiveType)inst.a.arg0, (AeonPrimitiveType)inst.a.arg1);
        vm_end

                vm_start(OP_PUSHTHIS)
                //RzStackValue v = ctx.getThisPtr();
                //ctx.push_value(v);
                vm_end

                vm_start(OP_SIZEOF)

                vm_end

                vm_start(OP_TYPEINFO)
                int index = inst.a.arg0;
        ctx.push_value(RzStackValue::make_ptr(ctx.engine->typedb[index]));
        RZLOG("LOADED TYPE INFO %s\n", ctx.engine->typedb[index]->getName().c_str());
        vm_end

                vm_start(OP_DELETEOBJECT)

                vm_end

                vm_start(OP_THREAD_RUN)

                vm_end

                vm_start(OP_MOV)
                ctx.esp += inst.a.arg1;
                //memset(ctx.esp, 1, abs(inst.a.arg1));
                RZLOG("ESP AT %x AFTER MOV\n", ctx.esp);
        vm_end

        vm_start(OP_BREAKPOINT)
              if (ctx.engine->_dbg )
              {
              	  ctx.engine->_dbg->ctx = &ctx;
                  ctx.engine->_dbg->onBreakpoint();
              }
        vm_end

                vm_start(OP_DTEST)
                RZLOG("TEST VALUE %d\n", inst.a.arg0);
        vm_end

                vm_start(OP_DEBUG)
                DoDebugOp(ctx, inst.a.arg0, inst.a.arg1);
        vm_end

                vm_start(OP_PUSHVAR)
        #if defined TRACE_VM
                RZLOG("Pushed var to stack from offset %d\n", inst.a.arg0);
#endif
        RzValue* referredValue = reinterpret_cast<RzValue*>(ctx.ebp - inst.a.arg0 - sizeof(RzValue));
        ctx.pushVariant(*referredValue);

        vm_end

                vm_start(OP_POPVAR)
                RzValue v;
        ctx.popVariant(v);
        vm_end

                vm_start(OP_VARCALL)
                ExecVariantCall(ctx, inst.a.arg0);
        vm_end

                vm_start(OP_VARSTORE)
                DoVarStore(ctx, inst.a.arg0, inst.a.arg1, inst.a.arg2);
        vm_end

                vm_start(OP_VARLOAD)
                DoVarLoad(ctx, inst.a.arg0);
        vm_end

                vm_start(OP_VARLOADREF)
                DoVarLoadRef(ctx, inst.a.arg0, inst.a.arg1);
        vm_end


        	default:
        		assert(false);

        } // end of switch
    }
}

