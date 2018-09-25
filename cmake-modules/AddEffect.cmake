include(FetchContent)
set(EFFECT_PREFIX "${ENGINE_DEPENDENCY_ROOT_DIR}/effect")
if (${MSVC_VERSION} EQUAL 1900)
    set(SLN_FILE "Effects11_2015.sln")
    set(VS_VERSION 14.0)
    set(VS_PLATFORM v140)
elseif(${MSVC_VERSION} GREATER_EQUAL 1910 AND ${MSVC_VERSION} LESS_EQUAL 1919)
    set(SLN_FILE "Effects11_2017_Win10.sln")
    set(VS_VERSION 15.0)
    set(VS_PLATFORM v141)
endif()
FetchContent_Declare(effect
        URL https://github.com/Microsoft/FX11/archive/641019d95a4124e0a6cc5523f856b54568871069.zip
        )
FetchContent_GetProperties(effect)
if(NOT effect_POPULATED)
  FetchContent_Populate(effect)
endif()
string(REPLACE "\\" "/" MSBUILD_PATH ${CMAKE_VS_MSBUILD_COMMAND})
execute_process(
    COMMAND ${MSBUILD_PATH} ${effect_SOURCE_DIR}/${SLN_FILE} /p:OutDir=${effect_BINARY_DIR}/ /p:PlatformToolset=${VS_PLATFORM} /p:Configuration=Debug /p:VisualStudioVersion=${VS_VERSION} /p:WindowsTargetPlatformVersion=${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}
)
execute_process(
    COMMAND ${MSBUILD_PATH} ${effect_SOURCE_DIR}/${SLN_FILE} /p:OutDir=${effect_BINARY_DIR}/ /p:PlatformToolset=${VS_PLATFORM} /p:Configuration=Release /p:VisualStudioVersion=${VS_VERSION} /p:WindowsTargetPlatformVersion=${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}
)
set(LIB_PREFIX "${CMAKE_STATIC_LIBRARY_PREFIX}")
set(LIB_SUFFIX "${CMAKE_STATIC_LIBRARY_SUFFIX}")
set(EFFECT_DEBUG_LIBRARIES
        "${LIB_PREFIX}effects11${CMAKE_DEBUG_POSTFIX}${LIB_SUFFIX}"
        "${CMAKE_THREAD_LIBS_INIT}")
SET(EFFECT_RELEASE_LIBRARIES
        "${LIB_PREFIX}effects11${LIB_SUFFIX}"
        "${CMAKE_THREAD_LIBS_INIT}")
include_directories(${effect_SOURCE_DIR}/inc)
link_directories(${effect_BINARY_DIR})