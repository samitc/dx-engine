#include <Windows.h>
#include <malloc.h>
#include "Engine.h"
#include "Command.h"
#include "ExternalImporter.h"
#include "Exporter.h"
CONSTEXPR char* ERRORS_NAME[] = { "command error","file name error","command not found error","shared memory error","map view error","last error" };
CONSTEXPR int ERRORS[] = { 1,2,3,1000,2000,100 };
CONSTEXPR bool strCmp(const char* c1, const char *c2)
{
	while (*c1 != 0 && *c2 != 0)
	{
		if (*c1 != *c2)
		{
			return false;
		}
		++c1;
		++c2;
	}
	return *c1 == *c2;
}
CONSTEXPR int findErrorCode(const char *error)
{
	CONSTEXPR size_t arraySize = ARRAYSIZE(ERRORS_NAME);
	for (CONSTEXPR size_t i = 0; i < arraySize; i++)
	{
		if (strCmp(error,ERRORS_NAME[i]))
		{
			return ERRORS[i];
		}
	}
	return ERRORS[arraySize - 1];
}
int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmd, int show)
{
	const int COMMAND_SIZE = 2;
	Command *commands[COMMAND_SIZE];
	commands[0] = new Exporter();
	commands[1] = new ExternalImporter();
	char* cmdS = cmd, *cmdP;
	int totalSize = 0;
	while (*cmdS != 0 && *cmdS != ' ')
	{
		cmdS++;
	}
	if (*cmdS == 0)
	{
		return findErrorCode("command error");
	}
	int l = cmdS - cmd;
	totalSize += l;
	l++;
	char *command = (char*)alloca(l);
	strncpy(command, cmd, l);
	command[l - 1] = 0;
	Command *coman=nullptr;
	for (size_t i = 0; i < COMMAND_SIZE; i++)
	{
		if (commands[i]->checkIsMe(command))
		{
			coman = commands[i];
			break;
		}
	}
	if (coman==nullptr)
	{
		return findErrorCode("command not found error");
	}
	cmdP = ++cmdS;
	char seperator;
	if (*cmdP == '"')
	{
		++cmdS;
		seperator = '"';
	}
	else
	{
		seperator = ' ';
	}
	++cmdP;
	while (*cmdP != 0 && *cmdP != seperator)
	{
		cmdP++;
	}
	if (*cmdP == 0 && seperator == '"')
	{
		return findErrorCode("file name error");
	}
	l = cmdP - cmdS;
	totalSize += l;
	++l;
	char* fileName = (char*)alloca(l);
	strncpy(fileName, cmdS, l);
	fileName[l - 1] = 0;
	coman->parseFileName(fileName);
	coman->parseArguments(cmdP + 1);
	int retCode = coman->execute();
	for (size_t i = 0; i < COMMAND_SIZE; i++)
	{
		delete commands[i];
	}
	return retCode;
}