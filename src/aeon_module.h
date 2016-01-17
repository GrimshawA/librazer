#ifndef aeon_module_h__
#define aeon_module_h__

#include "aeon_bytecode.h"
#include "aeon_type.h"
#include <vector>
#include <array>
#include <stdint.h>

// Useful stuff for a function defined in atom
struct aeon_function
{
		uint32_t offset; ///< which instruction this function starts in the code
		std::string   prototype;
};

struct aeon_method
{
		uint32_t    offset; ///< The offset in the module's bytecode this method logic begins
		std::string name;   ///< The identifier for this method
};

struct ae_module_header
{
	bool        m_fixPtrs;
	bool        abiIndependent;
	std::string name;
};

/**
	\class aeon_module
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
class aeon_module
{
	public:

		std::string                   name;         ///< Every module must have a name like stdlib.io or nephilim.core.graphics
		std::vector<aeon_function>    functions;    ///< Every static class function and global is listed here
		std::vector<aeon_method>      methods;      ///< Every non-static class function is listed here
		std::vector<aeon_type>        types;
		std::vector<std::string>      stringPool;
		std::vector<double>           double_literals;
		std::vector<std::string>      auto_import_modules;  ///<  
		std::vector<aeon_instruction> instructions;         ///< The entire module's bytecode

	public:


		std::string getStringFromPool(uint32_t index);

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
};

#endif // aeon_module_h__
