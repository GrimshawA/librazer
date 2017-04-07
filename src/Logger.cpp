#include <Logger.h>
#include <stdarg.h>

void RzLogger::log(const std::string& msg)
{
#if defined LOGGING_ENABLED
	printf("%s", msg.c_str());
#endif
}

void RzLogger::exception(const std::string& msg)
{
    printf("RUNTIME EXCEPTION: %s\n", msg.c_str());
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