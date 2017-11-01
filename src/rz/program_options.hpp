#ifndef RZPROGRAMOPTIONS_HPP_
#define RZPROGRAMOPTIONS_HPP_

#include <vector>
#include <string>

namespace rz {

class program_options
{
public:

    struct Option
    {
       std::string name;
       std::string flag;
    };

    void addOption(const std::string& name, const std::string& flag)
    {

    }

private:
    std::vector<Option>      m_flags;
};

class options_results
{
public:
    /// Check for the existence of a given option and its count of ocurrences
    int count(const std::string& opt) const;

    std::vector<std::string> getPositionalArgs();

    std::vector<std::string> m_rawArgs;
};

}
#endif // RZPROGRAMOPTIONS_HPP_
