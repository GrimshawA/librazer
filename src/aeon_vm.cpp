#include "aeon_vm.h"
#include "aeon_bytecode.h"
#include "aeon_context.h"

#define vm_start(x) case x:{
#define vm_end break;}

aeon_vm::aeon_vm()
{
	stack.resize(512000);
	sp = &stack[512000 - 1];
}

aeon_module* aeon_vm::get_current_mod()
{
	return callstack[callstack.size() - 1].module;
}

void aeon_vm::setContext(aeon_context* context)
{
	ctx = context;
}

void aeon_vm::setArgFloat(uint32_t index, float v)
{
	*(sp - sizeof(aeon_variant)* index) = v;
}

float aeon_vm::getArgFloat(uint32_t index)
{
	return *(sp - index * sizeof(aeon_variant));
}

void aeon_vm::setArg(uint32_t index, aeon_variant v)
{
	uint64_t* argAddress = reinterpret_cast<uint64_t*>((sp - sizeof(uint64_t)* index));
	*argAddress = v._u64;
}

aeon_variant aeon_vm::getArg(uint32_t index)
{
	uint64_t* argAddress = reinterpret_cast<uint64_t*>((sp - sizeof(uint64_t)* index));
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
	sp -= 8;
	memcpy(sp, &n, sizeof(n));
	//Log("Pushed to stack: %d", n);
}

void aeon_vm::push_float(float value)
{
	sp -= sizeof(value);
	memcpy(sp, &value, sizeof(value));
}

void aeon_vm::push_double(double value)
{
	sp -= sizeof(value);
	memcpy(sp, &value, sizeof(value));
}

void aeon_vm::push_int32(int32_t value)
{
	sp -= sizeof(value);
	memcpy(sp, &value, sizeof(value));
}

void aeon_vm::push_bytes(uint32_t bytes)
{
	sp -= bytes;
}

void aeon_vm::push_objectref(atom_objectref ref)
{
	sp -= sizeof(ref);
	memcpy(sp, &ref, sizeof(ref));
}

uint64_t aeon_vm::pop()
{
	uint64_t stacktop;
	memcpy(&stacktop, sp, sizeof(uint64_t));
	sp += 8;
	return stacktop;
}

float aeon_vm::pop_float()
{
	float value;
	memcpy(&value, sp, sizeof(float));
	sp += sizeof(float);
	return value;
}

double aeon_vm::pop_double()
{
	double value;
	memcpy(&value, sp, sizeof(double));
	sp += sizeof(double);
	return value;
}

int32_t aeon_vm::pop_int32()
{
	int32_t value;
	memcpy(&value, sp, sizeof(int32_t));
	sp += sizeof(int32_t);
	return value;
}

atom_objectref aeon_vm::pop_objectref()
{
	atom_objectref value;
	memcpy(&value, sp, sizeof(atom_objectref));
	sp += sizeof(atom_objectref);
	return value;
}

void aeon_vm::call(aeon_module& module, const char* func)
{
	StackFrameInfo callinfo;
	callinfo.name = func;
	callinfo.object = nullptr;

	aeon_function* function = nullptr;
	for (auto& f : module.functions)
	{
		if (f.prototype == std::string(func))
			function = &f;
	}

	if (function)
	{
			callinfo.pc = function->offset;
			callinfo.module = &module;
			callstack.push_back(callinfo);
			//Log("Running %s", function->prototype.c_str());

			//Log("running: Stack at %d", getStackIndex());
			execute();
			//Log("running: Ending at %d", getStackIndex());
	}
	else
	{
		printf("Couldn't find the calling function");
	}
}

int aeon_vm::getStackIndex()
{
	uint64_t ptr1 = reinterpret_cast<uint64_t>(&stack[stack.size() - 1]);
	uint64_t ptr2 = reinterpret_cast<uint64_t>(sp);
	return ptr1 - ptr2;
}

void aeon_vm::execute()
{
	StackFrameInfo* cl = &callstack[callstack.size() - 1];

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
				callstack.push_back(callinfo);
				cl = &callstack[callstack.size() - 1];
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
				atom_objectref ref = pop_objectref();     // The object to call the method on
				int32_t        moduleIndex = pop_int32(); // The module the type belongs to
				int32_t        methodIndex = pop_int32(); // The method we want to call

				aeon_method&   methodInfo = ctx->modules[moduleIndex]->methods[methodIndex];
				
				StackFrameInfo callinfo;
				callinfo.name = methodInfo.name;
				callinfo.pc = methodInfo.offset;
				callinfo.module = ctx->modules[moduleIndex].get();
				callstack.push_back(callinfo);
				cl = &callstack[callstack.size() - 1];
				cl->pc--;
			vm_end

			case EOpCodes::call_method_n:
			{
				//Log("Unimplemented");
				break;
			};
			case EOpCodes::ret:
			{
								  callstack.pop_back();
								  if (callstack.size() == 0)
								  {
									  return;
								  }
								  else
								  {
									  // restore underlying function
									  cl = &callstack[callstack.size() - 1];
									  //cl->pc++;
								  }
								  break;
			}

			case EOpCodes::jz:
			{
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

			}break;

			case EOpCodes::jmp:
			{
								  int newpc = getinst_a(inst);
								  cl->pc = newpc;
								  break;
			}

			case EOpCodes::PushArg:
			{
									  uint32_t argIndex = getinst_a(inst);
									  uint32_t argType = getinst_b(inst);
									 // uint64_t argValue = cl->module->instructions[++cl->pc];
									  uint64_t argValue = 0;
									  aeon_variant  argVariant; argVariant._u64 = argValue; argVariant.type = argType;

									  setArg(argIndex, argVariant);

									  //Log("PushArg  arg:%u, %d", argIndex, argValue);
									  //vmvalue_print(argVariant);
									  break;
			}
			case EOpCodes::loadk:
			{
									int constant = getinst_a(inst);
									push(static_cast<uint64_t>(constant));
									//Log("[loadk] %d constant", constant);
			} break;

			case EOpCodes::lt:
			{
								 int opb = pop();
								 int opa = pop();
								 push(static_cast<int>(opa < opb));
								 //Log("[lt] %d < %d = %d", opa, opb, static_cast<int>(opa < opb));
								 break;
			}
			case EOpCodes::gt:
			{
								 int opb = pop();
								 int opa = pop();
								 push(static_cast<int>(opa > opb));
								// Log("[gt] %d > %d = %d", opa, opb, static_cast<int>(opa < opb));
								 break;
			}

			case EOpCodes::set:
			{
								  uint64_t address = pop();
								  uint64_t newv = pop();

								  aeon_variant v; v._u64 = address;
								  uint64_t* realAddr = reinterpret_cast<uint64_t*>(v._obj);

								  // Log("[set] Address %x took the new value %d", v._obj, newv);

								  *realAddr = newv;
			}break;

			case EOpCodes::mul:
			{
								  int opa = pop();
								  int opb = pop();
								  push(opa * opb);
								  //  Log("[mul] added two values %d and %d. result %d", opa, opb, opa * opb);
			}break;

			case EOpCodes::OP_DIV:
			{
								  int opb = pop();
								  int opa = pop();
								  int r = float(opa) / float(opb);
								  push(r);
								  // Log("[div] %d / %d yields %d", opa, opb, r);
			}break;

			case EOpCodes::mod:
			{
								  int opb = pop();
								  int opa = pop();

								  push(opa % opb);
								  // Log("[mod] %d % %d", opa, opb);
			}break;
			case EOpCodes::add:
			{
								  uint64_t opa = pop();
								  uint64_t opb = pop();
								  push(opa + opb);
								  // Log("[add] two values %d and %d = %d", opa, opb, opa + opb);
			}break;

			case EOpCodes::sub:
			{
								  int opa = pop();
								  int opb = pop();
								  push(opa - opb);
								  // Log("[sub] added two values %d and %d", opa, opb);
								  break;
			}

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
										callstack.push_back(callinfo);

										cl = &callstack[callstack.size() - 1];
										cl->pc--;
										break;
			}	

			case EOpCodes::iload:
			{
									int index = getinst_a(inst);
									push(*reinterpret_cast<uint64_t*>(localsStack[index]));
									//Log("[iload] Loaded int local");
			}break;

			case EOpCodes::ilocal:
			{
									 push(0);
									 localsStack.push_back(sp);
									 // Log("ilocal");
			}break;

			case EOpCodes::push_obj:
			{
									   int type_index = getinst_a(inst);
									   push_bytes(ctx->typedb[type_index]->size);
									   localsStack.push_back(sp);
									   break;
			}

			case EOpCodes::pop_obj:
			{
									  int type_index = getinst_a(inst);
									  sp += ctx->typedb[type_index]->size;
									  localsStack.push_back(sp);
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
									   sp += bytes;
									   break;
			}

			case EOpCodes::objsizeof:
			{
										uint32_t type_token = getinst_a(inst);
										push(ctx->typedb[type_token]->size);
										break;
			}

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

			case EOpCodes::newobj:{
									  int32_t objtype = getinst_a(inst);
									  atom_objectref obj = ctx->alloc(objtype);
									  push_objectref(obj);
									  break;
			}

			case EOpCodes::deleteobj:{
										 atom_objectref obj = pop_objectref();
										 ctx->release(obj, 0);
										 break;
			}

			case EOpCodes::isinst:{
									  break;
			}

		} // end of switch
	}
}