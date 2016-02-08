#ifndef aeon_context_h__
#define aeon_context_h__

#include "aeon_generics.h"
#include "aeCodeFile.h"
#include "aeQualType.h"
#include "aeon_type.h"
#include "aeon_value.h"
#include <vector>
#include <memory>
#include <stdint.h>

class aeon_vm;
class aeon_module;
class aeon_object;

struct aeon_config
{
	bool allowNativeInheritance; ///< Throws a compiler error if the script class tries to inherit from a c++ type
	bool allowMultipleInheritance; ///< Throws an error if a script class tries to inherit multiple bases
	bool allowDynamicTyping;         ///< Allow anonymous variables to be fully dynamic (type aeon_variant)
	bool allowAutomaticTypeInference; ///< Anonymous variables assume a type from inference, but are immutable (and not aeon_variant), otherwise explicit inference is supported
};

typedef void(*aeBindMethod)(aeon_vm*);
typedef void(*aeDestructorMethod)(void*);
typedef void(*aeConstructorMethod)(void*, aeon_vm*);

typedef int aeLiteralId;
typedef int aeFunctionId;

class aeFunction;

/**
	\class aeon_context
	\brief Brings different modules together in a single runtime environment
*/
class aeon_context
{
	public:

		struct object_heap
		{
			std::vector<unsigned char*> memory;
			std::vector<unsigned char*> freestore;
			aeType*                  type;
			// start with the most naive implementation
			// allocates with malloc for every single object and frees when needed
			// todo: fancier memory management and allocation
			std::vector<aeon_object*> objects;
		};

		std::vector<aeUnit>                           m_units;
		std::vector<aeType*>                       typedb;               ///< All the types on the context
		std::vector<object_heap>                      object_heaps;         ///< An heap of memory for each object type
		std::vector<std::unique_ptr<aeon_module> >    modules;              ///< All the loaded modules
		std::vector<std::string>                      string_literals;      ///< All the loaded string literals
		std::vector<double>                           m_floatTable;      ///< All the loaded double literals
		std::vector<int32_t>                          int_literals;         ///< All the script-side int literals loaded
		std::vector<aeFunction*>                      m_functionTable;      ///< All the functions available to scripts
		aeon_config                                   m_config;             ///< Language configurations for compilation and execution
public:

	aeon_context();

	void init_all();

	aeFunction* createFunction(const std::string& name);

	aeLiteralId getIntegerLiteral(int64_t n);
	aeLiteralId getStringLiteral(const std::string& s);
	aeLiteralId getFloatLiteral(float v);

	/// Executes and evaluates an arbitrary expression and returns the value
	aeon_value evaluate(const std::string& expression);
	
	void quick_build(const std::string& file);

	aeon_module* getModule(const std::string name);

	aeon_module* create_module(const std::string& name);


	/// Registers a native type to work with the language
	void registerType(const std::string& name, std::size_t size, const std::string& namespc = "");
	void registerTypeMethod(const std::string& typeName, const std::string& name, aeBindMethod method);
	void registerTypeBehavior(const std::string& typeName, const std::string& behavName, aeBindMethod dest);
	void registerTypeDestructor(const std::string& typeName, aeDestructorMethod dest);
	void registerTypeField(const std::string& typeName, const std::string& decl, int offset);
	void registerFunction(const std::string& decl, aeBindMethod func);

	aeFunction* getFunctionByIndex(uint32_t index);
	aeFunction* getFunctionByName(const std::string& name);
	uint32_t    getFunctionIndexByName(const std::string& name);

	aeFunctionId getNativeBehaviorIndex(const std::string& typeName, const std::string& behavior);

	aeType* getTypeInfo(const std::string& name);

	/// Creates a new object of that type
	aeon_object* createObject(const std::string& typen);

	/// Destroys the object and frees its memory
	void destroyObject(aeon_object* object);

	void register_primitive(const std::string& name, uint32_t size);
};

#endif // aeon_context_h__
