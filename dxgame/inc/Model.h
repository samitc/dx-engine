#ifndef _MODEL_H_
#define _MODEL_H_
#include "GOEntity.h"
#include "Engine.h"
#include <vector>
#include <list>
#include "ModelData.h"
class DXMain;
class Buffer;
class DataManager;
class Matrix;
struct SubSet;
struct ModelBuilderData
{
public://for engine
	ModelBuilderData(ModelData &&modelData);
	ModelData modelData;
};
//for engine
class ModelBuildBuilder
{
public:
	enum class DataType
	{
		vertex = 1,
		index = 1 << 1,//not supported yet
		normal = 1 << 2,
		texture = 1 << 3,
		bones = 1 << 4,
		ALL = vertex | index | normal | texture | bones
	};
	ModelBuildBuilder();
	ModelBuildBuilder(const ModelBuildBuilder &) = default;
	ModelBuildBuilder(ModelBuildBuilder &&);
	~ModelBuildBuilder() = default;
	ModelBuildBuilder& operator=(ModelBuildBuilder&&);
	DataType getOrder(int pos) const;
	void setOrder(int pos, DataType order);
protected:
	int modelIndex;
	DataType dataToCreate;
	unsigned int boneSize;
	std::vector<DataType> order;//not implement yet
	GET_SET_METHOD(boneSize, unsigned int, public);
	GET_SET_METHOD(modelIndex, int, public);
	GET_SET_METHOD(dataToCreate, DataType, public);
};
//class that represent model
class Model:public GOEntity
{
private:
	class ModelBuilderBase
	{
	public:
		ModelBuilderBase() = default;
		~ModelBuilderBase() = default;
		const ModelBuildBuilder &getModelBuildBuilder() const;
		void setModelBuildBuilder(ModelBuildBuilder &&);
	private:
		ModelBuildBuilder modelBuild;
	};
public:
	class CompleteModelBuilder :public ModelBuilderBase
	{
	public:
		CompleteModelBuilder(ModelBuilderData &&builder);
		unsigned int getNumOfModels() const;
		unsigned int getNumOfNodesForModel(unsigned int modelIndex);
		void getModelNodes(unsigned int modelIndex, Matrix* mat);
		void build(const DXMain &dx, Model *model) const;
	private:
		ModelBuilderData modelData;
	};
	//builder class for model
	class ModelBuilder
	{
	public:
		ModelBuilder();
		ModelBuilder(int sizeOfElements, int numberOfElements, void *vertex);
		ModelBuilder(const void *vertex, int sizeOfElements, int numberOfElements);
		ModelBuilder(const void *vertex, const WORD *index, int sizeOfElements, int numberOfElements, int numberOfIndexes);
		ModelBuilder(const void *vertex, const DWORD *index, int sizeOfElements, int numberOfElements, int numberOfIndexes);
		ModelBuilder(const void *vertex, const WORD *index, int sizeOfElements, int numberOfElements, int  numberOfIndexes, const std::list<SubSet> &subset);
		ModelBuilder(const void *vertex, const DWORD *index, int sizeOfElements, int numberOfElements, int numberOfIndexes, const std::list<SubSet> &subset);
		ModelBuilder(const std::list<SubSet> &subset, void *vertex, WORD *index, int sizeOfElements, int numberOfElements, int numberOfIndexes);
		ModelBuilder(const std::list<SubSet> &subset, void *vertex, DWORD *index, int sizeOfElements, int numberOfElements, int numberOfIndexes);
		ModelBuilder(const ModelBuilder&) = delete;
		~ModelBuilder() = default;
		void build(const DXMain &, Model *) const;
		Model *build(const DXMain &) const;
		ModelBuilder& setIndexes(const WORD *index, int numberOfIndexes);
		ModelBuilder& setIndexes(const DWORD *index, int numberOfIndexes);
		ModelBuilder& setIndexes(WORD *index, int numberOfIndexes);
		ModelBuilder& setIndexes(DWORD *index, int numberOfIndexes);
		ModelBuilder& addSubSet(const SubSet &subSet);
	private:
		void changeIndex(int numberOfIndex);
		const void* vertex;
		const WORD* data;
		const DWORD* dData;
		void* ncVertex;
		WORD* ncData;
		DWORD* ncdData;
		int sizeOfElements;
		int numberOfElements;
		int numberOfIndexes;
		std::list<SubSet> subset;
	};
	//builder class for model from file
	class ModelBuilderFromResource :public ModelBuilderBase
	{
	public:
		//create builder from file
		ModelBuilderFromResource(const char* fileName);
		ModelBuilderFromResource(const ModelBuilderFromResource &) = delete;
		ModelBuilderFromResource(ModelBuilderFromResource &&);
		~ModelBuilderFromResource();
		void build(const DXMain &, Model *) const;
		void build(const DXMain &) const;
		ModelBuilderData createData() const;
		void build(const DataManager& manager, const DXMain &, const std::string& name, Model *) const;
		void build(const DataManager& manager, const DXMain &, const std::string& name) const;
	public://for engine
		const ModelData* getModelData() const;
		static ModelData *load(const byte *data, int size);
	private:
		void testMData() const;
		ModelBuilderFromResource();
		char* fileLoc;
		mutable ModelData mData;
		mutable Model* m;
		const mutable DXMain *dx;
	};
	//builder class for model from number of engine objects
	class ModelBuilderFromObjects
	{
	public:
		class DataWrapper
		{
		public:
			DataWrapper(void *data, unsigned int l);
			DataWrapper(const void *data, unsigned int l);
			DataWrapper(const DataWrapper& copy);
			~DataWrapper();
			void* data;
			unsigned int l;
		};
	public:
		ModelBuilderFromObjects() = default;
		ModelBuilderFromObjects(const ModelBuilderFromObjects&) = delete;
		//add engine object to the builder
		//the object is deallocate automatically at the deallocation of the builder
		ModelBuilderFromObjects& addEntity(const GOEntity* entity);
		void build(const DXMain &, Model *) const;
		Model *build(const DXMain &) const;
		~ModelBuilderFromObjects();
	private:
		std::list<const GOEntity*> entities;
	};
	class ModelBuilderFromEngineResource :public ModelBuilderBase
	{
	public:
		struct ModelDataWrapper
		{
			void *vertex;
			void *index;
			SubSet *subSets;
			int sizeOfVertex;
			int sizeOfIndex;
			int numberOfVertex;
			int numberOfIndex;
			int numberOfSubset;
			~ModelDataWrapper();
		};
		ModelBuilderFromEngineResource() = delete;
		ModelBuilderFromEngineResource(std::string name);
		static void saveModel(const DataManager& manager, const Model *m, const std::string& name);
		void build(const DataManager& manager, const DXMain &, Model *m) const;
	public://public for engine
		static ModelDataWrapper *load(const byte *data, int size);
		static byte* save(const Model*, int *);
	private:
		std::string name;
	};
public:
	Model();
	virtual ~Model();
	virtual void render(const RenderDevice &) override;
public://for engine
	void setHandle(HANDLE dataHandle);
protected:
	//called before spesific subset is render
	virtual bool renderSubSet(const SubSet& subSet);
	//return the vertex data
	const void* getVertex(unsigned int index) const;
	DWORD getIndex(int index) const;
	//return the subset data
	std::list<SubSet>& getsubsetStart();
protected://for engine
	void waitForData() const;
private:
	Model(const DXMain& dx, const void *vertex, const WORD *index, int sizeOfElements, int numberOfElements, int numberOfIndexes);
	Model(const DXMain & dx, const void * vertex, const WORD * index, int sizeOfElements, int numberOfElements, int numberOfIndexes, const std::list<SubSet> &);
	void Init(const DXMain& dx, const void *vertex, const WORD *index, int sizeOfElements, int numberOfElements, int numberOfIndexes, const std::list<SubSet> &);
	void Init(const DXMain& dx, const void *vertex, const DWORD *index, int sizeOfElements, int numberOfElements, int numberOfIndexes, const std::list<SubSet> &);
	void Init(const DXMain& dx, void *vertex, const WORD *index, int sizeOfElements, int numberOfElements, int numberOfIndexes, const std::list<SubSet> &);
	void Init(const DXMain& dx, void *vertex, const DWORD *index, int sizeOfElements, int numberOfElements, int numberOfIndexes, const std::list<SubSet> &);
	void Init(const DXMain& dx, void *vertex, WORD *index, int sizeOfElements, int numberOfElements, int numberOfIndexes, const std::list<SubSet> &);
	void Init(const DXMain& dx, void *vertex, DWORD *index, int sizeOfElements, int numberOfElements, int numberOfIndexes, const std::list<SubSet> &);
	Model& operator=(Model& obj) = delete;
	template<class T> friend void ModelInit(Model& model, const DXMain & dx, void * vertex, T * index, int sizeOfElements, int numberOfElements, int numberOfIndexes,const std::list<SubSet>&);
	std::list<SubSet> subsetStart;
	void* vertex;
	void* index;
	bool isDWord;
	mutable HANDLE dataHandle;
	SAMEC_GET_SET_METHOD(vertex, void*, private);
	SAMEC_GET_SET_METHOD(index, void*, private);
	GET_SET_METHOD(isDWord, bool, private);
	GET_SET_METHOD(subsetStart, std::list<SubSet>, protected);
};
template<class T> void ModelInit(Model& model, const DXMain & dx, const void * vertex, const T * index, int sizeOfElements, int numberOfElements, int numberOfIndexes, const std::list<SubSet>&);
ModelBuildBuilder::DataType operator&(ModelBuildBuilder::DataType, ModelBuildBuilder::DataType);
Matrix f2m(const Float4x4 &f);
#endif