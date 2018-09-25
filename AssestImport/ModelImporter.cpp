#include "ModelImporter.h"
#include "File.h"
#include "SubSet.h"
#include "ModelData.h"
ModelImporter::ModelImporter(const File & file) :file(new File(file))
{
}
ModelImporter::~ModelImporter()
{
}
template<class T, class S> int getUseSize(T &dat, S defaultVal)
{
	int count = 0;
	S* s = (S*)&dat;
	while (count<sizeof T/sizeof S&&*s!=defaultVal)
	{
		++count;
		++s;
	}
	return count*sizeof S;
}
IndexType ModelImporter::getModelImportSize() const
{
	return vertData.size()*getUseSize(vertData[0], std::numeric_limits<float>::infinity()) + texturesData.size()*getUseSize(texturesData[0], std::numeric_limits<float>::infinity())
		+ normaldata.size()*getUseSize(normaldata[0], std::numeric_limits<float>::infinity()) + (subsets.size() > 1 ? subsets.size()*sizeof SubSet : 0) + 
		9 * sizeof ModelData::DATA_COUNT;//{vertex,normal,texture}(count and size),index,subset,texturenames
}
ModelImporter::FaceData::FaceData()
{
}
ModelImporter::FaceData::FaceData(unsigned int vertexIndex, unsigned int textureIndex, unsigned int normalIndex) : vertexIndex(vertexIndex), textureIndex(textureIndex), normalIndex(normalIndex)
{
}