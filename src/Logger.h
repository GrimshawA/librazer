#ifndef Logger_h__
#define Logger_h__

#define LOGGING_ENABLED 0

#include <string>

class RzLogger
{
public:
	
	static void log(const std::string& msg);

	static std::string format(const char* str, ...);
};

#define RZLOG(msg, ...) RzLogger::log(RzLogger::format(msg, __VA_ARGS__));

#endif // Logger_h__
