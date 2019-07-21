#include <RazerCompiler/BuildReport.h>
#include <razer/utils/Logger.h>

RzBuildReport::RzBuildReport()
: m_enableStdOut(true)
, m_treatWarningsAsErrors(false)
{

}

void RzBuildReport::deliverReport(const RzReport& report) {
    m_reports.push_back(report);
    RzReport& rep = m_reports.back();

    if (rep.m_type == RzReport::Warning && m_treatWarningsAsErrors) {
        rep.m_type = RzReport::Error;
    }

    if (m_enableStdOut) {
        if (rep.m_type == RzReport::Warning) {
            RZLOG("warning: %s", rep.m_message.c_str());
        }
        else if (rep.m_type == RzReport::Error) {
            RZLOG("error: %s", rep.m_message.c_str());
        }
    }
}
