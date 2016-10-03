#ifndef AEMODULE_H__
#define AEMODULE_H__

#include <AEON/Runtime/AEByteCode.h>
#include <AEON/Runtime/AEType.h>
#include <AEON/aeQualType.h>
#include <AEON/Runtime/AEFunction.h>
#include <vector>
#include <array>
#include <stdint.h>

class AENativeFunctionWrapper {
public:
	aeBindMethod f;
};

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

	void exportType(const std::string& name, int32_t size);
	void exportMethod(const std::string& name, const std::string& signature, aeBindMethod fn);

	int identifierPoolIndex(const std::string& identifier);

public:

	int getDependencyId(const std::string& name);
	int getDependencyId(AEModule* module);

	bool depends(const std::string& name) const;
	bool depends(AEModule* module) const;

	int createDependency(const std::string& name);
	int createDependency(AEModule* module);

public:

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

	struct ModuleDependency
	{
		std::string name;
		std::string importAlias;
		AEModule*   module;
	};

	std::string                          m_name;               ///< Every module must have a name like stdlib.io or nephilim.core.graphics
	AEContext*                           m_context;
	std::vector<AEFunction>              m_functions;          ///< All script functions
	std::vector<AENativeFunctionWrapper> m_nativeFunctions;    ///< All native functions exported
	std::vector<AEType*>                 m_types;
	std::vector<std::string>             m_stringPool;
	std::vector<double>                  m_doublePool;
	std::vector<int64_t>                 m_intPool;
	std::vector<std::string>             m_identifierPool;
	std::vector<AEInstruction>           m_code;               ///< The entire module's bytecode
	std::vector<ModuleDependency>        m_dependencies;       ///< Dependencies of this module
};

#endif // AEMODULE_H__
