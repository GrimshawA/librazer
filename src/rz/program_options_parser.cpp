#include "program_options_parser.hpp"

namespace rz {

options_results parse_cmd_args(const program_options& opts, int argc, char** argv)
{
    options_results res;
    for (int i = 1; i < argc; ++i)
    {
        res.m_rawArgs.push_back(argv[i]);
    }
    return res;
}

}
