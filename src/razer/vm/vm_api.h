#ifndef VM_API_HPP_
#define VM_API_HPP_

class RzThreadContext;
class RzFunction;

/// Invoke a function in the context cx from the host application
void RzCall(RzThreadContext* cx, RzFunction* fun);

#endif // VM_API_HPP_
