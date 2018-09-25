#ifndef _BASELOG_H_
#define _BASELOG_H_
#include <Logger.h>
#include "Engine.h"
//the logger that used by the engine
class BaseLog
{
public:
	//get the engine logger
	//parameters:
	//	msg:the text to write to the log
	static BaseLog& getInstance();
	~BaseLog() = default;
	//print to log in debug level
	//parameters:
	//	msg:the text to write to the log
	void debug(const char *msg);
	//print to log in fatal level
	//parameters:
	//	msg:the text to write to the log
	void fatal(const char* msg);
	//print to log in error level
	//parameters:
	//	msg:the text to write to the log
	void error(const char* msg);
	//print to log in infog level
	//parameters:
	//	msg:the text to write to the log
	void info(const char* msg);
	//print to log in warn level
	//parameters:
	//	msg:the text to write to the log
	void warn(const char* msg);
	//print to log in trace level
	//parameters:
	//	msg:the text to write to the log
	void trace(const char* msg);
private:
	BaseLog();
	Sys::Logging::Logger log;
	static CONSTEXPRN int CHACH_SIZE = 1024;
	wchar_t msgChach[CHACH_SIZE];
};
#endif