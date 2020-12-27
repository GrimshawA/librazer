#ifndef RZENGINE_H__
#define RZENGINE_H__

#include <razer/config.h>
#include <razer/vm/ThreadContext.h>
#include <razer/runtime/QualType.h>
#include <razer/runtime/RzType.h>
#include <razer/runtime/RzValue.h>
#include <razer/runtime/Module.h>
#include <razer/runtime/Generic.h>
#include <vector> 
#include <map>
#include <stdint.h> 

class RzVirtualMachine;
class AEObject;
class IDebugger;

struct aeon_config
{
	bool allowNativeInheritance; ///< Throws a compiler error if the script class tries to inherit from a c++ type
	bool allowMultipleInheritance; ///< Throws an error if a script class tries to inherit multiple bases
	bool allowDynamicTyping;         ///< Allow anonymous variables to be fully dynamic (type aeon_variant)
	bool allowAutomaticTypeInference; ///< Anonymous variables assume a type from inference, but are immutable (and not aeon_variant), otherwise explicit inference is supported
};

typedef void(*aeBindMethod)(RzGeneric);
typedef void(*aeDestructorMethod)(void*);
typedef void(*aeConstructorMethod)(void*, RzVirtualMachine*);
typedef void(*RzTemplateConstructorMethod)(void*, RzVirtualMachine*, int);

typedef int aeLiteralId;
typedef int aeFunctionId;

class RzFunction;

class aeEnum
{
public:
	std::map<std::string, int> table;

	aeEnum()
	{
        //m_typeKind = KindEnum;
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
class RZ_API RzEngine
{
public:

	/// Loads the source file and calls main() on it, blocks until it returns
	/// Returns 0 on success and another value in case of error
    static int exec(const std::string& filename, int argc, char** argv);

public:

		struct object_heap
		{
			std::vector<unsigned char*> memory;
			std::vector<unsigned char*> freestore;
			RzType*                  type;
			// start with the most naive implementation
			// allocates with malloc for every single object and frees when needed
			// todo: fancier memory management and allocation
			std::vector<AEObject*> objects;
		};

public:

	RzEngine();
    ~RzEngine();

	void setDebugger(IDebugger* dbg);

    RzFunction* createFunction(const std::string& name);

	aeLiteralId getIntegerLiteral(int64_t n);
	aeLiteralId getStringLiteral(const std::string& s);
	aeLiteralId getFloatLiteral(float v);

	/// Executes and evaluates an arbitrary expression and returns the value
	RzValue evaluate(const std::string& expression);
	
	void quick_build(const std::string& file);

	static RzValue readValue(const std::string& filename);

	RzModule* getModule(const std::string name);

	RzModule* createModule(const std::string& name);
	
	/// Resolve to which module the symbol resolves to, e.g 'std.io.File' would resolve to 'std'
	RzModule* resolveModule(const std::string& name);
    int getModuleIndex(RzModule* module);

    /// Registers a native type to work with the language
    void registerTypeField(const std::string& typeName, const std::string& decl, int offset);
	void registerEnum(const std::string& enumName);
	void registerEnumValue(const std::string& enumName, const std::string& valueName, int value);
	void registerPrimitive(const std::string& name, uint32_t size);
	void registerTypedef(const std::string& from, const std::string& to);

    RzFunction* getFunctionByName(const std::string& fullyQualifiedName) const;
    RzFunction* getFunctionByIndex(uint32_t index);
	uint32_t    getFunctionIndexByName(const std::string& name);
	aeEnum*     getEnumByName(const std::string& name);
	aeFunctionId getNativeBehaviorIndex(const std::string& typeName, const std::string& behavior);

	RzType* getTypeInfo(const std::string& name);
	int32_t getTypeInfoIndex(RzType* typeInfo);
	uint64_t getTypeModuleIndex(RzType* type);

	/// Creates a new object of that type
	AEObject* createObject(const std::string& typen);

	/// Destroys the object and frees its memory
	void destroyObject(AEObject* object);

public:
	RzVirtualMachine* m_vm;
    std::vector<RzType*>                          typedb;               ///< All the types on the context
    std::vector<object_heap>                      object_heaps;         ///< An heap of memory for each object type
    std::vector<std::unique_ptr<RzModule> >       modules;              ///< All the loaded modules
    std::vector<std::string>                      string_literals;      ///< All the loaded string literals
    std::vector<std::string>                      m_functionIdTable;    ///< Maps each dynamic call identifier to an integer index
    std::vector<double>                           m_floatTable;         ///< All the loaded double literals
    std::vector<int32_t>                          int_literals;         ///< All the script-side int literals loaded
    std::vector<aeTypedef>                        m_typedefs;
    std::vector<aeEnum*>                          m_enums;
    aeon_config                                   m_config;             ///< Language configurations for compilation and execution
    IDebugger*                                    _dbg = nullptr;
};

#endif // RZENGINE_H__
