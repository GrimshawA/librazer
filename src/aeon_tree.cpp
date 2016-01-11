#include "aeon_tree.h"

	float ast_expr::as_float()
	{
		return static_cast<ast_floatexpr*>(this)->value;
	}

	std::string ast_expr::as_string()
	{
		return static_cast<ast_stringexpr*>(this)->value;
	}

int ast_expr::as_int()
{
	return static_cast<ast_intexpr*>(this)->value;
}