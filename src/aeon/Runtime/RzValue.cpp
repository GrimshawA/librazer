#include <Rzr/RzValue.h>
#include <AEON/Runtime/AEObject.h>
#include <cstdlib>

bool AEValueRef::createOnAssign()
{
	if (container && container->type() == RzValue::VALUE_OBJECT)
		return true;

	return false;
}

//////////////////////////////////////////////////////////////////////////

RzValue::RzValue()
: m_valueType(VALUE_UNDEFINED)
, _real(0)
{
	
}

RzValue::RzValue(std::initializer_list<int> v)
{
	m_valueType = VALUE_ARRAY;
	_array = new AEArray();

	for (auto& elem : v)
	{
		_array->values.push_back(RzValue(elem));
	}
}

RzValue::RzValue(int v)
{
	m_valueType = VALUE_INT;
	_int = v;

	printf("Returning int as %d\n", _int);

}

RzValue::RzValue(const std::string& v)
{
	m_valueType = VALUE_STRING;
	_string = new AEString();
	_string->ref++;
	_string->str = v;
}

RzValue::RzValue(std::function<void()> fn)
{
	m_valueType = VALUE_FUNCTION;
	_function = new AEFunction();
	_function->tmp = fn;
}

RzValue::RzValue(AEObject* obj)
{
	m_valueType = VALUE_OBJECT;
	_object = obj;
}

RzValue::~RzValue()
{
	//release();
}

RzValue::ValueType RzValue::type() const
{
	return m_valueType;
}

int RzValue::length() const
{
	if (m_valueType == VALUE_ARRAY)
	{
		return _array->values.size();
	}
	else if (m_valueType == VALUE_STRING)
	{
		return _string->str.size();
	}
	else if (m_valueType == VALUE_OBJECT)
	{
		return _object->m_names.size();
	}
	else
		return 0;
}

RzValue RzValue::at(int index)
{
	return _array->values[index];
}

std::string RzValue::typeName() const
{
	if (m_valueType == VALUE_OBJECT)
	{
		return property("typename").str();
	}
	else
		return "";
}

void RzValue::release()
{
	if (m_valueType == VALUE_STRING)
	{
		if (--_string->ref == 0)
			delete _string;
	}

	m_valueType = VALUE_UNDEFINED;
}

void RzValue::setValue(int index, RzValue v)
{
	if (m_valueType == VALUE_ARRAY)
	{
		if (_array->values.size() < index + 1)
			_array->values.resize(index + 1);
		_array->values[index] = v;
	}
}

void RzValue::setValue(const std::string& name, const std::string& value)
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

void RzValue::setValue(const std::string& name, int value)
{
	if (m_valueType != VALUE_OBJECT)
	{
		_object = new AEObject;
		m_valueType = VALUE_OBJECT;
	}

	//printf("%s: %s\n", name.c_str(), value.str().c_str());
	_object->setProperty(name, RzValue(value));
}

void RzValue::setValue(const std::string& name, const RzValue& value)
{
	if (m_valueType != VALUE_OBJECT)
	{
		_object = new AEObject;
		m_valueType = VALUE_OBJECT;
	}

	printf("%s: %s\n", name.c_str(), value.str().c_str());
	_object->setProperty(name, (RzValue)value);
}

RzValue RzValue::property(const std::string& name) const
{
	if (m_valueType != VALUE_OBJECT)
		return RzValue();

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

	return RzValue();
}

RzValue RzValue::property(int index)
{
	return _object->m_properties[index];
}

std::string RzValue::propertyName(int index)
{
	return _object->propertyName(index);
}

int RzValue::numProperties() const
{
	if (m_valueType == VALUE_OBJECT)
	{
		return _object->count();
	}

	return 0;
}

void RzValue::call()
{
	if (m_valueType == VALUE_FUNCTION)
	{
		_function->tmp();
	}
}

void RzValue::call(const AEValueList& argumentList)
{

}

std::string RzValue::str() const
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

const char* RzValue::c_str() const
{
	return str().c_str();
}

bool RzValue::isCallable()
{
	return m_valueType == VALUE_FUNCTION;
}

bool RzValue::isUndefined()
{
	return m_valueType == VALUE_UNDEFINED;
}

bool RzValue::isString()
{
	return m_valueType == VALUE_STRING;
}

int RzValue::toInteger()
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

std::string RzValue::toString() const
{
	return str();
}

bool RzValue::toBoolean() const
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

RzValue RzValue::makeArray()
{
	RzValue v;
	v.m_valueType = VALUE_ARRAY;
	v._array = new AEArray;
	return v;
}

AEValueRef RzValue::makeRef()
{
	AEValueRef ref;
	if (m_valueType == VALUE_INT)
	{
		ref.data = &_int;
		ref.type = AEValueRef::REF_RAW;
	}
	return ref;
}

AEValueRef RzValue::makeRefForChild(const std::string& name)
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
		_object->setProperty(name, RzValue());
		ref.value = _object->getValueRef(name);
		ref.type = AEValueRef::REF_VALUE;
	}
	
	return ref;
}

RzValue& RzValue::operator=(const RzValue& v)
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

RzValue& RzValue::operator=(const std::string& v)
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

RzValue& RzValue::operator=(int v)
{
	m_valueType = VALUE_INT;
	_int = v;
	return *this;
}

RzValue& RzValue::operator=(float v)
{
	_real = v;
	return *this;
}

RzValue RzValue::operator+(const RzValue& b)
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
		return RzValue();
}

RzValue RzValue::operator-(const RzValue& b)
{
	return RzValue();
}

RzValue RzValue::operator*(const RzValue& b)
{
	return RzValue();
}

RzValue RzValue::operator/(const RzValue& b)
{
	return RzValue();
}

bool RzValue::operator==(const RzValue& b)
{
	return false;
}

RzValue RzValue::operator+(AEArray& v)
{
	RzValue ret;
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
		return RzValue();
}

RzValue RzValue::operator+(AEString& v)
{
	return RzValue();
}

RzValue RzValue::operator+(int32_t v)
{
	RzValue r;
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

RzValue::operator bool()
{
	return toBoolean();
}

void RzValue::setFromArray(RzValue v)
{

}

void RzValue::setFromString(RzValue v) 
{

}

void RzValue::setFromObject(void* ptr, AEType* typeInfo)
{
	_object = new AEObject();
	_object->m_obj = ptr;
	_object->m_type = typeInfo;
	m_valueType = VALUE_OBJECT;
}
