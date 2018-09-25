#ifndef _ASSIMPT_H_
#define _ASSIMPT_H_
#include "Importer.h"
#include <Importer.hpp>
#include "File.h"
#include "ModelData.h"
struct aiScene;
class AssimpT :public Importer
{
public:
	AssimpT(const File &file);
	~AssimpT() = default;
	bool createData() override;
	void preapereWriteMemory() override;
	DWORD getSize() const override;
	bool writeData(void* loc) const override;
private:
	File f;
	Assimp::Importer import;
	const aiScene* scene;
	ModelData data;
	DWORD bufSize;
};
#endif