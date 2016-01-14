#ifndef aeon_type_h__
#define aeon_type_h__

#include "aeon_generics.h"
#include <vector>
#include <bitset>
#include <string>
#include <stdint.h>

class aeon_module;

/**
	\class aeon_type
	\brief Every type available to script is represented by one of this

	It provides a seamless interface to list and query all available types
	in the language. They also map to the TypeInfo struct that results of the
	typeof(X) operator.
*/
class aeon_type
{
	public:

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
				struct native
				{
					void* funptr;
				};

				struct script
				{
					int index;
				};
			};
		};

		struct FieldInfo
		{
			uint16_t offset;
			uint16_t size;
			std::string name;
		};

		struct NestedTypeInfo
		{
			aeon_type* ref;
		};

		struct ParentTypeInfo
		{
			aeon_type* ref;
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

	public:

		/// Build the type info object
		aeon_type();

		/// Build the type info object, explicitly used when declaring c++ types
		aeon_type(const std::string& _name, uint32_t _size);

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
		FieldInfo* getField(std::string name);

		/// Get the size of the type (C + script class)
		uint32_t getSize();

		/// Get the module this type is a part of
		aeon_module* getModule();

		/// Takes some input parameters and properly prepares the type for the new field
		void createField(FieldInfo fieldInfo);

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
		std::vector<FieldInfo>      m_fields; ///< Every field that contributes to the final object
};

#endif // aeon_type_h__
