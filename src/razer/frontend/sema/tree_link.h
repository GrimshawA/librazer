#ifndef SEMA_TREELINK_HPP_
#define SEMA_TREELINK_HPP_

#include <vector>
#include <functional>
#include <unordered_map>

#include <razer/frontend/AST/Nodes.h>

class aeNodeExpr;

namespace sema
{

/**
 * @brief The tree_link class
 *
 * Go through the ast and link things to types etc
 *
 * Linking can be rewrites of the tree or just patching up fields
 */
class tree_link
{
public:
    //explicit tree_link();

    void link(aeNodeFunction& func);
    void link_block(aeNodeBlock& blk);

public:
    RzEngine* e = nullptr;
};

}

#endif // SEMA_TREELINK_HPP_
