#ifndef CODEGEN_VM_HPP_
#define CODEGEN_VM_HPP_

#include <razer/ir/ir.hpp>

class CodeGenVM
{
public:
    void build(IRModule& module);
    void build(IRFunction& func);
};

#endif // CODEGEN_VM_HPP_
