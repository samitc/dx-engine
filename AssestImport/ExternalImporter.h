#ifndef _EXTERNALIMPORTER_H_
#define _EXTERNALIMPORTER_H_
#include "Command.h"
#include <string>
#include "Engine.h"
class ExternalImporter :public Command
{
public:
	ExternalImporter() = default;
	~ExternalImporter();
	bool checkIsMe(const char* command) override;
	void parseFileName(const char* filename) override;
	int execute() override;
private:
private:
	std::string fileName;
};
#endif