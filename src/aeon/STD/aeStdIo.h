#ifndef aeon_stdlib_h__
#define aeon_stdlib_h__

class aeContext;

/// Loads default functionality into the vm
void register_stdlib(aeContext* ctx);

#endif // aeon_stdlib_h__
