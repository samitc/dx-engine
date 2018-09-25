#ifndef _MODELIMPORTER_H_
#define _MODELIMPORTER_H_
#include <vector>
#include <memory>
#include "ExternalImporter.h"
#include "Engine.h"
class File;
class ModelData;
struct VertexData;
struct TextData;
struct NormalData;
struct SubSet;
using IndexType = unsigned int;
const IndexType FACE_DEFUALT = -1;
class ModelImporter :public ExternalImporter
{
public:
	ModelImporter(const File &file);
	virtual ~ModelImporter();
	virtual bool writeData(ModelData& model) const = 0;
	IndexType getModelImportSize() const;
protected:
	struct FaceData
	{
	public:
		FaceData();
		FaceData(unsigned int vertexIndex, unsigned int textureIndex, unsigned int normalIndex);
		IndexType vertexIndex;
		IndexType textureIndex;
		IndexType normalIndex;
	};
protected://for engine
	std::vector<VertexData> vertData;
	std::vector<TextData> texturesData;
	std::vector<NormalData> normaldata;
	std::vector<FaceData> faceData;
	std::vector<SubSet> subsets;
	std::unique_ptr<File> file;
private:
};
#endif