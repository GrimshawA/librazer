#include <razer/runtime/RzType.h>
#include <razer/vm/VirtualMachine.h>
#include <razer/utils/Logger.h>

RzType::RzType(RzModule& module, const std::string& _name, uint32_t _size)
    : m_module(module)
    , is_native(false)
    , m_userData(nullptr)
{
    m_name = _name;
    m_size = _size;
}

void* RzType::construct()
{
    if (is_native)
        return constructNative();
    else
        return constructScript();
}

void* RzType::constructNative()
{
    for (auto& method : m_methods)
    {
//        if (method.name == m_name)
//        {
//            void* mem = malloc(m_size);
//            if (is_templated) {
//                method.templatedConstructor(mem, nullptr, m_size);
//            }
//            else {
//                method.constructorCallback(mem, nullptr);
//            }

//            return mem;
//        }
    }

    printf("constructNative() should never fail! Missing factory method\n");
    return nullptr;
}

void* RzType::constructScript()
{
    // Script defined types have no native type to instance, they are just a chunk of memory big enough
    // to accommodate all data.
    printf("Allocated new %s with size %d\n", m_name.c_str(), getSize());
    void* memory = malloc(getSize());
    return memory;
}

uint64_t RzType::getConstructorIndex()
{
	return getModule()->getFunctionIndex(getName() + "." + getName());
}

void RzType::computeMetrics() {
    if (m_fields.empty())
    {
        m_size = 1;
        return;
    }

    m_size = 0;
    for (std::size_t i = 0; i < m_fields.size(); ++i) {
        m_size += m_fields[i].type.getSize();
    }
}

bool RzType::isPrimitive() const {
    return m_flags.test(FLAG_PRIMITIVE);
}

bool RzType::isEnum()
{
    return m_typeKind == KindEnum;
}

bool RzType::isTemplated()
{
    return !m_templateParams.empty();
}

bool RzType::isClass()
{
    return m_typeKind == KindClass;
}

bool RzType::isNative()
{
    return is_native;
}

bool RzType::isPod()
{
    for (auto& field : m_fields)
    {
        if (!field.type.getType()->isPod())
            return false;
    }

    return m_pod;
}

uint32_t RzType::getSize()
{
    return m_size;
}

uint32_t RzType::getNumMethods()
{
    return m_methods.size();
}

uint32_t RzType::getNumFields()
{
    return m_fields.size();
}

uint32_t RzType::getNumProtocols()
{
    return 0;
}

RzFunction* RzType::getMethod(const std::string& methodName) const
{
    for (int i = 0; i < m_methods.size(); ++i)
    {
        if (m_methods[i]->m_absoluteName == methodName)
            return m_methods[i];
    }

    return nullptr;
}

std::string RzType::getName()
{
    return m_name;
}

int RzType::getFunctionId(const std::string& name)
{
    return m_module.getFunctionIndexByName(m_name + "." + name);
}

RzFunction* RzType::getFunction(const std::string& name)
{
    return m_module.getFunction(m_name + "." + name);
}

aeBindMethod RzType::getNativeFunction(const std::string& name)
{
    for (auto& m : m_methods)
    {
        if (m->m_absoluteName == name)
            return m->methodCallback;
    }
    return nullptr;
}

RzFunction* RzType::selectMethod(const std::string& name, const std::vector<RzQualType>& argsList) {
    for(auto& m : m_methods) {
        if (m->m_name == name) {
            return m;
        }
    }

    return nullptr;
}

RzModule* RzType::getModule() {
    return &m_module;
}

void RzType::setFlag(TypeFlag flag) {
    m_flags.set(flag);
}

bool RzType::isScriptSide()
{
    return true;
}

bool RzType::isHostSide()
{
    return true;
}

bool RzType::isHybrid()
{
    return true;
}

aeField* RzType::getField(std::string name)
{
    for (auto& field : m_fields)
    {
        if (field.name == name)
        {
            return &field;
        }
    }

    return nullptr;
}

void RzType::createField(aeField fieldInfo)
{
    if (m_fields.size() == 0)
        m_size = 0;

    fieldInfo.offset = getSize();
    m_fields.push_back(fieldInfo);

    m_size += fieldInfo.size;
}

void RzType::registerField(const std::string& name, int offset)
{
    aeField fieldInfo;
    fieldInfo.name = name;
    fieldInfo.offset = offset;
    m_fields.push_back(fieldInfo);
}

void RzType::registerEnum(const std::string& name)
{
    EnumInfo enumInfo;
    enumInfo.name = name;
    m_enums.push_back(enumInfo);
}

void RzType::registerEnumProperty(const std::string& name, const std::string& property, int value)
{
    for (auto& enumm : m_enums)
    {
        if (enumm.name == name)
        {
            EnumEntryInfo enumEntryInfo;
            enumEntryInfo.name = property;
            enumEntryInfo.value = value;
            enumm.entries.push_back(enumEntryInfo);
            return;
        }
    }
    // throw api error
}
