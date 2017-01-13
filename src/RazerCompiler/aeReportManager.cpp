#include "aeReportManager.h"

aeReportManager::aeReportManager()
: m_enableStdOut(true)
{

}

void aeReportManager::emitLog(const std::string& message)
{
	aeReportLog entry;
	entry.m_message = message;
	addEntry(entry);
}

void aeReportManager::emitCompilerError(const std::string& message)
{
	aeReportLog entry;
	entry.m_message = message;
	entry.m_type = aeReportLog::Error;
	addEntry(entry);
}

void aeReportManager::addEntry(const aeReportLog& log)
{
	if (m_enableStdOut)
	{
		printf("%s\n", log.buildString().c_str());
	}
	logs.push_back(log);
}