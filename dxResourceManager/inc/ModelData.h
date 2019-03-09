#ifndef _MODELDATA_H_
#define _MODELDATA_H_
#include<Windows.h>
#include <vector>
#include "Engine.h"
#include "SubSet.h"
struct Float3
{
	float x, y, z;
};
struct Float4
{
	float x, y, z, w;
};
struct Float4x4
{
	float val[4][4];
	Float4x4 operator*(const Float4x4 &f) const;
	Float4x4 transpose() const;
};
struct VertexData
{
public:
	VertexData();
	VertexData(float x, float y, float z, float w);
	float x;
	float y;
	float z;
	float w;
};
struct TextData
{
public:
	TextData();
	TextData(float u, float v, float w);
	float u;
	float v;
	float w;
};
struct NormalData
{
public:
	NormalData();
	NormalData(float x, float y, float z);
	float x;
	float y;
	float z;
};
struct Node
{
	int parentNode;
	Float4x4 transformation;
	int modelIndex;
	int boneModelIndex;
};
struct ModelDat
{
	struct BoneDat
	{
		struct BoneWeight
		{
			unsigned int boneIndex;
			float weight;
		};
		std::vector<BoneWeight> weights;
	};
	struct MaterialDat
	{
		std::vector<std::string> diffTextName;
	};
	struct Bone
	{
		struct Frame
		{
			float time;
			Float3 transform;
			Float3 scale;
			Float4 quat;
		};
		int parentBone;
		Float4x4 boneTransformation;
		Float4x4 offsetMatrix;
		std::vector<std::vector<Frame>> frames;
	};
	std::vector<VertexData> vertex;
	std::vector<DWORD> index;
	std::vector<TextData> texture;
	std::vector<NormalData> normal;
	std::vector<BoneDat> bonesDat;
	std::vector<int> matirialIndexPerSubset;
	std::vector<MaterialDat> materials;
	std::vector<SubSet> subset;
	std::vector<Bone> bones;
	std::vector<Node> nodes;
	std::vector<std::string> animationsNames;
};
class ModelData
{
public:
	ModelData() = default;
	~ModelData() = default;
	ModelData(ModelData&&) = default;
	ModelData(const ModelData&) = default;
	ModelData& operator=(ModelData&&) = default;
	using DATA_COUNT = unsigned long;
	const std::vector<ModelDat> &getModels() const;
	const std::vector<Node> &getNodes()const;
	void setNodes(std::vector<Node> &&);
	void setModels(std::vector<ModelDat> &&);
	void writeToBuffer(void *) const;
	void readFromBuffer(void *);
public://for engine
private:
	std::vector<ModelDat> models;
	std::vector<Node> nodes;
};
#endif