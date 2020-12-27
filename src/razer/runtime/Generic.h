#ifndef RZGENERIC_H__
#define RZGENERIC_H__

#include <razer/config.h>
#include <string>
#include <stdint.h>

class RzVirtualMachine;
class RzThreadContext;

/**
    \class RzGeneric
    \brief Interface for interacting with the vm stack

    This interface is intended to serve the purpose of the generic calling convention of the language.

	To avoid direct interaction with the low level vm, it is allowed
	to use AEGeneric as the packer and unpacker of arguments and
	return types.

	It makes it easy to write bindings through this interface,
	making it possible to ignore implementation details while using 
	the language.
*/
class RzGeneric
{
public:

    /// [Argument extraction from Vm Stack]
    int32_t popInt32();
    bool popBoolean();
    float popFloat();
    void* popObject();
	std::string unpack_string();

	/// [Argument packing: host -> script]
    void pushInt32(int32_t value);
    void pushBoolean(bool value);
    void pushFloat(float value);
    void pushObject(void* obj);
	void pack_string(const std::string& value);

private:
    // Private until these primitives are supported
    void pack_uint32(uint32_t value);
    void pack_double(double value);
    double unpack_double();
    uint32_t unpack_uint32();

public:
	RzThreadContext* m_threadCtx;
	bool m_variantCall = false;
};

typedef void(*aeBindMethod)(RzGeneric);

#endif // RZGENERIC_H__
