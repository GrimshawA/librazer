#ifndef VM_API_HPP_
#define VM_API_HPP_

#include <razer/config.h>

class RzThreadContext;
class RzFunction;

/// Invoke a function in the context cx from the host application
RZ_API void RzCall(RzThreadContext* cx, RzFunction* fun);

#endif // VM_API_HPP_
