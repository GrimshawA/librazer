#ifndef RZMODULE_H__
#define RZMODULE_H__

#include <razer/config.h>
#include <razer/vm/InstructionSet.h>
#include <razer/runtime/RzType.h>
#include <razer/runtime/QualType.h>
#include <razer/runtime/Function.h>
#include <vector>
#include <array>
#include <deque>
#include <stdint.h>

class AENativeFunctionWrapper {
public:
	aeBindMethod f;
};

typedef int FnIndex;

/**
	\class RzModule
	\brief One or more scripts are compiled into a module

	By default, each file will generate a module for itself, but multiple files
	can be grouped together in the same module. Each module is named and can reference
	other modules.

	For example, the standard module std.io will contain multiple classes and functions,
	that are defined among multiple files.

	C++ must be exported into a language module as well, so its seamlessly recognized by the runtime.
	No code can live without a module in the context of the vm.

	Each module usually maps to one .obj file on disk, result of compilation.

	When compiling code, the code being compiled will belong to the module it specifies in source
	and its dependencies to imported modules will be resolved.

	-- auto_import_modules
	This allows for a module to import other modules by default. If a module was pre-prepared to
	import a given module, it can use types in the imported module without ever explicitly doing
	the import.
*/
class RZ_API RzModule
{
public:

	RzModule();
	~RzModule();

    /// Index of this module within the engine
    int index();

    /// Create a new function on the module to later point to a initial instruction
    RzFunction* createFunction(const std::string& name);

    RzFunction* getFunction(const std::string& name);
    RzFunction* getFunction(FnIndex index);

    FnIndex getFunctionIndex(RzFunction* func);
	FnIndex getFunctionIndex(const std::string& name);

	uint64_t getFunctionOffset(const std::string& name);
	uint64_t getFunctionOffset(FnIndex functionIndex);

	int64_t getTypeIndex(const std::string& name);
	int64_t getTypeIndex(RzType* type);
	
	RzType* getType(const std::string& name);
	RzType* getType(int64_t index);

	RzType* resolveType(int dependency_id, int type_id);

	std::string getStringFromPool(uint32_t index);
	double      getDoubleLiteral(uint32_t index);
	uint64_t    getIntegerLiteral(uint32_t index);

	int identifierPoolIndex(const std::string& identifier);

	void registerFunction(const std::string& sig, aeBindMethod fn);
	void registerGlobal(const std::string& sig, void* memory);

    RzType* registerType(const std::string& name, std::size_t size);
	void registerTypeConstructor(const std::string& name, aeConstructorMethod constructor);
    void registerTypeConstructor(const std::string& name, RzTemplateConstructorMethod constructor);
    void registerTypeDestructor();
	void registerMethod(const std::string& name, const std::string& sig, aeBindMethod fn);
	void registerField();
	void registerPropertyGet();
	void registerPropertySet();
	void registerEnum();
	void registerEnumValue();
	void registerTypedef();

	int resolveTypeModuleIndex(RzType* type);

public:
	// Type information

	void resolveType(RzQualType& type);
	bool hasType(const std::string& name);
	bool hasType(RzType* type);

public:

	int getDependencyId(const std::string& name);
	int getDependencyId(RzModule* module);

	bool depends(const std::string& name) const;
	bool depends(RzModule* module) const;

	int createDependency(const std::string& name);
	int createDependency(RzModule* module);

	RzModule* getDependantModule(int index);

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
		RzModule*   module;
	};

	std::string                          m_name;               ///< Every module must have a name like stdlib.io or nephilim.core.graphics
    RzEngine*                            m_context;
    std::deque<RzFunction>               m_functions;          ///< All script functions
	std::vector<AENativeFunctionWrapper> m_nativeFunctions;    ///< All native functions exported
	std::vector<RzType*>                 m_types;
	std::vector<std::string>             m_stringPool;
	std::vector<double>                  m_doublePool;
	std::vector<int64_t>                 m_intPool;
	std::vector<std::string>             m_identifierPool;
	std::vector<RzInstruction>           m_code;               ///< The entire module's bytecode
	std::vector<ModuleDependency>        m_dependencies;       ///< Dependencies of this module
    int                                  m_moduleIndex;        ///< Module index within the engine
};

#endif // RZMODULE_H__
