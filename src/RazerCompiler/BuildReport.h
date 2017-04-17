#ifndef RZBUILDREPORT_H__
#define RZBUILDREPORT_H__

#include <RazerVM/InstructionSet.h>
#include <RazerVM/VirtualMachine.h>

#include <vector>

/*
 * A report represents an error, warnings or other information the compiler outputs to the user
 */
class RzReport
{
public:

    enum LogType
    {
        Debug,
        Warning,
        Error,
        Fatal
    };

    LogType m_type;
    int     m_severity;
    int     m_code;
    int     m_timeStamp;
    std::string m_tag;
    std::string m_message;

public:

    std::string buildString() const
    {
        return m_message;
    }
};

/**
    \class RzBuildReport
    \brief Accumulates errors and warnings generated by compilation
*/
class RzBuildReport
{
public:

    RzBuildReport();

    void deliverReport(const RzReport& report);

private:
    bool                     m_treatWarningsAsErrors;
    bool                     m_enableStdOut;
    std::vector<RzReport>    m_reports;
};

#endif // RZBUILDREPORT_H__