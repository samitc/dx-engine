#ifndef _EXPORTER_H_
#define _EXPORTER_H_
#include "Command.h"
#include <String>
class Importer;
class Exporter : public Command
{
public:
	Exporter() = default;
	~Exporter()=default;
	bool checkIsMe(const char* command) override;
	void parseFileName(const char* filename) override;
	void parseArguments(const char* arguments) override;
	int execute() override;
	static unsigned char *save(const Importer*, int *);
private:
	std::string fileName;
	std::string resourceName;
};
#endif // !_EXPORTER_H_
