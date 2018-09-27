#include "Model.h"
#include <stack>
#include "Buffer.h"
#include "DXMain.h"
#include "Device.h"
#include "MapData.h"
#include "SubSet.h"
#include "BufferSizeNotMatch.h"
#include "BaseLog.h"
#include "DataManager.h"
#include "DxHandler.h"
#include "Matrix.h"
#include "ResourceManager.h"
namespace
{
    bool checkUnnecessarySubset(const int *numOfVertex, const int *numOfIndex, const SubSet& subset)
    {
        if (numOfIndex == nullptr)
        {
            return subset.indexCount == *numOfVertex&&subset.startIndex == 0;
        }
        else
        {
            return subset.indexCount == *numOfIndex&&subset.startIndex == 0 && subset.startVertex == 0;
        }
    }
}
Model::Model() :GOEntity()
{
}
Model::Model(const DXMain &dx, const void * vertex, const WORD * index, int sizeOfElements, int numberOfElements, int numberOfIndexes) : GOEntity()
{
	this->Init(dx, vertex, index, sizeOfElements, numberOfElements, numberOfIndexes, std::list<SubSet>());
}
Model::Model(const DXMain & dx, const void * vertex, const WORD * index, int sizeOfElements, int numberOfElements, int numberOfIndexes, const std::list<SubSet> &subset)
{
	this->Init(dx, vertex, index, sizeOfElements, numberOfElements, numberOfIndexes, subset);
}
Model::~Model()
{
	delete[] vertex;
}
void Model::render(const RenderDevice & renderDevice)
{
	waitForData();
	if (this->getsubsetStart().size() == 0)
	{
		GOEntity::render(renderDevice);
	}
	else
	{
		std::list<SubSet>::iterator s = this->getsubsetStart().begin(), e = this->getsubsetStart().end();
		this->prepareRendering(renderDevice);
		if (&this->getIbuffer() != nullptr)
		{
			renderDevice.getdx().getDevice().setIndexBuffer(this->getIbuffer().getBufferType(), 0, this->getIbuffer().getSizeOfElement());
		}
		while (s != e)
		{
			if (this->renderSubSet(*s))
			{
				if (&this->getIbuffer() != nullptr)
				{
					draw(renderDevice, (*s).indexCount, (*s).startIndex, (*s).startVertex);
				}
				else
				{
					draw(renderDevice, (*s).indexCount, (*s).startIndex, -1);
				}
			}
			++s;
		}
	}
}
void Model::setHandle(HANDLE dataHandle)
{
	this->dataHandle = dataHandle;
}
bool Model::renderSubSet(const SubSet & subSet)
{
	return true;
}
const void* Model::getVertex(unsigned int index) const
{
	waitForData();
	return (char*)this->vertex + (index*getVbuffer().getSizeOfElement());
}
DWORD Model::getIndex(int index) const
{
	waitForData();
	if (isDWord)
	{
		return ((DWORD*)(this->index))[index];
	}
	else
	{
		return ((WORD*)(this->index))[index];
	}
}
void* Model::getvertex()
{
	waitForData();
	return this->vertex;
}
std::list<SubSet>& Model::getsubsetStart()
{
	waitForData();
	return this->subsetStart;
}
template<class T> void ModelInit(Model& model, const DXMain & dx, void * vertex, T * index, int sizeOfElements, int numberOfElements, int numberOfIndexes, const std::list<SubSet>& subset)
{
    model.vertex = vertex;
    model.setVbuffer(new Buffer(dx.getResourceManager().createBuffer(numberOfElements, sizeOfElements, ResourceFormat::VERTEX_BUFFER, (Access)(Access::GpuRead | Access::CpuWrite), vertex)));
    if (index == nullptr || numberOfIndexes == 0)
    {
        model.setIbuffer(nullptr);
        model.index = nullptr;
    }
    else
    {
        if (sizeof(T) == sizeof(DWORD))
        {
            model.setisDWord(true);
        }
        else
        {
            model.setisDWord(false);
        }
        model.index = index;
        model.setIbuffer(new Buffer(dx.getResourceManager().createBuffer(numberOfIndexes, sizeof T, ResourceFormat::INDEX_BUFFER, (Access)(Access::GpuRead | Access::CpuWrite), index)));
    }
    if (subset.size() != 1 || !checkUnnecessarySubset(&numberOfElements, model.index == nullptr ? nullptr : &numberOfIndexes, subset.front()))
    {
        model.setsubsetStart(subset);
    }
}
template<class T> void ModelInit(Model& model, const DXMain & dx, void * vertex, const T * index, int sizeOfElements, int numberOfElements, int numberOfIndexes, const std::list<SubSet>& subset)
{
	if (index == NULL || numberOfIndexes == 0)
	{
		ModelInit(model, dx, vertex, (WORD*)nullptr, sizeOfElements, numberOfElements, 0, subset);
	}
	else
	{
		if (numberOfElements > 0xffff)
		{
			DWORD *in = new DWORD[numberOfIndexes];
			for (int i = 0; i < numberOfIndexes; i++)
			{
				in[i] = (DWORD)index[i];
			}
			ModelInit(model, dx, vertex, in, sizeOfElements, numberOfElements, numberOfIndexes, subset);
		}
		else
		{
			WORD *in = new WORD[numberOfIndexes];
			for (int i = 0; i < numberOfIndexes; i++)
			{
				in[i] = (WORD)index[i];
			}
			ModelInit(model, dx, vertex, in, sizeOfElements, numberOfElements, numberOfIndexes, subset);
		}
	}
}
template<class T> void ModelInit(Model& model, const DXMain & dx, const void * vertex, const T * index, int sizeOfElements, int numberOfElements, int numberOfIndexes, const std::list<SubSet>& subset)
{
	register int sizeOfVertex = numberOfElements*sizeOfElements;
	void *v = new char[sizeOfVertex];
	memcpy(v, vertex, sizeOfVertex);
	if (index == NULL || numberOfIndexes == 0)
	{
		ModelInit(model, dx, v, (WORD*)nullptr, sizeOfElements, numberOfElements, 0, subset);
	}
	else
	{
		if (numberOfElements > 0xffff)
		{
			DWORD *in = new DWORD[numberOfIndexes];
			for (int i = 0; i < numberOfIndexes; i++)
			{
				in[i] = (DWORD)index[i];
			}
			ModelInit(model, dx, v, in, sizeOfElements, numberOfElements, numberOfIndexes, subset);
		}
		else
		{
			WORD *in = new WORD[numberOfIndexes];
			for (int i = 0; i < numberOfIndexes; i++)
			{
				in[i] = (WORD)index[i];
			}
			ModelInit(model, dx, v, in, sizeOfElements, numberOfElements, numberOfIndexes, subset);
		}
	}
}
void Model::waitForData() const
{
	if (dataHandle != NULL)
	{
		while (dataHandle == INVALID_HANDLE_VALUE);
		WaitForSingleObject(dataHandle, INFINITE);
		CloseHandle(dataHandle);
		dataHandle = NULL;
	}
}
void Model::Init(const DXMain & dx, const void * vertex, const WORD * index, int sizeOfElements, int numberOfElements, int numberOfIndexes, const std::list<SubSet>& subset)
{
	ModelInit(*this, dx, vertex, index, sizeOfElements, numberOfElements, numberOfIndexes, subset);
}
void Model::Init(const DXMain & dx, const void * vertex, const DWORD * index, int sizeOfElements, int numberOfElements, int numberOfIndexes, const std::list<SubSet>& subset)
{
	ModelInit(*this, dx, vertex, index, sizeOfElements, numberOfElements, numberOfIndexes, subset);
}
void Model::Init(const DXMain & dx, void * vertex, const WORD * index, int sizeOfElements, int numberOfElements, int numberOfIndexes, const std::list<SubSet>& subset)
{
	ModelInit(*this, dx, vertex, index, sizeOfElements, numberOfElements, numberOfIndexes, subset);
}
void Model::Init(const DXMain & dx, void * vertex, const DWORD * index, int sizeOfElements, int numberOfElements, int numberOfIndexes, const std::list<SubSet>& subset)
{
	ModelInit(*this, dx, vertex, index, sizeOfElements, numberOfElements, numberOfIndexes, subset);
}
void Model::Init(const DXMain & dx, void * vertex, WORD * index, int sizeOfElements, int numberOfElements, int numberOfIndexes, const std::list<SubSet>& subset)
{
	ModelInit(*this, dx, vertex, index, sizeOfElements, numberOfElements, numberOfIndexes, subset);
}
void Model::Init(const DXMain & dx, void * vertex, DWORD * index, int sizeOfElements, int numberOfElements, int numberOfIndexes, const std::list<SubSet>& subset)
{
	ModelInit(*this, dx, vertex, index, sizeOfElements, numberOfElements, numberOfIndexes, subset);
}
Model::ModelBuilderFromResource::ModelBuilderFromResource(const char * modelLocation):ModelBuilderFromResource()
{
	int l = strlen(modelLocation) + 1;
	fileLoc = new char[l];
	strcpy(fileLoc, modelLocation);
}
Model::ModelBuilderFromResource::ModelBuilderFromResource(ModelBuilderFromResource &&copy) :mData(std::move(copy.mData)), fileLoc(copy.fileLoc), dx(copy.dx), m(copy.m)
{
	copy.fileLoc = nullptr;
	copy.dx = nullptr;
	copy.m = nullptr;
}
Model::ModelBuilderFromResource::~ModelBuilderFromResource()
{
	delete[] fileLoc;
}
void Model::ModelBuilderFromResource::build(const DXMain &dx, Model *model) const
{
	m = model;
	dx.loadModel(fileLoc, model, getModelBuildBuilder());
	this->dx = &dx;
}
void Model::ModelBuilderFromResource::build(const DXMain & dx) const
{
	this->build(dx, nullptr);
}
ModelBuilderData Model::ModelBuilderFromResource::createData() const
{
	return ModelBuilderData(ModelData(*getModelData()));
}
void Model::ModelBuilderFromResource::build(const DataManager & manager, const DXMain & dx, const std::string & name, Model *model) const
{
	m = model;
	std::unique_ptr<ModelData> dat(manager.loadStatic<Model::ModelBuilderFromResource, ModelData>(name.c_str()));
	this->mData = std::move(*dat);
	this->dx = &dx;
	DxHandler::buildModel(dx, m, mData, getModelBuildBuilder());
	m->dataHandle = nullptr;
}
void Model::ModelBuilderFromResource::build(const DataManager & manager, const DXMain & dx, const std::string & name) const
{
	throw "Not implement";//as long as the loading process is not multi-threaded there is no sence for such method
}
const ModelData * Model::ModelBuilderFromResource::getModelData() const
{
	testMData();
	return &mData;
}
ModelData * Model::ModelBuilderFromResource::load(const byte * data, int size)
{
	int version = ((int*)data)[0];
	ModelData *ret = new ModelData();
	ret->readFromBuffer((void*)(((int*)data) + 1));
	return ret;
}
void Model::ModelBuilderFromResource::testMData() const
{
	if (mData.getModels().size() == 0)
	{
		mData = std::move(dx->getModelData(fileLoc));
	}
}
Model::ModelBuilderFromResource::ModelBuilderFromResource() :mData(), dx(nullptr), m(nullptr)
{
}
Model::ModelBuilderFromObjects& Model::ModelBuilderFromObjects::addEntity(const GOEntity * entity)
{
	if (std::find(this->entities.begin(), this->entities.end(), entity) == this->entities.end())
	{
		this->entities.push_back(entity);
	}
	return *this;
}
void Model::ModelBuilderFromObjects::build(const DXMain &dx, Model *m) const
{
	if (this->entities.size() != 0)
	{
	std::list<const GOEntity*>::const_iterator s = this->entities.begin(), e = this->entities.end();
	std::list<DataWrapper> vertex;
	std::list<DataWrapper> indexes;
	std::list<SubSet> subsets;
	unsigned int sizeOfElement = (*s)->getvBuffer()->getSizeOfElement();
	unsigned int numberOfVertex = 0;
	unsigned int numberOfIndex = 0;
	while (s != e)
	{
		if ((*s)->getvBuffer()->getSizeOfElement() != sizeOfElement)
		{
			BaseLog::getInstance().warn("size of element is not compatible with multiple objects");
			throw BufferSizeNotMatch((*s)->getvBuffer()->getSizeOfElement(), sizeOfElement);
		}
		if ((*s)->getiBuffer() != nullptr)
		{
			unsigned int size = (*s)->getiBuffer()->getSizeOfElement();
			unsigned int num = (*s)->getiBuffer()->getNumberOfElements();
			{//index buffer
				Buffer b = dx.getResourceManager().createBuffer(num, size, ResourceFormat::NONE, (Access)(Access::GpuRead | Access::CpuRead | Access::CpuWrite), NULL);
				b.copyFromResource(dx, (*s)->getiBuffer());
				MapData map(b);
				indexes.push_back(DataWrapper((const void*)map.getData(dx, MapType::MAP_READ), size*num));
				map.applyData(dx);
			}
			unsigned int sizeV = (*s)->getvBuffer()->getSizeOfElement();
				unsigned int numV = (*s)->getvBuffer()->getNumberOfElements();
			{//vertex buffer
					Buffer b = dx.getResourceManager().createBuffer(numV, sizeV, ResourceFormat::NONE, (Access)(Access::GpuRead | Access::CpuRead | Access::CpuWrite), NULL);
				b.copyFromResource(dx, (*s)->getvBuffer());
				MapData map(b);
				vertex.push_back(DataWrapper((const void*)map.getData(dx, MapType::MAP_READ), sizeV*numV));
				map.applyData(dx);
			}
			SubSet s;
			s.startIndex = numberOfIndex;
			s.startVertex = numberOfVertex;
			s.indexCount = num;
			subsets.push_back(std::move(s));
			numberOfIndex += num;
			numberOfVertex += numV;
		}
		else
		{
			unsigned int size = (*s)->getvBuffer()->getSizeOfElement();
			unsigned int num = (*s)->getvBuffer()->getNumberOfElements();
			{//index buffer
				WORD *indexP = new WORD[num];
				WORD* p = indexP;
				for (unsigned int i = 0; i < num; i++, p++)
				{
					*p = i;
				}
				indexes.push_back(DataWrapper(indexP, (sizeof WORD)*num));
			}
			{//vertex buffer
				Buffer b = dx.getResourceManager().createBuffer(num, size, ResourceFormat::NONE, (Access)(Access::GpuRead | Access::CpuRead | Access::CpuWrite), NULL);
				b.copyFromResource(dx, (*s)->getvBuffer());
				MapData map(b);
				vertex.push_back(DataWrapper((const void*)map.getData(dx, MapType::MAP_READ), size*num));
				map.applyData(dx);
			}
			SubSet s;
			s.startIndex = numberOfIndex;
			s.startVertex = numberOfVertex;
			s.indexCount = num;
			subsets.push_back(std::move(s));
			numberOfIndex += num;
				numberOfVertex += num;
			}
		s++;
	}
	void *finalData = new char[sizeOfElement*numberOfVertex];
	WORD *finalIndex = new WORD[numberOfIndex];
	std::list<DataWrapper>::const_iterator sD = vertex.begin(), eD = vertex.end();
	char* curPos = (char*)finalData;
	while (sD != eD)
	{
		memcpy(curPos, (*sD).data, (*sD).l);
		curPos += (*sD).l;
		++sD;
	}
	sD = indexes.begin();
	eD = indexes.end();
	curPos = (char*)finalIndex;
	while (sD != eD)
	{
		memcpy(curPos, (*sD).data, (*sD).l);
		curPos += (*sD).l;
		++sD;
	}
		ModelBuilder(finalData, finalIndex, sizeOfElement, numberOfVertex, numberOfIndex, subsets).build(dx, m);
	delete[]finalIndex;
	delete[]finalData;
	}
}
Model * Model::ModelBuilderFromObjects::build(const DXMain & dx) const
{
	if (entities.size() == 0)
	{
		return nullptr;
	}
	Model *ret = new Model();
	build(dx, ret);
	return ret;
}
Model::ModelBuilderFromObjects::~ModelBuilderFromObjects()
{
	std::list<const GOEntity*>::iterator s = this->entities.begin(), e = this->entities.end();
	while (s!=e)
	{
		delete (*s);
		s++;
	}
}
Model::ModelBuilderFromObjects::DataWrapper::DataWrapper(void * data, unsigned int l)
{
	this->l = l;
	this->data = data;
}
Model::ModelBuilderFromObjects::DataWrapper::DataWrapper(const void * data, unsigned int l)
{
	this->l = l;
	this->data = new char[l];
	memcpy(this->data, data, l);
}
Model::ModelBuilderFromObjects::DataWrapper::DataWrapper(const DataWrapper & copy):l(copy.l)
{
	this->data = new char[l];
	memcpy(this->data, copy.data, l);
}
Model::ModelBuilderFromObjects::DataWrapper::~DataWrapper()
{
	delete[] data;
}
Model::ModelBuilder::ModelBuilder() :vertex(nullptr), data(nullptr), dData(nullptr), ncVertex(nullptr), ncData(nullptr), ncdData(nullptr), sizeOfElements(0), numberOfElements(0), numberOfIndexes(0)
{
}
Model::ModelBuilder::ModelBuilder(int sizeOfElements, int numberOfElements, void *vertex):
	vertex(nullptr), sizeOfElements(sizeOfElements), numberOfElements(numberOfElements), data(nullptr), dData(nullptr), numberOfIndexes(0), subset(), ncVertex(vertex), ncData(nullptr), ncdData(nullptr)
{
}
Model::ModelBuilder::ModelBuilder(const void * vertex, int sizeOfElements, int numberOfElements) :
	vertex(vertex), sizeOfElements(sizeOfElements), numberOfElements(numberOfElements), data(nullptr), dData(nullptr), numberOfIndexes(0), subset(), ncVertex(nullptr), ncData(nullptr), ncdData(nullptr)
{
}
Model::ModelBuilder::ModelBuilder(const void * vertex, const WORD * index, int sizeOfElements, int numberOfElements, int numberOfIndexes)
	: vertex(vertex), sizeOfElements(sizeOfElements), numberOfElements(numberOfElements), data(index), dData(nullptr), numberOfIndexes(numberOfIndexes),ncVertex(nullptr),ncData(nullptr),ncdData(nullptr)
{
}
Model::ModelBuilder::ModelBuilder(const void * vertex, const DWORD * index, int sizeOfElements, int numberOfElements, int numberOfIndexes)
	: vertex(vertex), sizeOfElements(sizeOfElements), numberOfElements(numberOfElements), data(nullptr), dData(index), numberOfIndexes(numberOfIndexes), ncVertex(nullptr), ncData(nullptr), ncdData(nullptr)
{
}
Model::ModelBuilder::ModelBuilder(const void * vertex, const WORD * index, int sizeOfElements, int numberOfElements, int numberOfIndexes, const std::list<SubSet> &subset) :
	vertex(vertex), sizeOfElements(sizeOfElements), numberOfElements(numberOfElements), data(index), dData(nullptr), numberOfIndexes(numberOfIndexes), subset(subset), ncVertex(nullptr), ncData(nullptr), ncdData(nullptr)
{
}
Model::ModelBuilder::ModelBuilder(const void * vertex, const DWORD * index, int sizeOfElements, int numberOfElements, int numberOfIndexes, const std::list<SubSet> &subset) :
	vertex(vertex), sizeOfElements(sizeOfElements), numberOfElements(numberOfElements), data(nullptr), dData(index), numberOfIndexes(numberOfIndexes), subset(subset), ncVertex(nullptr), ncData(nullptr), ncdData(nullptr)
{
}
Model::ModelBuilder::ModelBuilder(const std::list<SubSet>&subset, void *vertex, WORD *index, int sizeOfElements, int numberOfElements, int numberOfIndexes) :
	ncVertex(vertex), ncData(index), ncdData(nullptr), sizeOfElements(sizeOfElements), numberOfElements(numberOfElements), numberOfIndexes(numberOfIndexes), subset(subset)
{
}
Model::ModelBuilder::ModelBuilder(const std::list<SubSet>&subset, void *vertex, DWORD *index, int sizeOfElements, int numberOfElements, int numberOfIndexes) :
	ncVertex(vertex), ncData(nullptr), ncdData(index), sizeOfElements(sizeOfElements), numberOfElements(numberOfElements), numberOfIndexes(numberOfIndexes), subset(subset)
{
}
void Model::ModelBuilder::build(const DXMain & dx, Model * model) const
{
	if (data != nullptr)
	{
		if (vertex != nullptr)
	{
		model->Init(dx, vertex, data, sizeOfElements, numberOfElements, numberOfIndexes, subset);
	}
	else
	{
			model->Init(dx, ncVertex, data, sizeOfElements, numberOfElements, numberOfIndexes, subset);
		}
	}
	else if (dData != nullptr)
	{
		if (vertex != nullptr)
		{
		model->Init(dx, vertex, dData, sizeOfElements, numberOfElements, numberOfIndexes, subset);
	}
		else
		{
			model->Init(dx, ncVertex, dData, sizeOfElements, numberOfElements, numberOfIndexes, subset);
		}
	}
	else if (ncData != nullptr)
	{
		model->Init(dx, ncVertex, ncData, sizeOfElements, numberOfElements, numberOfIndexes, subset);
	}
	else if (ncdData != nullptr)
	{
		model->Init(dx, ncVertex, ncdData, sizeOfElements, numberOfElements, numberOfIndexes, subset);
	}
	else
	{
		if (vertex != nullptr)
		{
			model->Init(dx, vertex, (const WORD*)nullptr, sizeOfElements, numberOfElements, 0, subset);
		}
		else
		{
			model->Init(dx, ncVertex, (WORD*)nullptr, sizeOfElements, numberOfElements, 0, subset);
		}
	}
}
Model * Model::ModelBuilder::build(const DXMain & dx) const
{
	Model* m = new Model();
	this->build(dx, m);
	return m;
}
Model::ModelBuilder& Model::ModelBuilder::setIndexes(const WORD * index, int numberOfIndexes)
{
	changeIndex(numberOfIndexes);
	data = index;
	return *this;
}
Model::ModelBuilder& Model::ModelBuilder::setIndexes(const DWORD * index, int numberOfIndexes)
{
	changeIndex(numberOfIndexes);
	dData = index;
	return *this;
}
Model::ModelBuilder& Model::ModelBuilder::setIndexes(WORD * index, int numberOfIndexes)
{
	changeIndex(numberOfIndexes);
	ncData = index;
	return *this;
}
Model::ModelBuilder& Model::ModelBuilder::setIndexes(DWORD * index, int numberOfIndexes)
{
	changeIndex(numberOfIndexes);
	ncdData = index;
	return *this;
}
Model::ModelBuilder& Model::ModelBuilder::addSubSet(const SubSet &subSet)
{
	this->subset.push_back(subSet);
	return *this;
}
void Model::ModelBuilder::changeIndex(int numberOfIndex)
{
	data = nullptr;
	dData = nullptr;
	ncData = nullptr;
	ncdData = nullptr;
	numberOfIndexes = numberOfIndex;
}
Model::ModelBuilderFromEngineResource::ModelBuilderFromEngineResource(std::string name) :name(std::move(name))
{
}
void Model::ModelBuilderFromEngineResource::saveModel(const DataManager & manager, const Model * m, const std::string &name)
{
	m->waitForData();
	manager.saveStatic<Model::ModelBuilderFromEngineResource, Model>(name.c_str(), m);
}
void Model::ModelBuilderFromEngineResource::build(const DataManager & manager, const DXMain & dx, Model * m) const
{
	std::unique_ptr<ModelDataWrapper> dat(manager.loadStatic<Model::ModelBuilderFromEngineResource, ModelDataWrapper>(name.c_str()));
	std::list<SubSet> subset;
	for (int i = 0; i < dat->numberOfSubset; i++)
	{
		subset.push_back(dat->subSets[i]);
	}
	if (dat->numberOfIndex == 0)
	{
		ModelBuilder(dat->vertex, (WORD*)nullptr, dat->sizeOfVertex, dat->numberOfVertex, 0, subset).build(dx, m);
	}
	else if (dat->sizeOfIndex == 2)
	{
		ModelBuilder(dat->vertex, (WORD*)dat->index, dat->sizeOfVertex, dat->numberOfVertex, dat->numberOfIndex, subset).build(dx, m);
	}
	else
	{
		ModelBuilder(dat->vertex, (DWORD*)dat->index, dat->sizeOfVertex, dat->numberOfVertex, dat->numberOfIndex, subset).build(dx, m);
	}
}
Model::ModelBuilderFromEngineResource::ModelDataWrapper * Model::ModelBuilderFromEngineResource::load(const byte * data, int size)
{
	ModelDataWrapper *ret = new ModelDataWrapper();
	Model *m = new Model();
	int version = ((int*)data)[0];
	ret->numberOfVertex = ((int*)(data))[1];
	ret->sizeOfVertex = ((int*)(data))[2];
	ret->numberOfIndex = ((int*)(data))[3];
	ret->sizeOfIndex = ((int*)(data))[4];
	ret->numberOfSubset = ((int*)(data))[5];
	data += sizeof(int)*6;
	ret->vertex = new char[ret->numberOfVertex*ret->sizeOfVertex];
	memcpy(ret->vertex, data, ret->numberOfVertex*ret->sizeOfVertex);
	data += ret->numberOfVertex*ret->sizeOfVertex;
	if (ret->numberOfIndex>0)
	{
		ret->index = new char[ret->numberOfIndex*ret->sizeOfIndex];
		memcpy(ret->index, data, ret->numberOfIndex*ret->sizeOfIndex);
		data += ret->numberOfIndex*ret->sizeOfIndex;
	}
	else
	{
		ret->index = nullptr;
	}
	if (ret->numberOfSubset>0)
	{
		ret->subSets = new SubSet[ret->numberOfSubset];
		memcpy(ret->subSets, data, ret->numberOfSubset*sizeof(SubSet));
	}
	else
	{
		ret->subSets = nullptr;
	}
	return ret;
}
byte * Model::ModelBuilderFromEngineResource::save(const Model *m, int *s)
{
	int numberOfVertex = m->getvBuffer()->getNumberOfElements();
	int sizeOfVertex = m->getvBuffer()->getSizeOfElement();
	int numberOfIndex = m->getiBuffer() == nullptr ? 0 : m->getiBuffer()->getNumberOfElements();
	int sizeOfIndex = m->getisDWord() ? 4 : 2;
	int numberOfSubset = m->getsubsetStart().size();
	*s = sizeof(int) * 6 + numberOfVertex*sizeOfVertex + numberOfIndex*sizeOfIndex + numberOfSubset*sizeof(SubSet);
	byte *dat = new byte[*s];
	((int*)dat)[0] = 1;
	((int*)dat)[1] = numberOfVertex;
	((int*)dat)[2] = sizeOfVertex;
	((int*)dat)[3] = numberOfIndex;
	((int*)dat)[4] = sizeOfIndex;
	((int*)dat)[5] = numberOfSubset;
	byte *p = dat + sizeof(int) * 6;
	memcpy(p, m->vertex, numberOfVertex*sizeOfVertex);
	p += numberOfVertex*sizeOfVertex;
	if (numberOfIndex > 0)
	{
		memcpy(p, m->index, sizeOfIndex * numberOfIndex);
		p += sizeOfIndex * numberOfIndex;
	}
	if (numberOfSubset > 0)
	{
		for (const auto & sub:m->getsubsetStart())
		{
			memcpy(p, &sub, sizeof(SubSet));
			p += sizeof(SubSet);
		}
	}
	return dat;
}
Model::ModelBuilderFromEngineResource::ModelDataWrapper::~ModelDataWrapper()
{
	delete[]vertex;
	delete[] index;
	delete[]subSets;
}
ModelBuildBuilder::DataType operator&(ModelBuildBuilder::DataType a, ModelBuildBuilder::DataType b)
{
	return (ModelBuildBuilder::DataType)(((int)(a))&((int)(b)));
}
ModelBuildBuilder::ModelBuildBuilder() :modelIndex(0), dataToCreate(DataType::ALL), boneSize(4), order()
{
	order.push_back(DataType::vertex);
	order.push_back(DataType::normal);
	order.push_back(DataType::texture);
	order.push_back(DataType::bones);
}
ModelBuildBuilder::ModelBuildBuilder(ModelBuildBuilder &&copy) :modelIndex(copy.modelIndex), dataToCreate(copy.dataToCreate), boneSize(copy.boneSize), order(std::move(copy.order))
{
}
ModelBuildBuilder& ModelBuildBuilder::operator=(ModelBuildBuilder &&copy)
{
	modelIndex = copy.modelIndex;
	dataToCreate = copy.dataToCreate;
	boneSize = copy.boneSize;
	order = std::move(copy.order);
	return *this;
}
ModelBuildBuilder::DataType ModelBuildBuilder::getOrder(int pos) const
{
	return order[pos];
}
void ModelBuildBuilder::setOrder(int pos, DataType order)
{
	this->order[pos] = order;
}
const ModelBuildBuilder & Model::ModelBuilderBase::getModelBuildBuilder() const
{
	return modelBuild;
}
void Model::ModelBuilderBase::setModelBuildBuilder(ModelBuildBuilder &&c)
{
	modelBuild = std::move(c);
}
Model::CompleteModelBuilder::CompleteModelBuilder(ModelBuilderData && builder) :modelData(std::move(builder))
{
}
unsigned int Model::CompleteModelBuilder::getNumOfModels() const
{
	return modelData.modelData.getModels().size();
}
unsigned int Model::CompleteModelBuilder::getNumOfNodesForModel(unsigned int modelIndex)
{
	unsigned int count = 0;
	for (const auto& node : modelData.modelData.getNodes())
	{
		if (node.modelIndex == modelIndex)
		{
			count++;
		}
	}
	return count;
}
void Model::CompleteModelBuilder::getModelNodes(unsigned int modelIndex, Matrix * mat)
{
	std::stack<const Node*> curNodes;
	const auto &allNodes = modelData.modelData.getNodes();
	for (const auto& node : allNodes)
	{
		if (node.modelIndex == modelIndex)
		{
			const auto *cur = &node;
			while (cur->parentNode != -1)
			{
				curNodes.push(cur);
				cur = &allNodes[cur->parentNode];
			}
			*mat = f2m(cur->transformation);
			while (!curNodes.empty())
			{
				*mat = *mat*f2m(curNodes.top()->transformation);
				curNodes.pop();
			}
			mat++;
		}
	}
}
void Model::CompleteModelBuilder::build(const DXMain & dx, Model * model) const
{
	DxHandler::buildModel(dx, model, modelData.modelData, getModelBuildBuilder());
}
ModelBuilderData::ModelBuilderData(ModelData && modelData) :modelData(std::move(modelData))
{
}
Matrix f2m(const Float4x4 &f)
{
	return Matrix(f.val);
}
GET_SET_METHOD_IMPL(subsetStart, std::list<SubSet>, Model);
SAME_SET_METHOD_IMPL(vertex, void*, Model);
SAME_SET_METHOD_IMPL(index, void*, Model);
GET_SET_METHOD_IMPL(isDWord, bool, Model);
GET_SET_METHOD_IMPL(boneSize, unsigned int, ModelBuildBuilder);
GET_SET_METHOD_IMPL(modelIndex, int, ModelBuildBuilder);
GET_SET_METHOD_IMPL(dataToCreate, ModelBuildBuilder::DataType, ModelBuildBuilder);