#ifndef aeon_stdlib_h__
#define aeon_stdlib_h__

class aeon_context;

/// Loads default functionality into the vm
void register_stdlib(aeon_context* ctx);

#endif // aeon_stdlib_h__
