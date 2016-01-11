#ifndef aeon_context_h__
#define aeon_context_h__

#include "aeon_generics.h"
#include "aeon_type.h"
#include <vector>
#include <memory>
#include <stdint.h>

class aeon_vm;
class aeon_module;

/**
	\class aeon_context
	\brief Brings different modules together in a single runtime environment
*/
class aeon_context
{
	public:

		struct aeon_nativecall
		{
			typedef void(*func)(atom_generic*);
			func f;
			/// Just the function name
			std::string prototype;
			uint32_t    mask;
		};

		struct method_indirect
		{
			aeon_module* module;
			uint32_t     index;
		};

		struct object_heap
		{
			std::vector<unsigned char*> memory;
			std::vector<unsigned char*> freestore;
			aeon_type*                  type;
		};

		std::vector<aeon_type*>                       typedb;               ///< All the types on the context
		std::vector<object_heap>                      object_heaps;         ///< An heap of memory for each object type
		std::vector<std::unique_ptr<aeon_module> >    modules;              ///< All the loaded modules
		std::vector<std::string>                      string_literals;      ///< All the loaded string literals
		std::vector<double>                           double_literals;      ///< All the loaded double literals
		std::vector<int32_t>                          int_literals;         ///< All the script-side int literals loaded
		std::vector<aeon_nativecall>                  native_functions;     ///< All the native functions registered
		std::vector<aeon_nativecall>                  native_methods;       ///< All native class methods registered
		std::vector<method_indirect>                  table_methods;         ///< Every script class method has an absolute index in this table
		std::vector<method_indirect>                  table_functions;       ///< Every script function loaded has an absolute index to this table

public:

	aeon_context();

	void init_all();
	
	void quick_build(const std::string& file);

	aeon_module* getModule(const std::string name);

	aeon_module* create_module(const std::string& name);

	void registerFunction(const char* proto, void* f);

	void register_type(const std::string& name, std::size_t size);

	int32_t getNativeFunctionIndex(const std::string& name);

	aeon_type* getTypeInfo(const std::string& name);

	/// Allocates memory for an object of the given registered type or primitive
	void* alloc(uint32_t typeindex);

	/// Release memory of an object
	/// This memory model might change anytime
	void release(void* addr, uint32_t typeindex);

	void register_primitive(const std::string& name, uint32_t size);
};

#endif // aeon_context_h__
