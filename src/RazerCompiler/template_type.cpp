#include "template_type.hpp"

TemplateType::MetaType TemplateType::metaType() const
{
    return m_type;
}

RzType* TemplateType::specialize(const std::vector<RzQualType> types) const
{
    RzType* ptr = nullptr;

    return ptr;
}
