set(RESOURCE_MANAGER_SOURCE
	ModelData.cpp
	DataManager.cpp
	inc/DataManagerT.cpp
	FileData.cpp
	ResourceData.cpp
	)
set(RESOURCE_MANAGER_HEADER
	inc/DataManager.h
	inc/FileData.h
	inc/ModelData.h
	inc/SubSet.h
	ModelDataHelper.h
	ResourceData.h
	)
ADD_LIBRARY(dxResourceManager STATIC ${COMMON_SOURCE} ${RESOURCE_MANAGER_SOURCE} ${RESOURCE_MANAGER_HEADER} ${LZ4_CPP})
target_include_directories(dxResourceManager PUBLIC ${DxEngine_SOURCE_DIR}/dxResourceManager/inc)