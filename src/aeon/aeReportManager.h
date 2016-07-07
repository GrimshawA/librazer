#ifndef aeReportManager_h__
#define aeReportManager_h__

#include <AEON/Runtime/AEByteCode.h>
#include <AEON/Runtime/AEVm.h>

#include <vector>

class aeReportLog
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

class aeReportManager
{
public:
	std::vector<aeReportLog> logs;
	bool                     m_enableStdOut;

public:

	aeReportManager();

	void emitLog(const std::string& message);
	void emitCompilerError(const std::string& message);

	void addEntry(const aeReportLog& log);
};

#endif // aeClosure_h__
