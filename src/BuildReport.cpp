#include <BuildReport.h>
#include <Logger.h>

RzBuildReport::RzBuildReport()
: m_enableStdOut(true)
{

}

void RzBuildReport::emitLog(const std::string& message)
{
	aeReportLog entry;
	entry.m_message = message;
	addEntry(entry);
}

void RzBuildReport::emitCompilerError(const std::string& message)
{
	aeReportLog entry;
	entry.m_message = message;
	entry.m_type = aeReportLog::Error;
	addEntry(entry);
}

void RzBuildReport::addEntry(const aeReportLog& log)
{
	if (m_enableStdOut)
	{
		printf("%s\n", log.buildString().c_str());
	}
	logs.push_back(log);
}