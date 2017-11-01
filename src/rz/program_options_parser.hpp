#ifndef RZ_CMD_PARSER_HPP_
#define RZ_CMD_PARSER_HPP_

#include "program_options.hpp"

namespace rz
{

options_results parse_cmd_args(const program_options& opts, int argc, char** argv);

};

#endif // RZ_CMD_PARSER_HPP_
