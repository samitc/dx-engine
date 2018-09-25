#ifndef _DXHANDLER_H_
#define _DXHANDLER_H_
#include <Windows.h>
#include <vector>
#include <thread>
#include <atomic>
#include "Engine.h"
#include "ModelData.h"
#include "Model.h"
class DXMain;
class DxHandler//for engine
{
public:
	DxHandler(const DXMain& dx);
	~DxHandler();
	void loadModel(const char*, Model *, const ModelBuildBuilder &builder);
	ModelData getModelData(const char*) const;
	static void buildModel(const DXMain& dx, Model *model, const ModelData &data, const ModelBuildBuilder &builder);
private:
	struct LoadedModelData
	{
		LoadedModelData(char*, ModelData&&);
		ModelData data;
		char *modelLocation;
	};
	struct ModelHandleData
	{
		struct ModelsPackData
		{
			Model* model;
			ModelBuildBuilder builder;
			HANDLE modelHandle;
		};
		ModelHandleData() = default;
		ModelHandleData(ModelHandleData&&);
		ModelHandleData& operator=(ModelHandleData&&);
		~ModelHandleData();
		char* modelLocation;
		char* eventName;
		char* eventResponeName;
		char* fileData;
		std::vector<ModelsPackData> models;
	};
	void resourceLoadThreadRun();
	void wakeFromModelLoad(const DXMain& dx, DWORD wakeIndex, std::vector<HANDLE>&, std::vector<ModelHandleData>&);
	static void removeLoadedModel(DWORD wakeIndex, std::vector<HANDLE>&, std::vector<HANDLE>&, std::vector<ModelHandleData>&);
	bool startModelLoad(HANDLE&, HANDLE&, std::vector<ModelHandleData>&);
	static void buildModel(const DXMain& dx, ModelHandleData& model, ModelData& data,int index);
	static CONSTEXPRN char* resourceEventName = "newResourceEvent";
	static CONSTEXPRN char FOLDER_SEPERATOR = '\\';
	const DXMain& dx;
	HANDLE newResourceEvent;
	char *model;
	Model* m;
	ModelBuildBuilder modelBuilder;
	std::thread thread;
	std::atomic_flag dataReady;
	std::atomic<bool> exitFlag;
	std::vector<LoadedModelData> loadedModelData;//todo: make thread safe vector
};
#endif