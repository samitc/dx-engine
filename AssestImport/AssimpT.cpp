#include "AssimpT.h"
#include <set>
#include <map>
#include <postprocess.h>
#include <scene.h>
#include <mesh.h>
using DataIndexer = std::vector<unsigned int>;
template<class T> struct KeyFrameVal
{
	float time;
	T value;
};
template<class T, class U> void convertVector(std::vector<KeyFrameVal<T>> &addVec, U* keys, unsigned int num)
{
	addVec.reserve(num);
	for (size_t i = 0; i < num; i++)
	{
		KeyFrameVal<T> add;
		add.time = (float)keys[i].mTime;
		static_assert(sizeof(T) == sizeof(U::mValue), "wrong size");
		memcpy(&add.value, &keys[i].mValue, sizeof(T));
		addVec.push_back(std::move(add));
	}
}
template<class T, class U> void copyData(T &t, const U &u)
{
	static_assert(sizeof(T) == sizeof(U), "wrong size");
	memcpy(&t, &u, sizeof(T));
}
//this method convert the vector because our vector is(x,y,z,w) and assimp vector is(w,x,y,z)
template <> void copyData<Float4, aiQuaternion>(Float4 &t, const aiQuaternion &u)
{
	static_assert(sizeof(Float4) == sizeof(aiQuaternion), "wrong size");
	t.x = u.x;
	t.y = u.y;
	t.z = u.z;
	t.w = u.w;
}
template<class T, class U> void copyData(T *t, const U *u, int numOfItems)
{
	static_assert(sizeof(T) == sizeof(U), "wrong size");
	memcpy(t, u, sizeof(T)*numOfItems);
}
namespace std
{
	template<typename T> T* begin(T* ptr) { return ptr; }
}
template<class T> void insertToVector(std::vector<T> &insertTo, const std::vector<T> &insertFrom)
{
	insertTo.insert(insertTo.end(), insertFrom.begin(), insertFrom.end());
}
template<class T, class Z, class Convert> void insertToVector(std::vector<T> &insertTo, const std::vector<Z> &insertFrom, Convert &convertMethod)
{
	insertTo.reserve(insertTo.size() + insertFrom.size());
	for (auto &temp : insertFrom)
	{
		insertTo.push_back(convertMethod(temp));
	}
}
template<class T> void addToArray(std::vector<T> &vec, T* arr, unsigned int size)
{
	vec.insert(vec.begin(), std::begin(arr), std::begin(arr) + size);
}
template<class T> struct NodeTree
{
	std::vector<NodeTree<T>> child;
	T val;
};
struct AssimpData
{
	struct Node
	{
		std::string name;
		Float4x4 transform;
		DataIndexer meshIndex;
		DataIndexer boneIndex;
		std::vector<DataIndexer> animationIndex;
	};
	enum class PrimitiveType
	{
		POINT = 1,
		LINE = 2,
		TRIANGLE = 4
	};
	struct Mesh
	{
		std::string name;
		PrimitiveType meshType;
		std::vector<VertexData> vertex;
		std::vector<DWORD> index;
		std::vector<TextData> texture;
		std::vector<NormalData> normal;
		unsigned int materialIndex;
		DataIndexer boneIndex;
	};
	struct Material
	{
		std::vector<std::string> diffTextName;
	};
	struct Animation
	{
		std::string name;
		struct Frames
		{
			std::vector<KeyFrameVal<Float3>> transformation;
			std::vector<KeyFrameVal<Float3>> scale;
			std::vector<KeyFrameVal<Float4>> quat;
		};
		std::vector<Frames> frames;
	};
	struct Bone
	{
		std::string name;
		struct Weight
		{
			unsigned int meshId;
			unsigned int vertexId;
			float weight;
		};
		Float4x4 offsetMatrix;
		std::vector<Weight> weights;
	};
	NodeTree<Node> nodes;
	std::vector<Mesh> meshes;
	std::vector<Material> materials;
	std::vector<Bone> bones;
	std::vector<Animation> animations;
};
AssimpT::AssimpT(const File & file) :f(file), scene(nullptr), bufSize(0)
{
}
bool AssimpT::createData()
{
	import.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, aiComponent_COLORS | aiComponent_CAMERAS | aiComponent_LIGHTS);
	scene = import.ReadFile(f.getloc(),  aiProcess_RemoveRedundantMaterials | /*aiProcess_FindDegenerates(Can couse mesh with only 2 index) |*/ aiProcess_RemoveComponent | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_Triangulate | aiProcess_GenUVCoords | aiProcess_TransformUVCoords | aiProcess_FindInstances |/* aiProcess_OptimizeMeshes(can couse to ligel subset to be merged) |*/ aiProcess_OptimizeGraph | aiProcess_ImproveCacheLocality);
	return scene != 0;
}
DWORD AssimpT::getSize() const
{
	return bufSize;
}
void scanNodes(aiNode *node, NodeTree<AssimpData::Node> &nodes)
{
	nodes.val.name = node->mName.C_Str();
	copyData(nodes.val.transform, node->mTransformation);
	nodes.val.transform = nodes.val.transform.transpose();
	addToArray(nodes.val.meshIndex, node->mMeshes, node->mNumMeshes);
	nodes.child.resize(node->mNumChildren);
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		scanNodes(node->mChildren[i], nodes.child[i]);
	}
}
template<class T, class U> void addVertexData(std::vector<T> &vec, U *dat, unsigned int size)
{
	vec.reserve(size);
	for (size_t i = 0; i < size; i++)
	{
		vec.push_back(std::move(T(dat[i].x, dat[i].y, dat[i].z)));
	}
}
template<> void addVertexData<DWORD, aiFace>(std::vector<DWORD> &vec, aiFace *dat, unsigned int size)
{
	vec.reserve(size*dat->mNumIndices);
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < dat[i].mNumIndices; j++)
		{
			vec.push_back((DWORD)dat[i].mIndices[j]);
		}
	}
}
template<class U> void addVertexData(std::vector<VertexData> &vec, U *dat, unsigned int size)
{
	vec.reserve(size);
	for (size_t i = 0; i < size; i++)
	{
		vec.push_back(std::move(VertexData(dat[i].x, dat[i].y, dat[i].z, std::numeric_limits<float>::infinity())));
	}
}
template<class T, class Pr> NodeTree<T>* findInTree(NodeTree<T> &nodes, Pr &pred)
{
	NodeTree<T>* p;
	if (pred(nodes.val))
	{
		return &nodes;
	}
	for (size_t i = 0; i < nodes.child.size(); i++)
	{
		p = findInTree(nodes.child[i], pred);
		if (p != nullptr)
		{
			return p;
		}
	}
	return nullptr;
}
template<class T, class Pr> void findAllInTree(NodeTree<T> &nodes,std::vector<NodeTree<T>*> &resoults, Pr &pred)
{
	if (pred(nodes.val))
	{
		resoults.push_back(&nodes);
	}
	for (size_t i = 0; i < nodes.child.size(); i++)
	{
		findAllInTree(nodes.child[i], resoults, pred);
	}
}
void scanBones(AssimpData::Mesh &mesh, aiBone **bones, unsigned int numOfBones, AssimpData &assimp,int meshId)
{
	auto &nodes = assimp.nodes;
	for (size_t i = 0; i < numOfBones; i++)
	{
		auto bone = bones[i];
		unsigned int boneIndex;
		NodeTree<AssimpData::Node> *node = findInTree(nodes, [bone](auto &node)
		{
			return strcmp(bone->mName.C_Str(), node.name.c_str()) == 0;
		});
		auto boneExsist = std::find_if(assimp.bones.cbegin(), assimp.bones.cend(), [bone](const auto& nBone)
		{
			return strcmp(bone->mName.C_Str(), nBone.name.c_str()) == 0;
		});
		if (boneExsist == assimp.bones.cend())
		{
			boneIndex = assimp.bones.size();
			assimp.bones.resize(boneIndex + 1);
			auto &b = assimp.bones[boneIndex];
			b.name = bone->mName.C_Str();
			copyData(b.offsetMatrix, bone->mOffsetMatrix);
			node->val.boneIndex.push_back(boneIndex);
		}
		else
		{
			boneIndex = boneExsist - assimp.bones.cbegin();
		}
		AssimpData::Bone *aBone = &assimp.bones[boneIndex];
		AssimpData::Bone::Weight w;
		w.meshId = meshId;
		for (size_t i = 0; i < bone->mNumWeights; i++)
		{
			w.vertexId = bone->mWeights[i].mVertexId;
			w.weight = bone->mWeights[i].mWeight;
			aBone->weights.push_back(w);
		}
		mesh.boneIndex.push_back(boneIndex);
	}
}
void loadMeshes(const aiScene *scene, AssimpData &assimpData)
{
	auto &meshes = assimpData.meshes;
	meshes.resize(scene->mNumMeshes);
	for (size_t i = 0; i < scene->mNumMeshes; i++)
	{
		auto mesh = scene->mMeshes[i];
		auto &aMesh = meshes[i];
		aMesh.meshType = (AssimpData::PrimitiveType) (DWORD)mesh->mPrimitiveTypes;
		aMesh.name = mesh->mName.C_Str();
		addVertexData(aMesh.vertex, mesh->mVertices, mesh->mNumVertices);
		if (mesh->HasNormals())
		{
			addVertexData(aMesh.normal, mesh->mNormals, mesh->mNumVertices);
		}
		if (mesh->HasTextureCoords(0))
		{
			addVertexData(aMesh.texture, mesh->mTextureCoords[0], mesh->mNumVertices);
		}
		addVertexData(aMesh.index, mesh->mFaces, mesh->mNumFaces);
		aMesh.materialIndex = mesh->mMaterialIndex;
		scanBones(aMesh, mesh->mBones, mesh->mNumBones, assimpData,i);
	}
}
template<class T, class Z> void addFrame(std::vector<KeyFrameVal<T>> &frames, Z *keys, unsigned int sizeOfKeys, float ticksPerSecond)
{
	frames.resize(sizeOfKeys);
	for (size_t i = 0; i < sizeOfKeys; i++)
	{
		frames[i].time = (float)keys[i].mTime / ticksPerSecond;
		copyData(frames[i].value, keys[i].mValue);
	}
}
void createAnimation(aiNodeAnim ** anim, unsigned int numOfAnim, AssimpData::Animation &animation, float ticksPerSecond, NodeTree<AssimpData::Node> &nodes,unsigned int animNumber)
{
	animation.frames.resize(numOfAnim);
	for (size_t i = 0; i < numOfAnim; i++)
	{
		auto an = anim[i];
		auto &frame = animation.frames[i];
		auto node = findInTree(nodes, [an](auto &node)
		{
			return strcmp(an->mNodeName.C_Str(), node.name.c_str()) == 0;
		});
		if (node->val.animationIndex.size()<= animNumber)
		{
			node->val.animationIndex.resize(animNumber + 1);
		}
		node->val.animationIndex[animNumber].push_back(i);
		addFrame(frame.transformation, an->mPositionKeys, an->mNumPositionKeys, ticksPerSecond);
		addFrame(frame.scale, an->mScalingKeys, an->mNumScalingKeys, ticksPerSecond);
		addFrame(frame.quat, an->mRotationKeys, an->mNumRotationKeys, ticksPerSecond);
	}
}
void loadAnimation(const aiScene *scene,AssimpData& assimp)
{
	auto &animations = assimp.animations;
	auto &nodes = assimp.nodes;
	if (scene->HasAnimations())
	{
		animations.resize(scene->mNumAnimations);
		for (size_t i = 0; i < scene->mNumAnimations; i++)
		{
			auto anim = scene->mAnimations[i];
			auto &animation = animations[i];
			animation.name = anim->mName.C_Str();
			createAnimation(anim->mChannels, anim->mNumChannels, animation, (float)(anim->mTicksPerSecond == 0 ? 1 : anim->mTicksPerSecond), assimp.nodes, i);
		}
	}
}
void loadTexturesName(const aiMaterial* mat, aiTextureType type, std::vector<std::string> &text)
{
	aiString texturePath;
	unsigned int numTextures = mat->GetTextureCount(type);
	text.reserve(numTextures);
	for (unsigned int i = 0; i < numTextures; i++)
	{
		if (mat->GetTexture(type, 0, &texturePath) == AI_SUCCESS)
		{
			text.push_back(texturePath.C_Str());
		}
	}
}
void loadMatirial(const aiScene *scene, AssimpData& assimp)
{
	if (scene->HasMaterials())
	{
		assimp.materials.resize(scene->mNumMaterials);
		for (size_t i = 0; i < scene->mNumMaterials; i++)
		{
			auto &mat = assimp.materials[i];
			const aiMaterial* material = scene->mMaterials[i];
			loadTexturesName(material, aiTextureType_DIFFUSE, mat.diffTextName);
		}
	}
}
AssimpData scanScene(const aiScene *scene)
{
	AssimpData assimpData;
	scanNodes(scene->mRootNode, assimpData.nodes);
	loadMeshes(scene, assimpData);
	loadAnimation(scene, assimpData);
	loadMatirial(scene, assimpData);
	return assimpData;
}
template<class T, class Eq,class Less> unsigned int findInSortet(const std::vector<T> &vec, const T &val, Eq &eq, Less &less)
{
	auto it = std::find_if(vec.begin(), vec.end(), [&eq, &val](auto &value)
	{
		return eq(val, value);
	});
	return it == vec.end() ? -1 : it - vec.begin();
}
struct ModelSubSet
{
	ModelSubSet() :ModelSubSet(0, 0) {}
	ModelSubSet(unsigned int m, unsigned int s) :modelIndex(m), subSetIndex(s) {}
	unsigned int modelIndex;
	unsigned int subSetIndex;
};
struct MeshTemp
{
	unsigned int assimpIndex;
	unsigned int meshIndex;
	unsigned int startVertex;
	unsigned int startIndex;
	DataIndexer boneIndex;
	std::vector<NodeTree<AssimpData::Node>*>node;
	ModelSubSet modelSubSet;
};
#define updateTimes(curTime,index,maxI,vec) if(index<maxI&&curTime>vec[index].time){curTime=vec[index].time;}
template<class A,class B,class C> bool updateTime(unsigned int aI, unsigned int bI, unsigned int cI, const std::vector<A> &aV, const std::vector<B> &bV, const std::vector<C> &cV,float &time)
{
	float minTime = std::numeric_limits<float>::infinity();
	updateTimes(minTime, aI, (aV.size()), aV);
	updateTimes(minTime, bI, (bV.size()), bV);
	updateTimes(minTime, cI, (cV.size()), cV);
	time = minTime;
	return minTime != std::numeric_limits<float>::infinity();
}
template<class T, class Z> void addDat(ModelDat::Bone::Frame &frame, std::vector<T> &vec, unsigned int &index, Z &data)
{
	if (index < vec.size())
	{
		if (vec[index].time == frame.time)
		{
			copyData(data, vec[index++].value);
		}
	}
}
void loadBoneAnimation(const AssimpData &dat, const std::vector<DataIndexer> &animationsIndex, ModelDat::Bone &b,unsigned int &size)
{
	b.frames.resize(animationsIndex.size());
	size += sizeof(ModelData::DATA_COUNT)*(1 + animationsIndex.size());
	for (size_t i = 0; i < animationsIndex.size(); i++)
	{
		auto &bAnim = b.frames[i];
		for (auto &animFrames : animationsIndex[i])
		{
			auto frames = dat.animations[i].frames[animFrames];
			unsigned int ti = 0, si = 0, qi = 0;
			ModelDat::Bone::Frame frame;
			frame.time = 0;
			while (updateTime(ti,si,qi,frames.transformation,frames.scale,frames.quat,frame.time))
			{
				addDat(frame, frames.transformation, ti, frame.transform);
				addDat(frame, frames.scale, si, frame.scale);
				addDat(frame, frames.quat, qi, frame.quat);
				bAnim.push_back(frame);
			}
		}
		size += bAnim.size()*sizeof(ModelDat::Bone::Frame);
	}
}
unsigned int getVertexId(const AssimpData::Bone::Weight& w, const std::vector<MeshTemp> &tempsMesh, unsigned int boneIndex)
{
	auto it = std::find_if(tempsMesh.cbegin(), tempsMesh.cend(), [&w](const auto& tm)
	{
		return tm.assimpIndex == w.meshId;
	});
	unsigned int ret = w.vertexId;
	if (it != tempsMesh.cend())
	{
		ret += (*it).startVertex;
	}
	return ret;
}
void loadWeight(const AssimpData::Bone &bone, unsigned int nboneIndex, ModelDat &model, const std::vector<MeshTemp> &tempsMesh, int boneIndex, unsigned int &size)
{
	ModelDat::BoneDat::BoneWeight w;
	w.boneIndex = nboneIndex;
	size += bone.weights.size()*sizeof(ModelDat::BoneDat::BoneWeight);
	for (auto &wi : bone.weights)
	{
		w.weight = wi.weight;
		model.bonesDat[getVertexId(wi, tempsMesh, boneIndex)].weights.push_back(w);
	}
}
int convertBones(const AssimpData &dat, const NodeTree<AssimpData::Node> &node, std::vector<ModelDat> &models, std::vector<Node>& nodes, int parentBone, const std::vector<MeshTemp> &meshTemps, unsigned int &size)
{
	int modelIndex = -1;
	for (auto boneIndex : node.val.boneIndex)
	{
		unsigned int mIndex = -1;
		auto & meshTempIt = std::find_if(meshTemps.cbegin(), meshTemps.cend(), [boneIndex, &mIndex](const MeshTemp &tempMesh)
		{
			if (tempMesh.boneIndex.size() == 0)
			{
				return false;
			}
			return (mIndex = findInSortet(tempMesh.boneIndex, boneIndex, std::equal_to<unsigned int>(), std::less<unsigned int>())) != -1;
		});
		if (meshTempIt != meshTemps.cend())
		{
			auto &meshTemp = *meshTempIt;
			auto &bone = dat.bones[boneIndex];
			auto &model = models[meshTemp.meshIndex];
			modelIndex = meshTemp.meshIndex;
			model.bonesDat.resize(model.vertex.size());
			int dboneIndex = model.bones.size();
			size += model.bones.size()*(sizeof(int) + 2 * sizeof(Float4x4)) + sizeof(ModelData::DATA_COUNT);//for static variable and size of BoneDat::weights
			model.bones.resize(dboneIndex + 1);
			ModelDat::Bone &b = model.bones[dboneIndex];
			b.boneTransformation = node.val.transform;
			b.offsetMatrix = bone.offsetMatrix;
			b.parentBone = parentBone;
			loadBoneAnimation(dat, node.val.animationIndex, b, size);
			loadWeight(bone, dboneIndex, model, meshTemps, boneIndex, size);
			for (auto& n : node.child)
			{
				int temp = convertBones(dat, n, models, nodes, dboneIndex, meshTemps, size);
				if (temp != -1 && temp != modelIndex)
				{
					throw "Not implement";
				}
			}
		}
	}
	return modelIndex;
}
void convertNodes(const AssimpData &dat, const NodeTree<AssimpData::Node> &node, int lastNodeIndex, std::vector<ModelDat> &models, std::vector<Node>& nodes, int parentNode, std::vector<MeshTemp> &meshTemps, unsigned int &size)
{
	if (node.val.boneIndex.size() == 0 && node.val.meshIndex.size() == 0)
	{
		Node no;
		no.modelIndex = -1;
		no.boneModelIndex = -1;
		no.parentNode = parentNode;
		int nodeIndex = nodes.size();
		no.transformation = node.val.transform;
		nodes.push_back(no);
		lastNodeIndex = nodes.size() - 1;
		size += sizeof(Node);
		for (size_t i = 0; i < node.child.size(); i++)
		{
			convertNodes(dat, node.child[i], lastNodeIndex, models, nodes, nodeIndex, meshTemps, size);
		}
	}
	else if (node.val.meshIndex.size() > 0)
	{
		const auto &curNode = node.val;
		auto &model = *std::find_if(meshTemps.cbegin(), meshTemps.cend(), [&curNode](const auto& meshTemp)
		{
			return std::find_if(meshTemp.node.cbegin(), meshTemp.node.cend(), [&curNode](const auto& nodes)
			{
				return &curNode == &nodes->val;
			}) != meshTemp.node.cend();
		});
		Node no;
		//assume only one mesh per node.if there is more then one they combined as subset.
		no.boneModelIndex = -1;
		no.modelIndex = model.meshIndex;
		no.parentNode = parentNode;
		no.transformation = node.val.transform;
		size += sizeof(Node);
		nodes.push_back(no);
	}
	else
	{
		nodes[lastNodeIndex].boneModelIndex = convertBones(dat, node, models, nodes, -1, meshTemps, size);

	}
}
struct MeshNode
{
	MeshNode(unsigned int m, const std::string&node) :meshIndex(m), nodeName(node) {}
	unsigned int meshIndex;
	std::string nodeName;
};
void convertModels(AssimpData &dat, std::vector<ModelDat> &models, std::vector<MeshTemp> &meshTemp, unsigned int &size)
{
	models.reserve(dat.meshes.size());
	std::vector<MeshNode> checkedMeshes;
	std::map<unsigned int, ModelSubSet> oldMeshToNewMesh;
	for (size_t i = 0; i < dat.meshes.size(); i++)
	{
		auto &curMesh = dat.meshes[i];
		if (curMesh.meshType == AssimpData::PrimitiveType::TRIANGLE)
		{
			std::vector<NodeTree<AssimpData::Node>*> foundNode;
			findAllInTree(dat.nodes, foundNode, [i](const auto node)
			{
				return findInSortet<unsigned int>(node.meshIndex, i, std::equal_to<unsigned int>(), std::less<unsigned int>()) != -1;
			});
			unsigned int modelIndex = models.size();
			size += 11 * sizeof(ModelData::DATA_COUNT) + sizeof(Node);//for 11 vectors
			ModelDat model;
			bool isNewModel = false;
			auto &vertex = model.vertex;
			auto &index = model.index;
			auto &text = model.texture;
			auto &normal = model.normal;
			//assume there is no situation like this:
			// n1 - m1,m2
			// n2 - m2,m3
			// n3 - m1,m3
			// all node with spesific mesh use the same mesh array
			// and all mesh for spesific node do not use with other node that has different subsets
			for (const auto& node : foundNode)
			{
				for (auto meshIndex : node->val.meshIndex)
				{
					if (std::find_if(checkedMeshes.cbegin(), checkedMeshes.cend(), [&node,meshIndex](const auto&meshNode)
					{
						return meshIndex == meshNode.meshIndex && node->val.name == meshNode.nodeName;
					}) == checkedMeshes.cend())
					{
						checkedMeshes.push_back(MeshNode(meshIndex, node->val.name));
						auto &oldToNew = oldMeshToNewMesh.find(meshIndex);
						if (oldToNew == oldMeshToNewMesh.cend())
						{
							isNewModel = true;
							auto &mesh = dat.meshes[meshIndex];
							unsigned int numOfVertex = vertex.size();
							MeshTemp temp;
							temp.assimpIndex = meshIndex;
							temp.node = foundNode;
							temp.boneIndex = mesh.boneIndex;
							temp.meshIndex = modelIndex;
							temp.startVertex = vertex.size();
							temp.startIndex = index.size();
							oldMeshToNewMesh[meshIndex] = ModelSubSet(modelIndex, model.subset.size());
							temp.modelSubSet = ModelSubSet(modelIndex, model.subset.size());
							SubSet subset;
							subset.startVertex = 0;
							subset.startIndex = index.size();
							subset.indexCount = mesh.index.size();
							model.subset.push_back(subset);
							insertToVector(vertex, mesh.vertex);
							insertToVector(text, mesh.texture);
							insertToVector(normal, mesh.normal);
							insertToVector(index, mesh.index, [numOfVertex](auto i)
							{
								return i + numOfVertex;
							});
							if (mesh.materialIndex != -1)
							{
								int matIndex = model.materials.size();
								ModelDat::MaterialDat m;
								m.diffTextName = dat.materials[mesh.materialIndex].diffTextName;
								size += sizeof(ModelData::DATA_COUNT);
								for (size_t i = 0; i < m.diffTextName.size(); i++)
								{
									size += m.diffTextName[i].size() + 1;
								}
								model.materials.push_back(m);
								model.matirialIndexPerSubset.push_back(matIndex);
							}
							else
							{
								model.matirialIndexPerSubset.push_back(-1);
							}
							meshTemp.push_back(std::move(temp));
						}
					}
				}
			}
			if (isNewModel)
			{
				size += sizeof(VertexData)*vertex.size() + sizeof(TextData)*text.size() + sizeof(NormalData)*normal.size() + sizeof(DWORD)*index.size() +
					sizeof(SubSet)*model.subset.size() + sizeof(int)*model.matirialIndexPerSubset.size();
				for (auto &anim : dat.animations)
				{
					size += anim.name.size() + 1;
					model.animationsNames.push_back(anim.name);
				}
				models.resize(modelIndex + 1);
				models[modelIndex] = std::move(model);
			}
		}
	}
}
unsigned int convertData(AssimpData &dat, std::vector<ModelDat> &models, std::vector<Node>& nodes)
{
	unsigned int size = sizeof(ModelData::DATA_COUNT) * 2;
	std::vector<MeshTemp> meshTemp;
	convertModels(dat, models, meshTemp, size);
	convertNodes(dat, dat.nodes, -1, models, nodes, -1, meshTemp, size);
	return size;
}
bool testBoneValidation(const std::vector<ModelDat> &models)
{
	for (const auto& model:models)
	{
		for (const auto& bone : model.bonesDat)
		{
			float sum = 0;
			for (size_t i = 0; i < bone.weights.size(); i++)
			{
				sum += bone.weights[i].weight;
			}
			sum -= 1;
			const float DIFF = 0.0001f;
			if (!(sum>=-DIFF&&sum<=DIFF))
			{
				return false;
			}
		}
	}
	return true;
}
void AssimpT::preapereWriteMemory()
{
	std::vector<ModelDat> models;
	std::vector<Node> nodes;
	bufSize = convertData(scanScene(scene), models, nodes);
	bool isPass = testBoneValidation(models);
	data.setNodes(std::move(nodes));
	data.setModels(std::move(models));
}
bool AssimpT::writeData(void * loc) const
{
	data.writeToBuffer(loc);
	return true;
}