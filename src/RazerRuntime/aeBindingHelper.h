#ifndef aeBindingHelper_h__
#define aeBindingHelper_h__

#include <aeon/aeVM.h>

template<typename T>
class aeBindingHelper
{
public:
	static void DefaultConstructor(RzVirtualMachine* vm)
	{
		void* memory = vm->m_mainContext.pop_value().ptr;
		new (memory) T();
	}

	static void Destructor(RzVirtualMachine* vm)
	{
		void* memory = vm->m_mainContext.pop_value().ptr;
		static_cast<T*>(memory)->~T();
	}
};

#define AE_WRAP_CONSTRUCT(TYPE) &aeBindingHelper<TYPE>::DefaultConstructor
#define AE_WRAP_DESTRUCT(TYPE) &aeBindingHelper<TYPE>::Destructor

#endif // aeBindingHelper_h__
