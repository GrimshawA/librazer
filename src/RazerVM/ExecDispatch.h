#ifndef RZEXECDISPATCH_H__
#define RZEXECDISPATCH_H__

class RzThreadContext;

/// The main dispatch loop for executing byte code
void dispatch_execute(RzThreadContext& ctx);

#endif