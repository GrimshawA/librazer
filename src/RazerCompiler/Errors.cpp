#include <RazerCompiler/Errors.h>
#include <RazerCompiler/RzCompiler.h>
#include <razer/utils/Logger.h>

void rzcerr_unknown_type(const std::string& type)
{
    //CompilerError("0003", "Declared local variable with unknown type.");
    RZLOG("error: No such type %s\n", type.c_str());
}
