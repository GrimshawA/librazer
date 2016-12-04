#ifndef RZENGINE_H__
#define RZENGINE_H__

#include <RazerVM/AEVmStack.h>
#include <AEON/Runtime/aeQualType.h>
#include <AEON/Runtime/AEType.h>
#include <Rzr/RzValue.h>
#include <Rzr/RzModule.h>
#include <AEON/Runtime/AEGeneric.h>
#include <vector>
#include <memory>
#include <map>
#include <stdint.h>

class AEVirtualMachine;
class AEObject;

struct aeon_config
{
	bool allowNativeInheritance; ///< Throws a compiler error if the script class tries to inherit from a c++ type
	bool allowMultipleInheritance; ///< Throws an error if a script class tries to inherit multiple bases
	bool allowDynamicTyping;         ///< Allow anonymous variables to be fully dynamic (type aeon_variant)
	bool allowAutomaticTypeInference; ///< Anonymous variables assume a type from inference, but are immutable (and not aeon_variant), otherwise explicit inference is supported
};

typedef void(*aeBindMethod)(AEGeneric);
typedef void(*aeDestructorMethod)(void*);
typedef void(*aeConstructorMethod)(void*, AEVirtualMachine*);

typedef int aeLiteralId;
typedef int aeFunctionId;

class AEFunction;

class aeEnum : public AEType
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
	\class RzEngine
	\brief A context represents the environment where the program lives and executes on

	Under the hood, there is one vm to execute the bytecode and callback into host code,
	which supports multi threading as well.

	The byte code the vm operates on lives in modules, which are all linked together under this environment.
	This guarantees an easy gateway to load or compile programs, and once everything is ready, execute them.
*/
class RzEngine
{
public:

	/// Loads the source file and calls main() on it, blocks until it returns
	/// Returns 0 on success and another value in case of error
	static int exec(const std::string& filename);

public:

		struct object_heap
		{
			std::vector<unsigned char*> memory;
			std::vector<unsigned char*> freestore;
			AEType*                  type;
			// start with the most naive implementation
			// allocates with malloc for every single object and frees when needed
			// todo: fancier memory management and allocation
			std::vector<AEObject*> objects;
		};

		std::vector<AEType*>                          typedb;               ///< All the types on the context
		std::vector<object_heap>                      object_heaps;         ///< An heap of memory for each object type
		std::vector<std::unique_ptr<RzModule> >       modules;              ///< All the loaded modules
		std::vector<std::string>                      string_literals;      ///< All the loaded string literals
		std::vector<std::string>                      m_functionIdTable;    ///< Maps each dynamic call identifier to an integer index
		std::vector<double>                           m_floatTable;         ///< All the loaded double literals
		std::vector<int32_t>                          int_literals;         ///< All the script-side int literals loaded
		std::vector<AEFunction*>                      m_functionTable;      ///< All the functions available to scripts
		std::vector<aeTypedef>                        m_typedefs;
		std::vector<aeEnum*>                          m_enums;
		aeon_config                                   m_config;             ///< Language configurations for compilation and execution
public:

	RzEngine();

	void init_all();

	AEFunction* createFunction(const std::string& name);

	aeLiteralId getIntegerLiteral(int64_t n);
	aeLiteralId getStringLiteral(const std::string& s);
	aeLiteralId getFloatLiteral(float v);

	/// Executes and evaluates an arbitrary expression and returns the value
	RzValue evaluate(const std::string& expression);
	
	void quick_build(const std::string& file);

	static RzValue readValue(const std::string& filename);

	RzModule* getModule(const std::string name);

	RzModule* createModule(const std::string& name);


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

	AEFunction* getFunctionByIndex(uint32_t index);
	AEFunction* getFunctionByName(const std::string& name);
	uint32_t    getFunctionIndexByName(const std::string& name);
	aeEnum*     getEnumByName(const std::string& name);
	aeFunctionId getNativeBehaviorIndex(const std::string& typeName, const std::string& behavior);

	AEType* getTypeInfo(const std::string& name);
	int32_t getTypeInfoIndex(AEType* typeInfo);

	/// Creates a new object of that type
	AEObject* createObject(const std::string& typen);

	/// Destroys the object and frees its memory
	void destroyObject(AEObject* object);

private:
	AEVirtualMachine* m_vm;
};

#endif // RZENGINE_H__
