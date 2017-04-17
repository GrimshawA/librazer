#ifndef Logger_h__
#define Logger_h__

#include <string>

class RzLogger
{
public:
	
	static void log(const std::string& msg);
    static void logToFile(const std::string& file, const std::string& content);
    static void exception(const std::string& msg);

	static std::string format(const char* str, ...);
};

#define RZLOG(msg, ...) RzLogger::log(RzLogger::format(msg, __VA_ARGS__));
#define RZEXCEPTION(msg, ...) RzLogger::exception(RzLogger::format(msg, __VA_ARGS__));

#endif // Logger_h__
