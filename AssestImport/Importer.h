#ifndef _IMPORTER_H_
#define _IMPORTER_H_
#include <Windows.h>
class Importer
{
public:
	Importer() = default;
	virtual ~Importer();
	virtual bool createData() = 0;
	virtual void preapereWriteMemory() = 0;
	virtual DWORD getSize() const = 0;
	virtual bool writeData(void* loc) const = 0;
	static Importer* createImporter(const char * fileName);
private:

};
#endif