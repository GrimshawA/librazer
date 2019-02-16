#ifndef RAZERLANGUAGE_BIND_UTILS_H
#define RAZERLANGUAGE_BIND_UTILS_H

#include <razer/vm/VirtualMachine.h>

namespace Private {
	template<int...Ints>
	struct IntegerPackRange {};

	template<int Begin, int Count, int...Tail>
	struct MakeIntRangeType {
		typedef typename MakeIntRangeType<Begin,Count-1,Begin+Count-1,Tail...>::type type;
	};

	template<int Begin, int...Tail>
	struct MakeIntRangeType<Begin,0,Tail...> {
		typedef IntegerPackRange<Tail...> type;
	};

	template<int Begin, int Count>
	inline typename MakeIntRangeType<Begin,Count>::type MakeIntRange() {
		return typename MakeIntRangeType<Begin,Count>::type();
	}

	template<typename Arg0, typename ... Args>
	struct PackExpand
	{
		using First = Arg0;
	};
}

template<typename T>
inline static void SetStackArg(T value);

template<>
inline void SetStackArg(std::string value) {
}

template<typename T>
inline static T GetStackArg(RzGeneric g) {
	return {};
}

template<>
inline std::string GetStackArg(RzGeneric g) {
	return {};
}

template < typename _Member_T >
struct MemberInvoker {};

/* Invoker with return type */
template < typename _R, typename _Class_T, typename ... _Args_TV>
struct MemberInvoker < _R (_Class_T::*)(_Args_TV...) > {

	static const int ArgsCount = sizeof...(_Args_TV);
	using Type_t = _Class_T;

	template<typename Method_t>
	static void Invoke(_Class_T* instance, Method_t method, RzGeneric g)
	{
		InvokeInternal(instance, method, g, Private::MakeIntRange<0,sizeof...(_Args_TV)>());
	}

	template<int ...Indices, typename Method_t>
	static void InvokeInternal(_Class_T* instance, Method_t method, RzGeneric g, Private::IntegerPackRange<Indices...>)
	{
		auto ret = (instance->*method)(/*(GetStackArg < _Args_TV >(callArgs[Indices], cx))...*/);
		//ApplyValueToHandle(callArgs.rval(), cx, ret);
	}
};

/* Invoker for void functions */
template < typename _Class_T, typename ... _Args_TV>
struct MemberInvoker < void (_Class_T::*)(_Args_TV...) > {

	static const int ArgsCount = sizeof...(_Args_TV);
	using Type_t = _Class_T;

	template<typename Method_t>
	static void Invoke(_Class_T* instance, Method_t method, RzGeneric g)
	{
		InvokeInternal(instance, method, g, Private::MakeIntRange<0,sizeof...(_Args_TV)>());
	}

	template<int ...Indices, typename Method_t>
	static void InvokeInternal(_Class_T* instance, Method_t method, RzGeneric g, Private::IntegerPackRange<Indices...>)
	{
		(instance->*method)(/*(GetStackArg < _Args_TV >(callArgs[Indices], cx))...*/);
	}
};


/*
 * Note: Migrate this to auto functor (c++17)
 */
template<typename Method_t, Method_t functor>
class AutoMethodWrapper
{
public:

	using Class_t = typename MemberInvoker<Method_t>::Type_t;
	using MethodSignature_t = Method_t;

	static const int ArgsCount = MemberInvoker<Method_t>::ArgsCount;

	static Method_t GetMethod() {
		return functor;
	}

	static void Invoke(RzGeneric g) {
		Class_t* obj = (Class_t*)g.popObject();
		MemberInvoker<Method_t>::Invoke(obj, functor, g);
	}
};

#endif //RAZERLANGUAGE_BIND_UTILS_H
