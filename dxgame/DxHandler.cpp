#include "DxHandler.h"
#include <queue>
#include <process.h>
#include "Engine.h"
#include "ResourceException.h"
#include "BaseLog.h"
template<class T, class S> int getUseSize(T &dat, S defaultVal)
{
	int count = 0;
	S* s = (S*)&dat;
	while (count<sizeof T / sizeof S&&*s != defaultVal)
	{
		++count;
		++s;
	}
	return count;
}
void buildModel(const DXMain &dx, const ModelData & data, const ModelBuildBuilder &builder, Model *model)
{
	int sizeOfVertex = 0;
	int numberOfIndex = 0;
	int numberOfVertex = 0;
	auto MAX_BONES = builder.getboneSize();
	bool normalExsist = false;
	bool textureExsist = false;
	bool bonesExsist = false;
	const auto &fModel = data.getModels()[builder.getmodelIndex()];
	int nDefVertex = getUseSize(fModel.vertex[0], std::numeric_limits<float>::infinity());
	int nDefNormal = 0;
	int nDefTexture = 0;
	if (fModel.vertex.size()>0)
	{
		sizeOfVertex += nDefVertex * sizeof(float);
		numberOfVertex = fModel.vertex.size();
	}
	if (fModel.normal.size() > 0 && (builder.getdataToCreate()&ModelBuildBuilder::DataType::normal) == ModelBuildBuilder::DataType::normal)
	{
		nDefNormal = getUseSize(fModel.normal[0], std::numeric_limits<float>::infinity());
		sizeOfVertex += nDefNormal * sizeof(float);
		if (fModel.normal.size() != numberOfVertex)
		{
			throw "error:number of vertex!=number of normals";
		}
		normalExsist = true;
	}
	if (fModel.texture.size() > 0 && (builder.getdataToCreate()&ModelBuildBuilder::DataType::texture) == ModelBuildBuilder::DataType::texture)
	{
		nDefTexture = getUseSize(fModel.texture[0], std::numeric_limits<float>::infinity());
		sizeOfVertex += nDefTexture * sizeof(float);
		if (fModel.texture.size() != numberOfVertex)
		{
			throw "error:number of vertex!=number of texture";
		}
		textureExsist = true;
	}
	if (fModel.index.size()>0)
	{
		numberOfIndex = fModel.index.size();
	}
	if (fModel.bonesDat.size() > 0 && (builder.getdataToCreate()&ModelBuildBuilder::DataType::bones) == ModelBuildBuilder::DataType::bones)
	{
		sizeOfVertex += MAX_BONES*(sizeof(unsigned int) + sizeof(float));
		bonesExsist = true;
	}
	char *dat = new char[sizeOfVertex*numberOfVertex];
	void *finalData = dat;
	ModelDat::BoneDat::BoneWeight *tempW = (ModelDat::BoneDat::BoneWeight *)alloca(MAX_BONES * sizeof(ModelDat::BoneDat::BoneWeight));
	for (size_t i = 0; i < fModel.vertex.size(); i++)
	{
		memcpy(dat, &fModel.vertex[i], nDefVertex * sizeof(float));
		dat += nDefVertex * sizeof(float);
		if (normalExsist)
		{
			memcpy(dat, &fModel.normal[i], nDefNormal * sizeof(float));
			dat += nDefNormal * sizeof(float);
		}
		if (textureExsist)
		{
			memcpy(dat, &fModel.texture[i], nDefTexture * sizeof(float));
			dat += nDefTexture * sizeof(float);
		}
		if (bonesExsist)
		{
			auto bones = fModel.bonesDat[i];
			if (bones.weights.size()>0)
			{
				if (bones.weights.size()<MAX_BONES)
				{
					memcpy(tempW, &bones.weights[0], bones.weights.size() * sizeof(ModelDat::BoneDat::BoneWeight));
					memset(tempW + bones.weights.size(), 0, (MAX_BONES - bones.weights.size())*(sizeof(unsigned int) + sizeof(float)));
				}
				else
				{
					memcpy(tempW, &bones.weights[0], MAX_BONES * sizeof(ModelDat::BoneDat::BoneWeight));
				}
			}
			else
			{
				memset(tempW + bones.weights.size(), 0, MAX_BONES*(sizeof(unsigned int) + sizeof(float)));
			}
			char *p = dat + sizeof(unsigned int)*MAX_BONES;
			for (size_t i = 0; i < MAX_BONES; i++)
			{
				*((float*)p) = tempW[i].weight;
				*((unsigned int*)dat) = tempW[i].boneIndex;
				dat += sizeof(unsigned int);
				p += sizeof(float);
			}
			dat += sizeof(float)*MAX_BONES;
		}
	}
	Model::ModelBuilder mBuilder(sizeOfVertex, numberOfVertex, finalData);
	if (numberOfIndex == 0)
	{
		mBuilder.setIndexes((WORD*)nullptr, 0);
	}
	else
	{
		mBuilder.setIndexes(&fModel.index[0], numberOfIndex);
	}
	if (fModel.subset.size()>0)
	{
		for (const auto& subset : fModel.subset)
		{
			mBuilder.addSubSet(subset);
		}
	}
	mBuilder.build(dx, model);
}
DxHandler::DxHandler(const DXMain& dx) :newResourceEvent(CreateEventA(NULL, false, false, resourceEventName)), model(nullptr), m(nullptr), dx(dx), thread(&DxHandler::resourceLoadThreadRun, this), exitFlag(false), dataReady()
{
}
DxHandler::~DxHandler()
{
	exitFlag.store(true, std::memory_order_relaxed);
	SetEvent(this->newResourceEvent);
	thread.join();
	CloseHandle(newResourceEvent);
}
void DxHandler::loadModel(const char * modelLocation, Model *m, const ModelBuildBuilder &builder)
{
	if (m != nullptr)
	{
		m->setHandle(INVALID_HANDLE_VALUE);
	}
	int l = strlen(modelLocation) + 1;
	char* newModel = new char[l];
	strcpy(newModel, modelLocation);
	ModelBuildBuilder builderTemp{ builder };
	while (dataReady.test_and_set(std::memory_order_acq_rel));
	model = newModel;
	modelBuilder = std::move(builderTemp);
	this->m = m;
	SetEvent(this->newResourceEvent);
}
ModelData DxHandler::getModelData(const char *loc) const
{
	auto it=std::find_if(loadedModelData.cbegin(), loadedModelData.cend(), [loc](const auto& model)
	{
		return strcmp(loc, model.modelLocation) == 0;
	});
	if (it==loadedModelData.cend())
	{
		return ModelData();
	}
	else
	{
		return (*it).data;
	}
}
void DxHandler::buildModel(const DXMain & dx, Model * model, const ModelData & data, const ModelBuildBuilder & builder)
{
	::buildModel(dx, data, builder, model);
}
struct WaitForProcess
{
	HANDLE wait;
	HANDLE respone;
};
void DxHandler::resourceLoadThreadRun()
{
	std::vector<HANDLE> waitHandle;
	waitHandle.reserve(MAXIMUM_WAIT_OBJECTS);
	std::vector<HANDLE> responeHandle;
	responeHandle.reserve(MAXIMUM_WAIT_OBJECTS - 1);
	std::vector<ModelHandleData> modelData;
	modelData.reserve(MAXIMUM_WAIT_OBJECTS - 1);
	waitHandle.push_back(this->newResourceEvent);
	std::queue<WaitForProcess> waitForProcess;
	while (true)
	{
		DWORD wakeHandle = WaitForMultipleObjects(waitHandle.size(), &waitHandle[0], false, INFINITE);
		wakeHandle -= WAIT_OBJECT_0;
		if (wakeHandle == 0)
		{
			if (!exitFlag.load(std::memory_order_relaxed))
			{
				WaitForProcess w;
				if (this->startModelLoad(w.wait, w.respone, modelData))
				{
					if (waitHandle.size() == MAXIMUM_WAIT_OBJECTS)
					{
						waitForProcess.push(w);
					}
					else
					{
						waitHandle.push_back(w.wait);
						responeHandle.push_back(w.respone);
					}
				}
			}
		}
		else
		{
			if (!exitFlag.load(std::memory_order_relaxed))
			{
				this->wakeFromModelLoad(dx, wakeHandle - 1, responeHandle, modelData);
			}
			removeLoadedModel(wakeHandle - 1, waitHandle, responeHandle, modelData);
			if (!waitForProcess.empty())
			{
				WaitForProcess w = waitForProcess.front();
				waitForProcess.pop();
				waitHandle.push_back(w.wait);
				responeHandle.push_back(w.respone);
			}
		}
		if (waitHandle.size() == 1 && exitFlag.load(std::memory_order_relaxed))
		{
			break;
		}
	}
}
void DxHandler::wakeFromModelLoad(const DXMain& dx, DWORD wakeIndex, std::vector<HANDLE>& resH, std::vector<ModelHandleData>&dat)
{
	std::string logStr("create model:");
	logStr.append(dat[wakeIndex].modelLocation);
	BaseLog::getInstance().debug(logStr.c_str());
	HANDLE data = OpenFileMappingA(PAGE_READWRITE, false, dat[wakeIndex].fileData);
	void *dataPtr = MapViewOfFile(data, FILE_MAP_READ, 0, 0, 0);
	ModelData m;
	m.readFromBuffer(dataPtr);
	UnmapViewOfFile(dataPtr);
	CloseHandle(data);
	SetEvent(resH[wakeIndex]);
	int countOfModels = dat[wakeIndex].models.size();
	for (size_t i = 0; i < dat[wakeIndex].models.size(); i++)
	{
		if (dat[wakeIndex].models[i].model != nullptr)
		{
			buildModel(dx, dat[wakeIndex], m, i);
			SetEvent(dat[wakeIndex].models[i].modelHandle);
		}
	}
	loadedModelData.push_back(LoadedModelData(dat[wakeIndex].modelLocation, std::move(m)));
	dat[wakeIndex].modelLocation = nullptr;
	if (countOfModels==0)
	{
		CloseHandle(resH[wakeIndex]);
	}
}
void DxHandler::removeLoadedModel(DWORD wakeIndex, std::vector<HANDLE>&wakeH, std::vector<HANDLE>&resH, std::vector<ModelHandleData>&dat)
{
	CloseHandle(*(wakeH.cbegin() + (wakeIndex + 1)));
	wakeH.erase(wakeH.begin() + (wakeIndex + 1));
	resH.erase(resH.begin() + wakeIndex);
	dat.erase(dat.begin() + wakeIndex);
}
template<class T> int testModelExist(const std::vector<T> &data, const char* name)
{
	auto size = data.size();
	for (size_t i = 0; i < size; i++)
	{
		if (strcmp(name,data[i].modelLocation)==0)
		{
			return i;
		}
	}
	return -1;
}
bool DxHandler::startModelLoad(HANDLE& waitHandle, HANDLE& responeHandle, std::vector<ModelHandleData>& modelData)
{
	char fileNameTemp[_MAX_PATH * 2];
	ModelHandleData dat;
	Model *m = this->m;
	dat.eventName = nullptr;
	dat.eventResponeName = nullptr;
	dat.fileData = nullptr;
	dat.modelLocation = this->model;
	ModelBuildBuilder modelBuilder = std::move(this->modelBuilder);
	this->dataReady.clear(std::memory_order_relaxed);
	int index = testModelExist(loadedModelData, dat.modelLocation);
	if (index != -1)
	{
		if (m != nullptr)
		{
			buildModel(dx, m, loadedModelData[index].data, modelBuilder);
			m->setHandle(nullptr);
		}
	}
	else if ((index = testModelExist(modelData, dat.modelLocation)) != -1)
	{
		if (m != nullptr)
		{
			HANDLE newHandle = CreateEvent(nullptr, true, false, nullptr);
			m->setHandle(newHandle);
			ModelHandleData::ModelsPackData pack;
			pack.builder = std::move(modelBuilder);
			pack.model = m;
			pack.modelHandle = newHandle;
			modelData[index].models.push_back(std::move(pack));
		}
	}
	else
	{
		HANDLE newHandle = CreateEvent(nullptr, true, false, nullptr);
		if (m!=nullptr)
		{
			m->setHandle(newHandle);
		}
		std::string logStr("load model:");
		logStr.append(dat.modelLocation);
		BaseLog::getInstance().debug(logStr.c_str());
		char *c = dat.modelLocation;
		char *p = fileNameTemp;
		while (*c != 0)
		{
			if (*c != FOLDER_SEPERATOR)
			{
				*p = *c;
				p++;
			}
			c++;
		}
		*p = NULL;
		int l = strlen(fileNameTemp) + 1;
		dat.eventName = new char[l];
		strcpy(dat.eventName, fileNameTemp);
		dat.eventResponeName = new char[l + 1];
		strcpy(dat.eventResponeName, fileNameTemp);
		dat.eventResponeName[l - 1] = 'o';
		dat.eventResponeName[l] = 0;
		waitHandle = CreateEventA(nullptr, false, false, dat.eventName);
		responeHandle = CreateEventA(nullptr, true, false, dat.eventResponeName);
		std::string arg("AssestImport.exe ImportExternal \"");
		arg.append(dat.modelLocation);
		arg.append("\"");
		dat.fileData = new char[strlen("ImportExternal") + strlen(dat.modelLocation) + 1];
		strcpy(dat.fileData, "ImportExternal");
		strcat(dat.fileData, fileNameTemp);
		STARTUPINFO info = { sizeof(info) };
		PROCESS_INFORMATION processInfo;
		if (!CreateProcessA("AssestImport.exe", (LPSTR)arg.c_str(), nullptr, nullptr, false, 0, nullptr, nullptr, (LPSTARTUPINFOA)&info, &processInfo))
		{
			BaseLog::getInstance().error("can not create assest import process");
		}
		CloseHandle(processInfo.hThread);
		CloseHandle(processInfo.hProcess);
		ModelHandleData::ModelsPackData pack;
		pack.builder = std::move(modelBuilder);
		pack.model = m;
		pack.modelHandle = newHandle;
		dat.models.push_back(std::move(pack));
		modelData.push_back(std::move(dat));
		return true;
	}
	return false;
}
void DxHandler::buildModel(const DXMain& dx, ModelHandleData & model, ModelData & data, int index)
{
	try
	{
		buildModel(dx, model.models[index].model, data, model.models[index].builder);
	}
	catch (const char*error)
	{
		std::string logStr("model loading:");
		logStr.append(model.modelLocation);
		logStr.append(". ");
		logStr.append(error);
		BaseLog::getInstance().error(logStr.c_str());
		throw ResourceException(logStr.c_str());
	}
}
DxHandler::ModelHandleData::ModelHandleData(ModelHandleData &&m) :models(std::move(m.models)), modelLocation(m.modelLocation), eventName(m.eventName), eventResponeName(m.eventResponeName), fileData(m.fileData)
{
	m.modelLocation = nullptr;
	m.eventName = nullptr;
	m.eventResponeName = nullptr;
	m.fileData = nullptr;
}
DxHandler::ModelHandleData & DxHandler::ModelHandleData::operator=(ModelHandleData &&m)
{
	if (this != &m)
	{
		models = std::move(m.models);
		modelLocation = m.modelLocation;
		eventName = m.eventName;
		eventResponeName = m.eventResponeName;
		fileData = m.fileData;
		m.modelLocation = nullptr;
		m.eventName = nullptr;
		m.eventResponeName = nullptr;
		m.fileData = nullptr;
	}
	return *this;
}
DxHandler::ModelHandleData::~ModelHandleData()
{
	delete[] modelLocation;
	delete[] eventName;
	delete[] eventResponeName;
	delete[] fileData;
}
DxHandler::LoadedModelData::LoadedModelData(char *loc, ModelData &&data) :modelLocation(loc), data(std::move(data))
{
}