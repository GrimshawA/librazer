#include "program_options.hpp"

namespace rz {

int options_results::count(const std::string& opt) const
{
    int c = 0;
    for (int i = 0; i < m_rawArgs.size(); ++i)
    {
        if (m_rawArgs[i] == opt)
            c++;
    }
    return c;
}

std::vector<std::string> options_results::getPositionalArgs()
{
    std::vector<std::string> args;

    for (int i = 0; i < m_rawArgs.size(); ++i)
    {
        if (m_rawArgs[i][0] != '-')
        {
            args.push_back(m_rawArgs[i]);
        }
    }
    return args;
}

}
