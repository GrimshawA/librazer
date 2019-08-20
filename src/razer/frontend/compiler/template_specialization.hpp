#ifndef RZTEMPLATE_TYPE_HPP_
#define RZTEMPLATE_TYPE_HPP_

#include <razer/runtime/RzType.h>

class AEStructNode;
class RzTemplateNode;

/**
 * @brief The TemplateSpecialization class
 *
 */
class TemplateSpecialization
{
public:

    /// Specialize any code entity which is templated
    /// into a concrete implementation for a set of type parameters
    AEStructNode* specialize(RzTemplateNode* node, RzQualType t);
};

#endif // RZTEMPLATE_TYPE_HPP_
