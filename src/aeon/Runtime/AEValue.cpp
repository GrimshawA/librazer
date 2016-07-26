#include <AEON/Runtime/AEValue.h>
#include <AEON/Runtime/AEObject.h>
#include <cstdlib>

AEValue::AEValue()
: m_valueType(VALUE_UNDEFINED)
, m_data(nullptr)
{
	
}

AEValue::~AEValue()
{

}

void AEValue::release()
{
	
}

void AEValue::setProperty(const std::string& name, const std::string& value)
{
	if (m_valueType == VALUE_OBJECT)
	{
		AEObject* object = (AEObject*)m_data;
		object->setProperty(name, value);
	}
}

void AEValue::setProperty(const std::string& name, int value)
{

}

void AEValue::setProperty(const std::string& name, const AEValue& value)
{

}

AEValue AEValue::property(const std::string& name)
{
	return AEValue();
}

void AEValue::call()
{

}

void AEValue::call(const AEValueList& argumentList)
{

}

bool AEValue::isCallable()
{
	return m_valueType == VALUE_FUNCTION;
}

bool AEValue::isUndefined()
{
	return m_valueType == VALUE_UNDEFINED;
}

bool AEValue::isString()
{
	return m_valueType == VALUE_STRING;
}

AEValue& AEValue::operator=(const AEValue& v)
{
	return *this;
}

AEValue& AEValue::operator=(const std::string& v)
{
	return *this;
}

AEValue& AEValue::operator=(int v)
{
	m_valueType = VALUE_INT;
	memcpy(m_data, &v, sizeof(int));
	return *this;
}

AEValue& AEValue::operator=(float v)
{
	return *this;
}

void AEValue::CreateString(const std::string& name, const std::string& value)
{
		AEValue v;
//		v.mRawProperty = name;
	//	v.mRawValue = value;
		//properties.push_back(v);
}

void AEValue::CreateFloat(const std::string& name, float value)
{
		AEValue v;
//		v.mRawProperty = name;
		//v.mRawValue = std::to_string(value);
	//	properties.push_back(v);
}

	void AEValue::CreateNumber(const std::string& name, const std::string& number)
	{
		AEValue v;
//		v.mRawProperty = name;
	//	v.mRawValue = number;
		//properties.push_back(v);
	}

	void AEValue::SetNumber(const std::string& name, const std::string& number)
	{
//		mRawProperty = name;
	//	mRawValue = number;
	}

	/// Sets this value as a number
	void AEValue::SetString(const std::string& name, const std::string& value)
	{
//		mRawProperty = name;
	//	mRawValue = value;
	}

	/// Check if this value is an object
	/// It is an object as long as it has more than one property
	bool AEValue::IsObject()
	{
		return m_valueType == VALUE_OBJECT;
	}

	AEValue* AEValue::begin()
	{
//		return properties.size() > 0 ? &properties[0] : end();
		return nullptr;
	}

	AEValue* AEValue::end()
	{
		//return properties.size() > 0 ? (&properties[properties.size() - 1]) + 1 : nullptr;
		return nullptr;
	}

	void AEValue::debugPrint()
	{
		if (m_valueType == VALUE_ARRAY)
		{
			printf("[");

			printf("]\n");
		}

		//Log("AtomValue: %s -> %s", mRawProperty.c_str(), mRawValue.c_str());
		//for (auto i : properties)
		//	i.debugPrint();
	}

	int AEValue::as_int()
	{
		//return atoi(mRawValue.c_str());
		return 0;
	}

	float AEValue::as_float()
	{
		//return atof(mRawValue.c_str());
		return 0;
	}

	std::string AEValue::as_string()
	{
		//return mRawValue;
		return "";
	}

	AEValue AEValue::operator[](const std::string& str)
	{
		/*for (auto& v : properties)
		{
			if (v.mRawProperty == str)
				return v;

			//Log("Comparison %s vs %s FAILED.", str.c_str(), v.mRawProperty.c_str());

		}*/

		return AEValue();
}