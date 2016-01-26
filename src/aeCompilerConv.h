#ifndef aeCompilerConv_h__
#define aeCompilerConv_h__

#include "aeQualType.h"
#include <vector>

class aeon_type;
class aeon_compiler;

/// Every time the compiler has to convert a type into another, he uses one of these functions
typedef void(*ICGenerator)(aeon_compiler*);

/**
	\class ConversionProcedure
	\brief Converts type T1 into T2

	The structure contains information on how to convert T1 to T2.
	The built-in conversions will be special cases for the vm,
	while the user defined ones actually resort to calling methods to transform
	T1 input argument into the T2 return type.
*/
struct ConversionProcedure
{
	enum ConversionModes
	{
		Primitive,
		CustomOverload,
		NativeCustomOverload,
	};

	bool allow_implicit; /// A conversion procedure will be automatically performed if this flag is on, otherwise only with a cast<>
	aeon_type* T1;
	aeon_type* T2;
	int conversion_mode;

	union
	{
		struct builtin
		{
			ICGenerator generatorFunc;
		};

		struct userdefined
		{
			int methodId;
		};
	};
};

/*
	\class OperatorOverloadInfo
	\brief Each built-in operator overload is registered here and compiled from here

	helper to get operator overload information and quickly decide which one to use.
*/
struct OperatorOverloadInfo
{
	enum OverloadableOperatorType
	{
		Addition,
		Subtraction,
		Multiplication,
		Division,
	};

	int         m_type;
	aeQualType  m_returnType;
	aeQualType  m_t1;
	aeQualType  m_t2;
	ICGenerator m_function;
};

struct OperatorOverloadTable
{
	std::vector<OperatorOverloadInfo> m_table;

	/// We ask the table to give us the right overload for the types we have
	/// The response can be invalid in failure
	/// In success, the response carries the return type of the custom overload
	/// First, it matches compatibility literally, if nothing satisfies these types, it will start trying to find implicit conversions.
	/// The current context of expression counts, as which overloads can be seen and have precedence is relative to that.
	OperatorOverloadInfo findAppropriate(aeQualType t1, aeQualType t2, std::string operatr, aeon_compiler* c)
	{
		OperatorOverloadInfo info;
		return info;
	}
};

struct ConversionProcedureTable
{
	std::vector<ConversionProcedure> m_table;

	void init();

	bool canConvert(aeQualType T1, aeQualType T2);
};

#endif // aeCompilerConv_h__
