#ifndef _COMMAND_H_
#define _COMMAND_H_
class Command
{
public:
	virtual bool checkIsMe(const char* command) = 0;
	virtual void parseFileName(const char* filename) = 0;
	virtual void parseArguments(const char* arguments) {};
	virtual int execute() = 0;
};
#endif