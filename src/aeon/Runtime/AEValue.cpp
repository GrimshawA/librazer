#include <AEON/Runtime/AEValue.h>
#include <AEON/Runtime/AEObject.h>
#include <cstdlib>

bool AEValueRef::createOnAssign()
{
	if (container && container->type() == AEValue::VALUE_OBJECT)
		return true;

	return false;
}

//////////////////////////////////////////////////////////////////////////

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

	printf("Returning int as %d\n", _int);

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

AEValue::AEValue(AEObject* obj)
{
	m_valueType = VALUE_OBJECT;
	_object = obj;
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

AEValue AEValue::at(int index)
{
	return _array->values[index];
}

std::string AEValue::typeName() const
{
	if (m_valueType == VALUE_OBJECT)
	{
		return property("typename").str();
	}
	else
		return "";
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
		if (_array->values.size() < index + 1)
			_array->values.resize(index + 1);
		_array->values[index] = v;
	}
}

void AEValue::setValue(const std::string& name, const std::string& value)
{
	if (m_valueType == VALUE_OBJECT)
	{
		_object->setProperty(name, value);
	}
	else if (m_valueType == VALUE_ANONOBJECT)
	{
		_dynObj->setValue(name, value);
	}
	else if (!m_valueType)
	{
		m_valueType = VALUE_ANONOBJECT;
		_dynObj = new AEDynamicObject;
		_dynObj->setValue(name, value);
	}
}

void AEValue::setValue(const std::string& name, int value)
{
	if (m_valueType != VALUE_OBJECT)
	{
		_object = new AEObject;
		m_valueType = VALUE_OBJECT;
	}

	//printf("%s: %s\n", name.c_str(), value.str().c_str());
	_object->setProperty(name, AEValue(value));
}

void AEValue::setValue(const std::string& name, const AEValue& value)
{
	if (m_valueType != VALUE_OBJECT)
	{
		_object = new AEObject;
		m_valueType = VALUE_OBJECT;
	}

	printf("%s: %s\n", name.c_str(), value.str().c_str());
	_object->setProperty(name, (AEValue)value);
}

AEValue AEValue::property(const std::string& name) const
{
	if (m_valueType != VALUE_OBJECT)
		return AEValue();

	int i = 0;
	for (auto& m : _object->m_properties)
	{
		printf("Searching %s vs %s\n", _object->m_names[i].c_str(), name.c_str());
		if (_object->m_names[i] == name)
		{
			return m;
		}
		++i;
	}

	return AEValue();
}

AEValue AEValue::property(int index)
{
	return _object->m_properties[index];
}

std::string AEValue::propertyName(int index)
{
	return _object->propertyName(index);
}

int AEValue::numProperties() const
{
	if (m_valueType == VALUE_OBJECT)
	{
		return _object->count();
	}

	return 0;
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
		if (_object->contains("typename"))
			return std::string("object:") + property("typename").str();
		else
			return "object";
	}
	else if (m_valueType == VALUE_REAL)
	{
		return std::to_string(_real);
	}
	else
		return "undefined";
}

const char* AEValue::c_str() const
{
	return str().c_str();
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

int AEValue::toInteger()
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
		return -1;
}

std::string AEValue::toString() const
{
	return str();
}

bool AEValue::toBoolean() const
{
	if(m_valueType == VALUE_OBJECT)
		return true;
	else if(m_valueType == VALUE_INT)
		return _int > 0;
	else if(m_valueType == VALUE_REAL)
		return _real > 0;
	else if(m_valueType == VALUE_ANONOBJECT)
		return true;
	else if(m_valueType == VALUE_STRING)
		return false;

	return false;
}

AEValue AEValue::makeArray()
{
	AEValue v;
	v.m_valueType = VALUE_ARRAY;
	v._array = new AEArray;
	return v;
}

AEValueRef AEValue::makeRef()
{
	AEValueRef ref;
	if (m_valueType == VALUE_INT)
	{
		ref.data = &_int;
		ref.type = AEValueRef::REF_RAW;
	}
	return ref;
}

AEValueRef AEValue::makeRefForChild(const std::string& name)
{
	AEValueRef ref;
	ref.data = nullptr;
	ref.type = AEValueRef::REF_UNDEFINED;
	ref.container = this;

	if (m_valueType == VALUE_OBJECT)
	{
		for (int i = 0; i < _object->m_properties.size(); ++i)
		{
			if (_object->m_names[i] == name)
			{
				if (_object->m_properties[i].m_valueType == VALUE_INT)
				{
					ref.data = ((char*)&_object->m_properties[i]._int);
					ref.type = AEValueRef::REF_RAW;
					printf("<< Filled raw int ptr for this %x %s  %x\n", _object, name.c_str(), &_object->m_properties[i]._int);
					return ref;
				}
			}
		}

		// Get here means it doesnt exist: create
		_object->setProperty(name, AEValue());
		ref.value = _object->getValueRef(name);
		ref.type = AEValueRef::REF_VALUE;
	}
	
	return ref;
}

AEValue& AEValue::operator=(const AEValue& v)
{
	if (v.m_valueType == VALUE_ARRAY)
	{
		_array = v._array;
	}
	else if (v.m_valueType == VALUE_OBJECT)
	{
		_object = v._object;
	}
	else if (v.m_valueType == VALUE_INT)
	{
		_int = v._int;
	}
	else if (v.m_valueType == VALUE_REAL)
	{
		_real = v._real;
	}
	else if (v.m_valueType == VALUE_STRING)
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

AEValue AEValue::operator+(const AEValue& b)
{
	if (b.m_valueType == VALUE_ARRAY)
	{
		return operator+(*b._array);
	}
	else if (b.m_valueType == VALUE_INT)
	{
		return operator+(b._int);
	}
	else if (b.m_valueType == VALUE_STRING)
	{
		return operator+(*b._string);
	}
	else
		return AEValue();
}

AEValue AEValue::operator-(const AEValue& b)
{
	return AEValue();
}

AEValue AEValue::operator*(const AEValue& b)
{
	return AEValue();
}

AEValue AEValue::operator/(const AEValue& b)
{
	return AEValue();
}

bool AEValue::operator==(const AEValue& b)
{
	return false;
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

AEValue::operator bool()
{
	return toBoolean();
}

void AEValue::setFromArray(AEValue v)
{

}

void AEValue::setFromString(AEValue v) 
{

}

void AEValue::setFromObject(void* ptr, AEType* typeInfo)
{
	_object = new AEObject();
	_object->m_obj = ptr;
	_object->m_type = typeInfo;
	m_valueType = VALUE_OBJECT;
}
