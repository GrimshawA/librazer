#ifndef AEGeneric_h__
#define AEGeneric_h__

#include <string>
//#include <Rzr/RzValue.h>
#include <stdint.h>

class AEVirtualMachine;

/**
	\class AEGeneric
	\brief Bridges the host application with the vm

	To avoid direct interaction with the low level vm, it is allowed
	to use AEGeneric as the packer and unpacker of arguments and
	return types.

	It makes it easy to write bindings through this interface,
	making it possible to ignore implementation details while using 
	the language.
*/
class AEGeneric
{
public:

	/// [Argument unpacking: script -> host]
	int32_t unpack_int32();
	uint32_t unpack_uint32();
	float unpack_float();
	double unpack_double();
	std::string unpack_string();
	//AEValue unpackVariant();

	/// [Argument packing: host -> script]
	void pack_int32(int32_t value);
	void pack_uint32(uint32_t value);
	void pack_float(float value);
	void pack_double(double value);
	void pack_string(const std::string& value);

//private:
	AEVirtualMachine* m_vm;
};

typedef void(*aeBindMethod)(AEGeneric);

#endif // AEGeneric_h__
