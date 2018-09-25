cmake_minimum_required(VERSION 3.11)
project(DxEngine)
add_definitions("/wd4005 /wd4838 /wd4996 /wd4305")
set(ENGINE_RENDER_API "DX_11" CACHE STRING "use DX_9 for directx 9, DX_11 for directx 11 and DX_12 for directx 12" )
set(ENGINE_CHECKING_LEVEL "CLDEBUG" CACHE STRING "how much the engine should check.any level define all the level after in the order. options are (with order) CLDEBUG CLRELEASE0 CLRELEASE1 CLRELEASE2 CLRELEASE3")
set(COMMON_SOURCE)
set(DXMAIN_SOURCE
	DxMain/Main.cpp
	)
set(DXMAIN_HEADER
	DxMain/Global.h
	DxMain/Defines.h
	)
set(ENGINE_DEPENDENCY_ROOT_DIR ${CMAKE_BINARY_DIR}/ CACHE STRING "")
if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
    set(BUILD_ARCH "x86")
else()
	set(BUILD_ARCH "x64")
endif()
set(CMAKE_DEBUG_POSTFIX "d")
include_directories(${DxEngine_SOURCE_DIR}/Include)
add_definitions(-D_UNICODE -DUNICODE)# to use w windows versions
if(ENGINE_RENDER_API STREQUAL "")
	message(SEND_ERROR "engine render api not set")
endif()
add_definitions(-D${ENGINE_RENDER_API})
add_definitions(-D${ENGINE_CHECKING_LEVEL})
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")
file(COPY
    ${PROJECT_SOURCE_DIR}/DxMain/pinkColor.fx
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(COPY
    ${PROJECT_SOURCE_DIR}/DxMain/color.fx
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(COPY
    ${PROJECT_SOURCE_DIR}/DxMain/Box.FBX
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(COPY
    ${PROJECT_SOURCE_DIR}/DxMain/dx.xml
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
INCLUDE(AddGTest)
Include(AddManMem)
include(AddLogging)
include(AddLZ4)
include(AddEffect)
ADD_SUBDIRECTORY(dxResourceManager)
ADD_SUBDIRECTORY(DxMath)
ADD_SUBDIRECTORY(DxWrapper)
ADD_SUBDIRECTORY(dxgame)
ADD_SUBDIRECTORY(AssestImport)
ADD_SUBDIRECTORY(GeneralTesting)
ADD_SUBDIRECTORY(PerformanceTesting)
add_executable(dxMain WIN32 ${COMMON_SOURCE} ${DXMAIN_SOURCE} ${DXMAIN_HEADER})
add_dependencies(dxMain dxEngine dxWrapper assestImport)
target_link_libraries(dxMain dxEngine dxWrapper)