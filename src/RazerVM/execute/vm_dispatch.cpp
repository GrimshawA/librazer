#include <RazerVM/VirtualMachine.h>
#include <RazerVM/ThreadContext.h>
#include <RazerVM/ThreadHandler.h>

#include <RazerVM/execute/vm_calls.cpp>
#include <RazerVM/execute/vm_dynamic.cpp>
#include <RazerVM/execute/vm_general.cpp>
#include <RazerVM/execute/vm_debug.cpp>

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
                DoNewObject(ctx, inst.arg0, inst.arg1);
        vm_end

                vm_start(OP_CALL)
                DoCall(ctx, inst.arg0, inst.arg1, inst.arg2);
        vm_end

                vm_start(OP_DCALL)
                DoDynamicCall(ctx, inst.arg0);
        vm_end

                vm_start(OP_CALLVIRTUAL)
                // todo
                vm_end

                vm_start(OP_NATIVECALL)
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
                DoNativeCall(ctx, inst.arg0, inst.arg1);
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

                vm_start(OP_UNARYSUB)
                    DoUnarySub(ctx, (AeonPrimitiveType)inst.arg0);
                vm_end

                vm_start(OP_AND)
                    DoAnd(ctx, (AeonPrimitiveType)inst.arg0);
                vm_end

                        vm_start(OP_OR)
                            DoOr(ctx, (AeonPrimitiveType)inst.arg0);
                        vm_end

                vm_start(OP_CONV)
                DoConversion(ctx, (AeonPrimitiveType)inst.arg0, (AeonPrimitiveType)inst.arg1);
        vm_end

                vm_start(OP_PUSHTHIS)
                //RzStackValue v = ctx.getThisPtr();
                //ctx.push_value(v);
                vm_end

                vm_start(OP_SIZEOF)

                vm_end

                vm_start(OP_TYPEINFO)
                int index = inst.arg0;
        ctx.push_value(RzStackValue::make_ptr(ctx.engine->typedb[index]));
        RZLOG("LOADED TYPE INFO %s\n", ctx.engine->typedb[index]->getName().c_str());
        vm_end

                vm_start(OP_DELETEOBJECT)

                vm_end

                vm_start(OP_THREAD_RUN)

                vm_end

                vm_start(OP_MOV)
                ctx.esp += inst.arg1;
        vm_end

        vm_start(OP_BREAKPOINT)
               // __asm int 3;
        vm_end

                vm_start(OP_DTEST)
                RZLOG("TEST VALUE %d\n", inst.arg0);
        vm_end

                vm_start(OP_DEBUG)
                DoDebugOp(ctx, inst.arg0, inst.arg1);
        vm_end

                vm_start(OP_PUSHVAR)
        #if defined TRACE_VM
                RZLOG("Pushed var to stack from offset %d\n", inst.arg0);
#endif
        RzValue* referredValue = reinterpret_cast<RzValue*>(ctx.ebp - inst.arg0 - sizeof(RzValue));
        ctx.pushVariant(*referredValue);

        vm_end

                vm_start(OP_POPVAR)
                RzValue v;
        ctx.popVariant(v);
        vm_end

                vm_start(OP_VARCALL)
                ExecVariantCall(ctx, inst.arg0);
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

