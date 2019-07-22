#ifndef RZTEMPLATE_TYPE_HPP_
#define RZTEMPLATE_TYPE_HPP_

#include <razer/runtime/RzType.h>

/**
 * @brief The TemplateType class
 *
 * Defines a template type which can be specialized with other types
 * into an actual usable type
 */
class TemplateType
{
public:

    enum MetaType
    {
        Template,
        Generic
    };

public:

    MetaType metaType() const;

    /// Generate a new usable type out of the template
    RzType* specialize(const std::vector<RzQualType> types) const;

private:
    MetaType m_type {Template};
};

#endif // RZTEMPLATE_TYPE_HPP_
