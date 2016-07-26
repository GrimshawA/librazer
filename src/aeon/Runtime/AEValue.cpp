#include <AEON/Runtime/AEValue.h>
#include <AEON/Runtime/AEObject.h>
#include <cstdlib>

AEValue::AEValue()
: m_valueType(VALUE_UNDEFINED)
, _real(0)
{
	
}

AEValue::AEValue(std::initializer_list<int> v)
{
	m_valueType = VALUE_ARRAY;
	_array = new AEArray();

	for (auto& elem : v)
	{
		_array->values.push_back(AEValue(elem));
	}
}

AEValue::AEValue(int v)
{
	m_valueType = VALUE_INT;
	_int = v;
}

AEValue::AEValue(const std::string& v)
{
	m_valueType = VALUE_STRING;
	_string = new AEString();
	_string->ref++;
	_string->str = v;
}

AEValue::AEValue(std::function<void()> fn)
{
	m_valueType = VALUE_FUNCTION;
	_function = new AEFunction();
	_function->tmp = fn;
}

AEValue::~AEValue()
{
	//release();
}

AEValue::ValueType AEValue::type() const
{
	return m_valueType;
}

int AEValue::length() const
{
	if (m_valueType == VALUE_ARRAY)
	{
		return _array->values.size();
	}
	else if (m_valueType == VALUE_STRING)
	{
		return _string->str.size();
	}
	else
		return 0;
}

void AEValue::release()
{
	if (m_valueType == VALUE_STRING)
	{
		if (--_string->ref == 0)
			delete _string;
	}

	m_valueType = VALUE_UNDEFINED;
}

void AEValue::setValue(int index, AEValue v)
{
	if (m_valueType == VALUE_ARRAY)
	{
		_array->values[index] = v;
	}
}

void AEValue::setProperty(const std::string& name, const std::string& value)
{
	if (m_valueType == VALUE_OBJECT)
	{
		_object->setProperty(name, value);
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
	if (m_valueType == VALUE_FUNCTION)
	{
		_function->tmp();
	}
}

void AEValue::call(const AEValueList& argumentList)
{

}

std::string AEValue::str() const
{
	if (m_valueType == VALUE_UNDEFINED)
	{
		return std::string("undefined");
	}
	else if (m_valueType == VALUE_STRING)
	{
		return _string->str;
	}
	else if (m_valueType == VALUE_FUNCTION)
	{
		return "function";
	}
	else if (m_valueType == VALUE_ARRAY)
	{
		std::string ret = "[";
		int i = 0;
		for (auto& elem : _array->values)
		{
			ret += elem.str();
			if (i < _array->values.size()-1)
				ret += ", ";
			++i;
		}
		ret += "]";
		return ret;
	}
	else if (m_valueType == VALUE_INT)
	{
		return std::to_string(_int);
	}
	else if (m_valueType == VALUE_OBJECT)
	{
		return "object";
	}
	else if (m_valueType == VALUE_REAL)
	{
		return std::to_string(_real);
	}
	else
		return "undefined";
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

int AEValue::asInt()
{
	if (m_valueType == VALUE_INT)
	{
		return _int;
	}
	else if (m_valueType == VALUE_REAL)
	{
		return static_cast<int>(_real);
	}
	else
		return 0;
}

AEValue& AEValue::operator=(const AEValue& v)
{
	if (m_valueType == VALUE_ARRAY)
	{
		_array = v._array;
	}
	else if (m_valueType == VALUE_OBJECT)
	{
		_object = v._object;
	}
	else if (m_valueType == VALUE_INT)
	{
		_int = v._int;
	}
	else if (m_valueType == VALUE_REAL)
	{
		_real = v._real;
	}
	else if (m_valueType == VALUE_STRING)
	{
		_string = v._string;
	}
	m_valueType = v.m_valueType;
	return *this;
}

AEValue& AEValue::operator=(const std::string& v)
{
	if (m_valueType != VALUE_STRING)
	{
		release();

		_string = new AEString();
		_string->str = v;
		_string->ref++;
		m_valueType = VALUE_STRING;
	}
	else
	{
		_string->str = v;
	}

	return *this;
}

AEValue& AEValue::operator=(int v)
{
	m_valueType = VALUE_INT;
	_int = v;
	return *this;
}

AEValue& AEValue::operator=(float v)
{
	_real = v;
	return *this;
}

AEValue AEValue::operator+(AEValue v)
{
	if (v.m_valueType == VALUE_ARRAY)
	{
		return operator+(*v._array);
	}
	else if (v.m_valueType == VALUE_INT)
	{
		return operator+(v._int);
	}
	else if (v.m_valueType == VALUE_STRING)
	{
		return operator+(*v._string);
	}
	else
		return AEValue();
}

AEValue AEValue::operator+(AEArray& v)
{
	AEValue ret;
	if (m_valueType == VALUE_ARRAY)
	{
		// Array + Array = concatenated array
		ret.m_valueType = VALUE_ARRAY;
		ret._array = new AEArray();

		for (auto& elem : _array->values)
		{
			ret._array->values.push_back(elem);
		}

		for (auto& elem : v.values)
		{
			ret._array->values.push_back(elem);
		}
		return ret;
	}
	else
		return AEValue();
}

AEValue AEValue::operator+(AEString& v)
{
	return AEValue();
}

AEValue AEValue::operator+(int32_t v)
{
	AEValue r;
	if (m_valueType == VALUE_INT)
	{
		r.m_valueType = VALUE_INT;
		r._int = _int + v;
		return r;
	}
	else if (m_valueType == VALUE_STRING)
	{
		AEString* newStr = new AEString();
		newStr->str = _string->str + std::to_string(v);
		newStr->ref++;
		r.m_valueType = VALUE_STRING;
		return r;
	}
	else
	{
		r.m_valueType = VALUE_UNDEFINED;
		return r;
	}
}

void AEValue::setFromArray(AEValue v)
{

}

void AEValue::setFromString(AEValue v) 
{

}