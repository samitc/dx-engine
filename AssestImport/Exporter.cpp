#include "Exporter.h"
#include "Importer.h"
#include "FileData.h"
#include "DataManager.h"
bool Exporter::checkIsMe(const char * command)
{
	return strcmp("Export", command) == 0;
}
void Exporter::parseFileName(const char * filename)
{
	fileName = filename;
}
void Exporter::parseArguments(const char * arguments)
{
	resourceName = arguments;
}
int Exporter::execute()
{
	Importer *importer = Importer::createImporter(fileName.c_str());
	importer->createData();
	importer->preapereWriteMemory();
	DataManager dat;
	dat.saveStatic<Exporter, Importer>(resourceName.c_str(), importer);
	delete importer;
	return 0;
}

unsigned char *Exporter::save(const Importer *importer, int *size)
{
	*size = importer->getSize() + sizeof(int);
	byte *data = new byte[*size];
	((int*)data)[0] = 1;
	importer->writeData(data + sizeof(int));
	return data;
}
