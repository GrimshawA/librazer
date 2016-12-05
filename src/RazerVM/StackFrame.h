#ifndef RZSTACKFRAME_H__
#define RZSTACKFRAME_H__

#include <string>
#include <stdint.h>

class RzModule;
class AEFunction;

class RzStackFrame
{
public:

	std::string getName();

public:

	AEFunction*    function;      ///< The function this frame represents
	std::string    name;          ///< Name?
	uint32_t       pc;            ///< So we can restore the program counter of the caller
	unsigned char* ebp;           ///< So we can restore the base pointer of the caller
	RzModule*      module;
	void*          object;
};

#endif // RZSTACKFRAME_H__
