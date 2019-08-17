#ifndef SEMA_DEPENDENCY_ANALYSIS_HPP_
#define SEMA_DEPENDENCY_ANALYSIS_HPP_

#include <vector>
#include <functional>
#include <unordered_map>

class aeNodeExpr;

namespace sema
{
    /*
     * A bound element is an identity that is either a concrete target or source of a binding expression
     **/
    struct bound_element
    {
        std::string name;
        std::vector<bound_element*> dependencies;
    };

    struct bound_element_tracker
    {
        bound_element* get(const std::string& name)
        {
            if (elements.find(name) == elements.end())
            {
                bound_element* e = new bound_element;
                e->name = name;
                elements[name] = e;
                return e;
            }

            return elements[name];
        }

        std::unordered_map<std::string, bound_element*> elements;
    };

    void visit(bound_element* e, const std::function<bool(bound_element*)>& callback);
    bool detect_circular_references(bound_element* from);

    void ast_visit_identities(aeNodeExpr* root, const std::function<void(aeNodeExpr*)>& callback);
}

#endif // SEMA_DEPENDENCY_ANALYSIS_HPP_
