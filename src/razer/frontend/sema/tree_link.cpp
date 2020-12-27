#include "tree_link.h"
#include <razer/runtime/RzEngine.h>
#include <razer/frontend/compiler/Errors.h>
#include <razer/utils/Logger.h>
#include <razer/frontend/AST/aeNodeExpr.h>
#include <razer/frontend/AST/aeNodeIdentifier.h>
#include <razer/frontend/AST/aeNodeBinaryOperator.h>
#include <razer/frontend/AST/aeNodeFunctionCall.h>
#include <algorithm>

namespace sema
{

void tree_link::link(aeNodeFunction& func)
{
    link_block(*func.m_block);
}

void tree_link::link_block(aeNodeBlock& blk)
{
    for (auto& stmt : blk.m_items)
    {
        switch (stmt->m_nodeType)
        {
            case AEN_FUNCTIONCALL: {
                auto call = static_cast<RzCallNode*>(stmt);

                if (!call->getFunction())
                {
                    auto name = call->m_name;

                    // TODO go to the type indexer properly
                    // algo is to pickup names in hash tables in O(1)
                    // as they come up in source, respecting all precedences

                    auto std = e->getModule("std");

                    call->m_fn = std->getFunction(name);
                    // now linked..

                    if (!call->m_fn)
                    {
                        RZLOG("Undeclared function %s\n", name.c_str());
                    }
                }

                break;
            }
        }
    }
}

}
