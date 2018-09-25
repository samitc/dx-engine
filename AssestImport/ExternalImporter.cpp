#include "ExternalImporter.h"
#include "AssimpT.h"
#include "File.h"
static CONSTEXPRN char FOLDER_SEPERATE = '\\';
static CONSTEXPRN char FILE_EXTENSION = '.';
ExternalImporter::~ExternalImporter()
{
}
bool ExternalImporter::checkIsMe(const char * command)
{
	return strcmp("ImportExternal", command) == 0;
}
void ExternalImporter::parseFileName(const char * filename)
{
	fileName = filename;
}
int ExternalImporter::execute()
{
	const char* command = "ImportExternal";
	Importer *importer = Importer::createImporter(fileName.c_str());
	importer->createData();
	importer->preapereWriteMemory();
	char* eventName = (char*)alloca(fileName.size()+1);
	char* resEve = (char*)alloca(fileName.size() + 2);
	char* FileMapName = (char*)alloca(strlen(command) + fileName.size() + 1);
	strcpy(FileMapName, command);
	char *p = FileMapName + strlen(command);
	char *p1 = eventName;
	char *p2 = resEve;
	const char *c = fileName.c_str();
	while (*c != 0)
	{
		if (*c != FOLDER_SEPERATE)
		{
			*p = *c;
			*p1 = *c;
			*p2 = *c;
			p++;
			p1++;
			p2++;
		}
		c++;
	}
	*p = 0;
	*p1 = 0;
	*p2 = 'o';
	*(p2 + 1) = 0;
	HANDLE eve = OpenEventA(EVENT_MODIFY_STATE, false, eventName);
	HANDLE reseve = OpenEventA(SYNCHRONIZE, false, resEve);
	HANDLE sharedMemory = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, importer->getSize(), FileMapName);
	if (sharedMemory == NULL)
	{
		return 1;
	}
	void *buf = MapViewOfFile(sharedMemory, FILE_MAP_WRITE, 0, 0, importer->getSize());
	if (buf == NULL)
	{
		CloseHandle(sharedMemory);
		return 1;
	}
	importer->writeData(buf);
	UnmapViewOfFile(buf);
	SetEvent(eve);
	WaitForSingleObject(reseve, INFINITE);
	CloseHandle(sharedMemory);
	CloseHandle(reseve);
	CloseHandle(eve);
	delete importer;
	return 0;
}
bool compareStr(const char* st1, const char* st2)
{
	while (*st1 == ((*st2) | 0x20) && *st1 != 0)
	{
		++st1;
		++st2;
	}
	return *st1 == *st2;
}