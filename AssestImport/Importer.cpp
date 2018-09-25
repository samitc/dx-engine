#include "Importer.h"
#include "AssimpT.h"
Importer::~Importer()
{
}
Importer * Importer::createImporter(const char * fileName)
{
	return new AssimpT(File::create(fileName));
}