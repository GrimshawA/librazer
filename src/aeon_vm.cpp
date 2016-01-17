#include "aeon_vm.h"
#include "aeon_bytecode.h"
#include "aeon_context.h"
#include "aeon_object.h"

#define vm_start(x) case x:{
#define vm_end break;}

#define LOGVM(x, a, b, c) printf("[vm] %s %d %d %d\n", #x, a, b, c);


aeon_vm::aeon_vm()
{
	
}

aeon_module* aeon_vm::get_current_mod()
{
	return m_stk.frames[m_stk.frames.size() - 1].module;
}

void aeon_vm::callMethod(aeon_object* object, const std::string& prototype)
{
	m_stk.ebp = m_stk.esp;

	vm_value thisPtrValue;
	thisPtrValue.ptr = object->addr;
	m_stk.push_value(thisPtrValue); // push 'this' ptr
	//void* xdr = m_stk.pop_value().ptr;
	  
	printf("callMethod: pushing this, pointing to %x popped\n", object->addr);

	call(*object->getType()->getModule(), prototype.c_str());
} 

void aeon_vm::callMethod(aeon_object* object, uint32_t methodId)
{

}

void aeon_vm::prepare(aeFunctionId function)
{

}

void aeon_vm::pushThis(void* obj)
{
	m_stk.push_addr(obj);
}

void aeon_vm::setContext(aeon_context* context)
{
	ctx = context;
} 

void aeon_vm::setArgFloat(uint32_t index, float v)
{
	*(m_stk.esp - sizeof(aeon_variant)* index) = v;
}

float aeon_vm::getArgFloat(uint32_t index)
{
	return *(m_stk.esp - index * sizeof(aeon_variant));
}

void aeon_vm::setArg(uint32_t index, aeon_variant v)
{
	uint64_t* argAddress = reinterpret_cast<uint64_t*>((m_stk.esp - sizeof(uint64_t)* index));
	*argAddress = v._u64;
}

aeon_variant aeon_vm::getArg(uint32_t index)
{
	uint64_t* argAddress = reinterpret_cast<uint64_t*>((m_stk.esp - sizeof(uint64_t)* index));
	aeon_variant v;
	v._u64 = *argAddress;
	return v;
}

void printBits2(size_t const size, void const * const ptr)
{
		unsigned char *b = (unsigned char*)ptr;
		unsigned char byte;
		int i, j;

		for (i = size - 1; i >= 0; i--)
		{
			for (j = 7; j >= 0; j--)
			{
				byte = b[i] & (1 << j);
				byte >>= j;
				printf("%u", byte);
			}
		}
		puts("");
}

void aeon_vm::push(uint64_t n)
{
	m_stk.esp -= 8;
	memcpy(m_stk.esp, &n, sizeof(n));
	//Log("Pushed to stack: %d", n);
}

void aeon_vm::push_float(float value)
{
	m_stk.esp -= sizeof(value);
	memcpy(m_stk.esp, &value, sizeof(value));
}

void aeon_vm::push_double(double value)
{
	m_stk.esp -= sizeof(value);
	memcpy(m_stk.esp, &value, sizeof(value));
}

void aeon_vm::push_int32(int32_t value)
{
	m_stk.esp -= sizeof(value);
	memcpy(m_stk.esp, &value, sizeof(value));
}

void aeon_vm::push_bytes(uint32_t bytes)
{
	m_stk.esp -= bytes;
}

void aeon_vm::push_objectref(atom_objectref ref)
{
	m_stk.esp -= sizeof(ref);
	memcpy(m_stk.esp, &ref, sizeof(ref));
}

uint64_t aeon_vm::pop()
{
	uint64_t stacktop;
	memcpy(&stacktop, m_stk.esp, sizeof(uint64_t));
	m_stk.esp += 8;
	return stacktop;
}

float aeon_vm::pop_float()
{
	float value;
	memcpy(&value, m_stk.esp, sizeof(float));
	m_stk.esp += sizeof(float);
	return value;
}

double aeon_vm::pop_double()
{
	double value;
	memcpy(&value, m_stk.esp, sizeof(double));
	m_stk.esp += sizeof(double);
	return value;
}

int32_t aeon_vm::pop_int32()
{
	int32_t value;
	memcpy(&value, m_stk.esp, sizeof(int32_t));
	m_stk.esp += sizeof(int32_t);
	return value;
}

void aeon_vm::call(aeon_module& module, const char* func)
{
	StackFrameInfo callinfo;
	callinfo.name = func;
	callinfo.object = nullptr;

	aeFunctionId functionId = 0;

	aeon_function* function = nullptr;
	int i = 0;
	for (auto& f : module.functions)
	{
		if (f.prototype == std::string(func))
		{
			function = &f;
			functionId = i;
		}
		++i;
	}

	prepare(functionId);

	if (function)
	{
		callinfo.pc = function->offset;
		callinfo.module = &module;
		callinfo.ebp = m_stk.esp;
		m_stk.frames.push_back(callinfo);

		execute();
	}
	else
	{
		printf("Couldn't find the calling function\n");
	}
}

int aeon_vm::getStackIndex()
{
	uint64_t ptr1 = reinterpret_cast<uint64_t>(&m_stk.stack[m_stk.stack.size() - 1]);
	uint64_t ptr2 = reinterpret_cast<uint64_t>(m_stk.esp);
	return ptr1 - ptr2;
}

void aeon_vm::execute()
{
	StackFrameInfo* cl = &m_stk.frames[m_stk.frames.size() - 1];

	for (; cl->pc < cl->module->instructions.size(); ++cl->pc)
	{
		aeon_instruction& inst = cl->module->instructions[cl->pc];
		switch (inst.opcode)
		{
			vm_start(OP_CALL)
				uint32_t funcid = getinst_a(inst);
				uint32_t params = getinst_b(inst);
				aeon_function func = cl->module->functions[funcid];

				StackFrameInfo callinfo;
				callinfo.name = func.prototype;
				callinfo.pc = func.offset;
				callinfo.module = cl->module;
				m_stk.frames.push_back(callinfo);
				cl = &m_stk.frames[m_stk.frames.size() - 1];
				cl->pc--;
			vm_end

			vm_start(OP_CALLNATIVE)
				uint32_t funcid = getinst_a(inst);
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

				delete gen;
			vm_end

			vm_start(OP_CALLMETHOD)
				//atom_objectref ref = pop_objectref();     // The object to call the method on
				int32_t        moduleIndex = pop_int32(); // The module the type belongs to
				int32_t        methodIndex = pop_int32(); // The method we want to call

				aeon_method&   methodInfo = ctx->modules[moduleIndex]->methods[methodIndex];
				
				StackFrameInfo callinfo;
				callinfo.name = methodInfo.name;
				callinfo.pc = methodInfo.offset;
				callinfo.ebp = m_stk.ebp;
				callinfo.module = ctx->modules[moduleIndex].get();
				m_stk.frames.push_back(callinfo);
				cl = &m_stk.frames[m_stk.frames.size() - 1];
				cl->pc--;
			vm_end

			vm_start(OP_CALLMETHOD_NAT)
				//Log("Unimplemented");
			vm_end

			vm_start(OP_RETURN)
				m_stk.frames.pop_back();
				if (m_stk.frames.size() == 0)
				{
					return;
				}
				else
				{
					if (m_stk.ebp != m_stk.esp)
						printf("Script function returned without popping all stack memory!");
					else
						printf("All memory is as expected");

					// restore underlying function
					cl = &m_stk.frames[m_stk.frames.size() - 1];
					m_stk.ebp = cl->ebp;
				}
			vm_end

			vm_start(OP_JZ)
				int pcoffset = getinst_a(inst);
				int cond = pop();
				if (cond == 0)
				{
					 cl->pc += pcoffset;
					 //Log("the if condition was false. eax %d", eax);
				}
				else
				{
					 //Log("the if condition was true. eax %d", eax);
				}
				//Log("[jz] test %d %d", cond, pcoffset);
			vm_end

			vm_start(OP_JMP)
				int newpc = getinst_a(inst);
				cl->pc = newpc;
			vm_end

			vm_start(OP_PUSHARG)
				uint32_t argIndex = getinst_a(inst);
				uint32_t argType = getinst_b(inst);
				// uint64_t argValue = cl->module->instructions[++cl->pc];
				uint64_t argValue = 0;
				aeon_variant  argVariant; argVariant._u64 = argValue; argVariant.type = argType;

				setArg(argIndex, argVariant);
			vm_end

			vm_start(OP_LOADK)
				auto n = ctx->int_literals[inst.arg1];
				m_stk.push_int32(n);
				printf("[loadk] %d constant\n", n);
			vm_end

			vm_start(OP_LT)
				int opb = pop();
				int opa = pop();
				push(static_cast<int>(opa < opb));
				//Log("[lt] %d < %d = %d", opa, opb, static_cast<int>(opa < opb));
			vm_end

			vm_start(OP_GT)
				int opb = pop();
				int opa = pop();
				push(static_cast<int>(opa > opb));
				// Log("[gt] %d > %d = %d", opa, opb, static_cast<int>(opa < opb));
			vm_end

			vm_start(OP_SET)
				int32_t operand = m_stk.pop_int32();
				int32_t* addr = (int32_t*)m_stk.pop_addr();

				*addr = operand;
				printf("%x now %d\n", addr, operand);
				LOGVM(OP_SET, 0, 0, 0);
			vm_end

			vm_start(OP_MUL)
				int opa = pop();
				int opb = pop();
				push(opa * opb);
				//  Log("[mul] added two values %d and %d. result %d", opa, opb, opa * opb);
			vm_end

			vm_start(OP_DIV)
				int opb = pop();
				int opa = pop();
				int r = float(opa) / float(opb);
				push(r);
				// Log("[div] %d / %d yields %d", opa, opb, r);
			vm_end

			vm_start(OP_MOD)
				int opb = pop();
				int opa = pop();
				push(opa % opb);
				// Log("[mod] %d % %d", opa, opb);
			vm_end

			vm_start(OP_ADD)
				uint64_t opa = pop();
				uint64_t opb = pop();
				push(opa + opb);
				// Log("[add] two values %d and %d = %d", opa, opb, opa + opb);
			vm_end

			vm_start(OP_SUB)
				int opa = pop();
				int opb = pop();
				push(opa - opb);
				// Log("[sub] added two values %d and %d", opa, opb);
			vm_end

			case EOpCodes::f2i:{
								   float v = pop_float();
								   push_int32(static_cast<int32_t>(v));
								   break;
			}

			case EOpCodes::i2f:{
								   int32_t v = pop_int32();
								   push_float(static_cast<float>(v));
								   break;
			}
			case EOpCodes::callvirt:{
										uint32_t funcid = getinst_a(inst);
										uint32_t params = getinst_b(inst);
										aeon_function func = cl->module->functions[funcid];

										StackFrameInfo callinfo;
										callinfo.name = func.prototype;
										callinfo.pc = func.offset;
										callinfo.module = cl->module;
										m_stk.frames.push_back(callinfo);

										cl = &m_stk.frames[m_stk.frames.size() - 1];
										cl->pc--;
										break;
			}	

			vm_start(OP_LOAD)
				if (inst.arg0 == AEK_THIS)
				{
					vm_value* thisObjAddr = reinterpret_cast<vm_value*>(m_stk.ebp - sizeof(vm_value));

					vm_value varptr;
					varptr.ptr = reinterpret_cast < unsigned char*>(thisObjAddr->ptr) + inst.arg1;
					printf("this points to %x\n", varptr.ptr);
					m_stk.push_value(varptr);
					printf("loading field addr from this->\n");
				}
				else if (inst.arg0 == AEK_EBP)
				{
					m_stk.push_addr(cl->ebp - inst.arg1);
					printf("Loading address %x\n", cl->ebp - inst.arg1);
				}
			vm_end

			case EOpCodes::ilocal:
			{
									 push(0);
									 localsStack.push_back(m_stk.esp);
									 // Log("ilocal");
			}break;

			case EOpCodes::push_obj:
			{
									   int type_index = getinst_a(inst);
									   push_bytes(ctx->typedb[type_index]->getSize());
									   localsStack.push_back(m_stk.esp);
									   break;
			}

			case EOpCodes::pop_obj:
			{
									  int type_index = getinst_a(inst);
									  m_stk.esp += ctx->typedb[type_index]->getSize();
									  localsStack.push_back(m_stk.esp);
									  break;
			}

			case EOpCodes::iloadaddr:
			{
										uint32_t index = getinst_a(inst);
										aeon_variant v; v._obj = localsStack[index];
										push(v._u64);
										//Log("[loadaddr] Loaded address %x", v._obj);
			}break;

			case EOpCodes::poplocal:
			{
									   uint32_t bytes = getinst_a(inst);
									   m_stk.esp += bytes;
									   break;
			}

			vm_start(OP_SIZEOF)
				uint32_t type_token = getinst_a(inst);
				push(ctx->typedb[type_token]->getSize());
			vm_end

			case EOpCodes::invsign:
			{
									  auto v = pop_int32();
									  v *= -1;
									  push_int32(v);
									  break;
			}

			case EOpCodes::push_thisobj:{
											push_objectref(cl->object);
											break;
			}

			vm_start(OP_NEWOBJECT)
				int32_t objtype = getinst_a(inst);
				// atom_objectref obj = ctx->alloc(objtype);
				// push_objectref(obj);
			vm_end

			vm_start(OP_DELETEOBJECT)
				
			vm_end

			case EOpCodes::isinst:{
									  break;
			}

			vm_start(OP_MOV)
				m_stk.esp += inst.arg1;
				printf("Allocated local var on stack %d bytes\n", inst.arg1);
			vm_end

			vm_start(OP_DTEST)
				printf("TEST VALUE %d\n", inst.arg0);
			vm_end

		} // end of switch
	}
}