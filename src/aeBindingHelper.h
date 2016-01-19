#ifndef aeBindingHelper_h__
#define aeBindingHelper_h__

#include "aeon_vm.h"
/*
template<class T> void luaU_push(aeon_vm*, T)
{

}
template<class T> T* luaW_check(aeon_vm*, int)
{
	return nullptr;
}
template<class T> T luaU_check(aeon_vm*, int)
{
	return 1;
}

// metaprogramming for creating indices ...

template<int...Ints>
struct int_pack {};

template<int Begin, int Count, int...Tail>
struct make_int_range_type {
	typedef typename make_int_range_type<Begin, Count - 1, Begin + Count - 1, Tail...>::type type;
};

template<int Begin, int...Tail>
struct make_int_range_type<Begin, 0, Tail...> {
	typedef int_pack<Tail...> type;
};

template<int Begin, int Count>
inline typename make_int_range_type<Begin, Count>::type
make_int_range()
{
	return typename make_int_range_type<Begin, Count>::type();
}


// the actual wrapper ...

template<class MemFunPtrType, MemFunPtrType PMF>
struct aeMethodBindingHelper;

template<class Clazz, class ReturnType, class...Args, ReturnType(Clazz::*PMF)(Args...)>
struct aeMethodBindingHelper<ReturnType(Clazz::*)(Args...), PMF> {
	static void doit(aeon_vm* L) {
		//doit_impl(L, make_int_range<2, sizeof...(Args)>());
	}
private:
	template<int...Indices>
	static int doit_impl(aeon_vm* L, int_pack<Indices...>) {
		luaU_push<ReturnType>(L,
			(luaW_check<Clazz>(L, 1)->*PMF)(
			luaU_check<Args>(L, Indices)...
			)
			);
		return 1;
	}
};

#define METHOD_WRAPPER(...) &aeMethodBindingHelper<decltype(__VA_ARGS__),__VA_ARGS__>::doit
*/

// Lua API dummies ...

struct lua_State {};

template<class T> void luaU_push(lua_State*, T);
template<class T> T* luaW_check(lua_State*, int);
template<class T> T luaU_check(lua_State*, int);


// metaprogramming for creating indices ...

template<int...Ints>
struct int_pack {};

template<int Begin, int Count, int...Tail>
struct make_int_range_type {
	typedef typename make_int_range_type<Begin, Count - 1, Begin + Count - 1, Tail...>::type type;
};

template<int Begin, int...Tail>
struct make_int_range_type<Begin, 0, Tail...> {
	typedef int_pack<Tail...> type;
};

template<int Begin, int Count>
inline typename make_int_range_type<Begin, Count>::type
make_int_range()
{
	return typename make_int_range_type<Begin, Count>::type();
}


// the actual wrapper ...

template<class MemFunPtrType, MemFunPtrType PMF>
struct lua_mem_func_wrapper;

template<class Clazz, class ReturnType, class...Args, ReturnType(Clazz::*PMF)(Args...)>
struct lua_mem_func_wrapper<ReturnType(Clazz::*)(Args...), PMF> {
	static int doit(lua_State* L) {
		return doit_impl(L, make_int_range<2, sizeof...(Args)>());
	}
private:
	template<int...Indices>
	static int doit_impl(lua_State* L, int_pack<Indices...>) {
		luaU_push<ReturnType>(L,
			(luaW_check<Clazz>(L, 1)->*PMF)(
			luaU_check<Args>(L, Indices)...
			)
			);
		return 1;
	}
};

#define METHOD_WRAPPER(...) &lua_mem_func_wrapper<decltype(__VA_ARGS__),__VA_ARGS__>::doit


#endif // aeBindingHelper_h__
