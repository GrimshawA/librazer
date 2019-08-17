#include "dependency_analysis.h"
#include <razer/frontend/AST/aeNodeExpr.h>
#include <razer/frontend/AST/aeNodeIdentifier.h>
#include <razer/frontend/AST/aeNodeBinaryOperator.h>
#include <algorithm>

namespace sema
{

void visit(bound_element* e, const std::function<bool(bound_element*)>& callback)
{
    for (auto& x : e->dependencies)
    {
        if (!callback(x))
            return;

        visit (x, callback);
    }
}

bool detect_circular_references(bound_element* from)
{
    std::vector<bound_element*> visited;
    visited.push_back(from);

    bool result = false;

    visit(from, [&](bound_element* child) -> bool
    {
        if (std::find(visited.begin(), visited.end(), child) != visited.end())
        {
            result = true;
            return false;
        }

        visited.push_back(child);
        return true;
    });

    return result;
}

void ast_visit_identities(aeNodeExpr* root, const std::function<void(aeNodeExpr*)>& callback)
{
    if (root->m_nodeType == AEN_IDENTIFIER)
    {
        callback(root);
        return;
    }

    if (root->m_nodeType == AEN_BINARYOP)
    {
        auto* binop = static_cast<aeNodeBinaryOperator*>(root);
        ast_visit_identities(binop->operandA, callback);
        ast_visit_identities(binop->operandB, callback);
    }
}

}
