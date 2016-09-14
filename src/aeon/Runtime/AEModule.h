#ifndef AEMODULE_H__
#define AEMODULE_H__

#include <AEON/Runtime/AEByteCode.h>
#include <AEON/Runtime/AEType.h>
#include <AEON/aeQualType.h>
#include <AEON/Runtime/AEFunction.h>
#include <vector>
#include <array>
#include <stdint.h>

typedef int FnIndex;

/**
	\class AEModule
	\brief One or more scripts are compiled into a module

	By default, each file will generate a module for itself, but multiple files
	can be grouped together in the same module. Each module is named and can reference
	other modules.

	For example, the standard module std.io will contain multiple classes and functions,
	that are defined among multiple files.

	Each module usually maps to one .obj file on disk, result of compilation.

	When compiling code, the code being compiled will belong to the module it specifies in source
	and its dependencies to imported modules will be resolved.

	-- auto_import_modules
	This allows for a module to import other modules by default. If a module was pre-prepared to
	import a given module, it can use types in the imported module without ever explicitly doing
	the import.
*/
class AEModule
{
public:

	AEModule();
	~AEModule();

	AEFunction* getFunction(const std::string& name);
	AEFunction* getFunction(FnIndex index);

	FnIndex getFunctionIndex(const std::string& name);

	uint64_t getFunctionOffset(const std::string& name);
	uint64_t getFunctionOffset(FnIndex functionIndex);

	int64_t getTypeIndex(const std::string& name);
	int64_t getTypeIndex(AEType* type);
	
	AEType* getType(const std::string& name);
	AEType* getType(int64_t index);

	std::string getStringFromPool(uint32_t index);
	double      getDoubleLiteral(uint32_t index);
	uint64_t    getIntegerLiteral(uint32_t index);


	int identifierPoolIndex(const std::string& identifier);

	std::string getName();


	uint32_t storeString(std::string s);

	/// Check if there is a function with this name in the module
	bool hasFunction(const std::string& name);

	int getFunctionIndexByName(const std::string& name);

	/// Write the module object to a file
	void write(const char* filename);

	/// Read the module from file
	void read(const char* filename);

	/// Write the program source to a human readable format ( hopefully )
	void dumpToFile(const std::string& filename);

	void debugCode();

public:

	std::string                   m_name;               ///< Every module must have a name like stdlib.io or nephilim.core.graphics
	AEContext*                    m_context;
	std::vector<AEFunction>       m_functions;          ///< Every static class function and global is listed here
	std::vector<AEType*>          m_types;
	std::vector<std::string>      m_stringPool;
	std::vector<double>           m_doublePool;
	std::vector<int64_t>          m_intPool;
	std::vector<std::string>      m_identifierPool;
	std::vector<AEInstruction>    m_code;               ///< The entire module's bytecode
	std::vector<AEModule*>        m_dependencies;       ///< Dependencies of this module
};

#endif // AEMODULE_H__
