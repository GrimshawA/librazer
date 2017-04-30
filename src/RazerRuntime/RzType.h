#ifndef RZTYPE_H__
#define RZTYPE_H__

#include <RazerRuntime/QualType.h>
#include <RazerRuntime/Generic.h>
#include <RazerRuntime/aeSymbol.h>
#include <vector>
#include <bitset>
#include <string>
#include <stdint.h>

class RzModule;
class RzEngine;
class RzVirtualMachine;
class RzFunction;

typedef void(*aeBindMethod)(RzGeneric);
typedef void(*aeDestructorMethod)(void*);
typedef void(*aeConstructorMethod)(void*, RzVirtualMachine*);

#define 	aeOFFSET(s, m)   ((size_t)(&reinterpret_cast<s*>(100000)->m)-100000)

/**
\class aeType
\brief Every type available to script is represented by one of this

It provides a seamless interface to list and query all available types
in the language. They also map to the TypeInfo struct that results of the
typeof(X) operator.
*/
class RzType : public aeSymbol
{
public:
	friend class RzEngine;

    enum TypeFlag {
        FLAG_PRIMITIVE
    };

	struct EnumEntryInfo
	{
		std::string name;
		int         value;
	};

	struct EnumInfo
	{
		std::string name;
		std::vector<EnumEntryInfo> entries;
	};
		
	struct MethodInfo
	{
		std::string name; ///< The name by which the language knows it
        std::vector<RzQualType> args; ///< The arguments it takes
		int type;
		int offset;
        RzFunction* func = nullptr; // the vm function if applicable
        bool native = false;

		union
		{
			aeBindMethod methodCallback;
			aeConstructorMethod constructorCallback;				
		};
	};

	struct ParentTypeInfo
	{
		RzType* ref;
		std::string access;
	};

	enum TypeKind
	{
		KindClass,
		KindEnum
	};

public:
	/// Build the type info object, explicitly used when declaring c++ types
    RzType(RzModule& module, const std::string& _name, uint32_t _size);

	/// Creates a new instance of the native object
	void* construct();
	void* constructNative();
	void* constructScript();

    // Figure the final size of the struct
    void computeMetrics();

    /// Check if his type is a primitive
    bool isPrimitive() const;

	/// Is this type an enum
	bool isEnum();

	/// Check if this type is templated( type<...>
	bool isTemplated();

	/// Check if this type is a class
	bool isClass();

	/// Check if the type is native-side defined
	bool isNative();

	/// Check if this type is a pod type
	bool isPod();

	/// Get the current number of methods
	uint32_t getNumMethods();

	/// Get the current number of fields on this type
	uint32_t getNumFields();

	/// Get the number of protocols this type adheres to
	uint32_t getNumProtocols();
		
	/// Get a method with the given name, no overload resolution
	MethodInfo* getMethod(const std::string& methodName) const;

	/// All and every type defined in aeon is script side
	bool isScriptSide();

	/// Host side are for application side types exposed to scripts
	bool isHostSide();

	/// Hybrids are script side that incorporate a cpp object
	bool isHybrid();

	/// Get the name of the type
	std::string getName();

	/// Get the id of the function by its name
	int getFunctionId(const std::string& name);

	RzFunction* getFunction(const std::string& name);

	/// Fetch the direct function pointer used to register the API
	aeBindMethod getNativeFunction(const std::string& name);

    /// Select the appropriate method to call based on name and arg list
    MethodInfo selectMethod(const std::string& name, const std::vector<RzQualType>& argsList);

	/// Get the field information for a given type
	aeField* getField(std::string name);

	/// Get the size of the type (C + script class)
	uint32_t getSize();

	/// Get the module this type is a part of
	RzModule* getModule();

    void setFlag(TypeFlag flag);

	/// Takes some input parameters and properly prepares the type for the new field
	void createField(aeField fieldInfo);

	/// Allow to register a host method into the type
	void registerMethod(const std::string& name, void* funptr);

	/// Allow to register a host class field
	void registerField(const std::string& name, int offset);

	/// Register a new enum
	void registerEnum(const std::string& name);

	/// Register a property to the enum
	void registerEnumProperty(const std::string& name, const std::string& property, int value);

//private:
	uint32_t                    m_size;   ///< The absolute type size in bytes (cpp + aeon fields)
	std::vector<aeField>        m_fields; ///< Every field that contributes to the final object

	TypeKind                    m_typeKind = KindClass;
	std::string                 m_name;
	std::string                 m_namespace;
	int                         m_id;
	int                         m_localId;
	bool                        m_allowPartialDeclaration;
	bool                        m_isPolymorphic;
	std::bitset<32>             m_flags;
	std::vector<MethodInfo>     m_methods;
	std::vector<EnumInfo>       m_enums;

    std::vector<std::string>    m_templateParams;

	bool is_native = false;
	void*                       m_userData;            ///< This allows the user to inject additional info on the type
	aeDestructorMethod          m_destructor;
	bool m_pod = true;

private:
    RzModule&                   m_module;
};

#endif // RZTYPE_H__
