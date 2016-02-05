#include "aeon_value.h"
#include <cstdlib>

aeon_value::aeon_value()
{
	mValueType = EObject;
}

void aeon_value::CreateString(const std::string& name, const std::string& value)
{
		aeon_value v;
		v.mRawProperty = name;
		v.mRawValue = value;
		properties.push_back(v);
}

void aeon_value::CreateFloat(const std::string& name, float value)
{
		aeon_value v;
		v.mRawProperty = name;
		//v.mRawValue = std::to_string(value);
		properties.push_back(v);
}

	void aeon_value::CreateNumber(const std::string& name, const std::string& number)
	{
		aeon_value v;
		v.mRawProperty = name;
		v.mRawValue = number;
		properties.push_back(v);
	}

	void aeon_value::SetNumber(const std::string& name, const std::string& number)
	{
		mRawProperty = name;
		mRawValue = number;
	}

	/// Sets this value as a number
	void aeon_value::SetString(const std::string& name, const std::string& value)
	{
		mRawProperty = name;
		mRawValue = value;
	}

	/// Check if this value is an object
	/// It is an object as long as it has more than one property
	bool aeon_value::IsObject()
	{
		return mValueType == EObject;
	}

	aeon_value* aeon_value::begin()
	{
		return properties.size() > 0 ? &properties[0] : end();
	}

	aeon_value* aeon_value::end()
	{
		return properties.size() > 0 ? (&properties[properties.size() - 1]) + 1 : nullptr;
	}

	void aeon_value::debugPrint()
	{

		//Log("AtomValue: %s -> %s", mRawProperty.c_str(), mRawValue.c_str());
		for (auto i : properties)
			i.debugPrint();
	}

	int aeon_value::as_int()
	{
		return atoi(mRawValue.c_str());
	}

	float aeon_value::as_float()
	{
		return atof(mRawValue.c_str());
	}

	std::string aeon_value::as_string()
	{
		return mRawValue;
	}

	aeon_value aeon_value::operator[](const std::string& str)
	{
		for (auto& v : properties)
		{
			if (v.mRawProperty == str)
				return v;

			//Log("Comparison %s vs %s FAILED.", str.c_str(), v.mRawProperty.c_str());

		}

		return aeon_value();
}