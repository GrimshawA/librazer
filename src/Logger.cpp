#include <Logger.h>
#include <stdarg.h>

void RzLogger::log(const std::string& msg)
{
#if defined LOGGING_ENABLED && LOGGING_ENABLED == 1
	printf("%s", msg.c_str());
#endif
}

std::string RzLogger::format(const char* str, ...)
{
	char buffer[2560];
	va_list args;
	va_start(args, str);
	vsprintf(buffer, str, args);
	va_end(args);
	return buffer;
}