#ifndef aeon_type_h__
#define aeon_type_h__

#include "aeon_generics.h"
#include "aeQualType.h"
#include "aeSymbol.h"
#include <vector>
#include <bitset>
#include <string>
#include <stdint.h>

class aeon_module;
class aeon_context;

typedef void(*aeBindMethod)(aeVM*);
typedef void(*aeDestructorMethod)(void*);
typedef void(*aeConstructorMethod)(void*, aeVM*);

#define 	aeOFFSET(s, m)   ((size_t)(&reinterpret_cast<s*>(100000)->m)-100000)

/**
	\class aeType
	\brief Every type available to script is represented by one of this

	It provides a seamless interface to list and query all available types
	in the language. They also map to the TypeInfo struct that results of the
	typeof(X) operator.
*/
class aeType : public aeSymbol
{
	public:
		friend class aeon_context;

		enum ETypeFlags
		{
			TYPE_POD,
			TYPE_NOVTABLE,
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
			std::vector<std::string> args; ///< The arguments it takes
			int type;

			union
			{
				aeBindMethod methodCallback;
				aeConstructorMethod constructorCallback;

				
			};
		};

		struct NestedTypeInfo
		{
			aeType* ref;
		};

		struct ParentTypeInfo
		{
			aeType* ref;
			std::string access;
		};

		struct ProtocolInfo
		{
			//implement
		};

		std::string                 m_name;
		std::string                 m_namespace;
		int                         m_id;
		int                         m_localId;
		bool                        m_allowPartialDeclaration;
		bool                        m_isPolymorphic;
		std::bitset<32>             m_flags;
		std::vector<MethodInfo>     m_methods;
		std::vector<EnumInfo>       m_enums;
		std::vector<NestedTypeInfo> m_structs;
		std::vector<ProtocolInfo>   m_protocols;
		aeon_module*                m_module;
		bool is_native = false;
		aeDestructorMethod          m_destructor;

	public:

		/// Build the type info object
		aeType();

		/// Build the type info object, explicitly used when declaring c++ types
		aeType(const std::string& _name, uint32_t _size);

		/// Get the current number of methods
		uint32_t getNumMethods();

		/// Get the current number of fields on this type
		uint32_t getNumFields();

		/// Get the number of protocols this type adheres to
		uint32_t getNumProtocols();

		/// All and every type defined in aeon is script side
		bool isScriptSide();

		/// Host side are for application side types exposed to scripts
		bool isHostSide();

		/// Hybrids are script side that incorporate a cpp object
		bool isHybrid();

		/// Get the name of the type
		std::string getName();

		/// Get the field information for a given type
		aeField* getField(std::string name);

		/// Get the size of the type (C + script class)
		uint32_t getSize();

		/// Get the module this type is a part of
		aeon_module* getModule();

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

	private:
		uint32_t                    m_size;   ///< The absolute type size in bytes (cpp + aeon fields)
		std::vector<aeField>        m_fields; ///< Every field that contributes to the final object
};

#endif // aeon_type_h__
