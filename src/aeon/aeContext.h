#ifndef aeon_context_h__
#define aeon_context_h__

#include <AEON/aeThreadState.h>
#include <AEON/aeQualType.h>
#include <AEON/aeType.h>
#include <AEON/aeVariant.h>
#include <vector>
#include <memory>
#include <map>
#include <stdint.h>

class aeVM;
class aeon_module;
class aeon_object;

struct aeon_config
{
	bool allowNativeInheritance; ///< Throws a compiler error if the script class tries to inherit from a c++ type
	bool allowMultipleInheritance; ///< Throws an error if a script class tries to inherit multiple bases
	bool allowDynamicTyping;         ///< Allow anonymous variables to be fully dynamic (type aeon_variant)
	bool allowAutomaticTypeInference; ///< Anonymous variables assume a type from inference, but are immutable (and not aeon_variant), otherwise explicit inference is supported
};

typedef void(*aeBindMethod)(aeVM*);
typedef void(*aeDestructorMethod)(void*);
typedef void(*aeConstructorMethod)(void*, aeVM*);

typedef int aeLiteralId;
typedef int aeFunctionId;

class aeFunction;

class aeEnum : public aeType
{
public:
	std::map<std::string, int> table;

	aeEnum()
	{
		m_typeKind = KindEnum;
	}
};

enum ObjectSpecifiers
{
	aeOBJ_POD = 1 << 0,
	aeOBJ_PRIMITIVE = 1 << 1,
};

class aeTypedef
{
public:
	std::string from;
	std::string to;
};

/**
	\class aeon_context
	\brief Brings different modules together in a single runtime environment
*/
class aeContext
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

		std::vector<aeType*>                          typedb;               ///< All the types on the context
		std::vector<object_heap>                      object_heaps;         ///< An heap of memory for each object type
		std::vector<std::unique_ptr<aeon_module> >    modules;              ///< All the loaded modules
		std::vector<std::string>                      string_literals;      ///< All the loaded string literals
		std::vector<double>                           m_floatTable;      ///< All the loaded double literals
		std::vector<int32_t>                          int_literals;         ///< All the script-side int literals loaded
		std::vector<aeFunction*>                      m_functionTable;      ///< All the functions available to scripts
		std::vector<aeTypedef>                        m_typedefs;
		std::vector<aeEnum*>                          m_enums;
		aeon_config                                   m_config;             ///< Language configurations for compilation and execution
public:

	aeContext();

	void init_all();

	aeFunction* createFunction(const std::string& name);

	aeLiteralId getIntegerLiteral(int64_t n);
	aeLiteralId getStringLiteral(const std::string& s);
	aeLiteralId getFloatLiteral(float v);

	/// Executes and evaluates an arbitrary expression and returns the value
	aeon_value evaluate(const std::string& expression);
	
	void quick_build(const std::string& file);

	aeon_module* getModule(const std::string name);

	aeon_module* createModule(const std::string& name);


	/// Registers a native type to work with the language
	void registerType(const std::string& name, std::size_t size, const std::string& namespc = "");
	void registerTypeMethod(const std::string& typeName, const std::string& name, aeBindMethod method);
	void registerTypeBehavior(const std::string& typeName, const std::string& behavName, aeBindMethod dest);
	void registerTypeDestructor(const std::string& typeName, aeDestructorMethod dest);
	void registerTypeField(const std::string& typeName, const std::string& decl, int offset);
	void registerFunction(const std::string& decl, aeBindMethod func);
	void registerEnum(const std::string& enumName);
	void registerEnumValue(const std::string& enumName, const std::string& valueName, int value);
	void registerPrimitive(const std::string& name, uint32_t size);
	void registerTypedef(const std::string& from, const std::string& to);

	aeFunction* getFunctionByIndex(uint32_t index);
	aeFunction* getFunctionByName(const std::string& name);
	uint32_t    getFunctionIndexByName(const std::string& name);
	aeEnum*     getEnumByName(const std::string& name);
	aeFunctionId getNativeBehaviorIndex(const std::string& typeName, const std::string& behavior);

	aeType* getTypeInfo(const std::string& name);
	int32_t getTypeInfoIndex(aeType* typeInfo);

	/// Creates a new object of that type
	aeon_object* createObject(const std::string& typen);

	/// Destroys the object and frees its memory
	void destroyObject(aeon_object* object);

};

#endif // aeon_context_h__
