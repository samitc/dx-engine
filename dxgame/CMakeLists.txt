set(DXENGINE_SOURCE
	State.cpp
	ShaderState.cpp
	PipeLineState.cpp
	Model.cpp
	Material.cpp
	InstanceModel.cpp
	GameTime.cpp
	Game.cpp
	GObjectContainer.cpp
	GObject.cpp
	GOEntity.cpp
	DxHandler.cpp
	DXMain.cpp
	AnimateModel.cpp
	BaseLog.cpp
	GeneralException.cpp
	ResourceException.cpp
	DxResException.cpp
	BufferSizeNotMatch.cpp
	DXException.cpp
	BufferCreateException.cpp
	EObject.cpp
	UObject.cpp
	)
set(DXENGINE_HEADER
	inc/BufferCreateException.h
	inc/DXException.h
	inc/BufferException.h
	inc/DxResException.h
	inc/DXMain.h
	inc/Game.h
	inc/GeneralException.h
	inc/GObject.h
	inc/GOEntity.h
	inc/GPUException.h
	inc/Model.h
	inc/PipeLineState.h
	inc/ResourceException.h
	inc/ShaderState.h
	inc/State.h
	inc/EObject.h
	inc/UObject.h
	inc/GameTime.h
	DxHandler.h
	Material.h
	InstanceModel.h
	BufferSizeNotMatch.h
	AnimateModel.h
	GObjectContainer.h
	)
if(AddMANMEM_FOUND AND AddLOGGING_FOUND)
    ADD_LIBRARY(dxEngine STATIC ${COMMON_SOURCE} ${DXENGINE_SOURCE} ${DXENGINE_HEADER})
    add_dependencies(dxEngine dxWrapper dxMath dxResourceManager)
    target_link_libraries(dxEngine dxResourceManager dxWrapper dxMath)
    target_include_directories(dxEngine PUBLIC ${DxEngine_SOURCE_DIR}/dxgame/inc)
    add_dependencies(dxEngine manmem)
    target_link_libraries(dxEngine
		debug ${MANMEM_DEBUG_LIBRARIES}
		optimized ${MANMEM_RELEASE_LIBRARIES})
    add_dependencies(dxEngine logging)
    target_link_libraries(dxEngine
		debug ${LOGGING_DEBUG_LIBRARIES}
		optimized ${LOGGING_RELEASE_LIBRARIES})
    target_link_libraries(dxEngine
		debug ${CCONS_DEBUG_LIBRARIES}
		optimized ${CCONS_RELEASE_LIBRARIES})
endif()