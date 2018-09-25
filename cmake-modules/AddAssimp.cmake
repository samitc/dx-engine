find_package(Threads REQUIRED)
include(ExternalProject)

if (MSYS OR MINGW)
    set(DISABLE_PTHREADS ON)
else ()
    set(DISABLE_PTHREADS OFF)
endif ()

if (MSVC)
    set(RELEASE_LIB_DIR ReleaseLibs)
    set(DEBUG_LIB_DIR DebugLibs)
elseif (XCODE_VERSION)
    set(RELEASE_LIB_DIR Release)
    set(DEBUG_LIB_DIR Debug)
else ()
    set(RELEASE_LIB_DIR "")
    set(DEBUG_LIB_DIR "")
endif ()
set(ASSIMP_CMAKE_ARGS
        "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
        "-DBUILD_SHARED_LIBS=OFF"
        "-DBUILD_FRAMEWORK=OFF"
        "-DASSIMP_ANDROID_JNIIOSYSTEM=OFF"
        "-DASSIMP_NO_EXPORT=ON"
        "-DASSIMP_BUILD_ASSIMP_TOOLS=OFF"
        "-DASSIMP_BUILD_TESTS=OFF"
        "-DLIBRARY_SUFFIX="
        "-DCMAKE_DEBUG_POSTFIX=${CMAKE_DEBUG_POSTFIX}")
if (MSVC)
    set(ASSIMP_CMAKE_ARGS ${ASSIMP_CMAKE_ARGS}
            "-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=${DEBUG_LIB_DIR}"
            "-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=${RELEASE_LIB_DIR}")
endif ()

set(ASSIMP_PREFIX "${ENGINE_DEPENDENCY_ROOT_DIR}/assimp")

find_package(Git)
if (NOT GIT_FOUND)
    set(AddAssimp_FOUND false CACHE BOOL "Was assimp setup correctly?")
else (NOT GIT_FOUND)
    set(AddAssimp_FOUND true CACHE BOOL "Was assimp setup correctly?")

    ExternalProject_Add(assimp
            URL https://github.com/assimp/assimp/archive/v4.1.0.zip
            TIMEOUT 600
            PREFIX "${ASSIMP_PREFIX}"
            CMAKE_ARGS "${ASSIMP_CMAKE_ARGS}"
            LOG_DOWNLOAD ON
            LOG_CONFIGURE ON
            LOG_BUILD ON
            # Disable install
            INSTALL_COMMAND ""
            )

    set(LIB_PREFIX "${CMAKE_STATIC_LIBRARY_PREFIX}")
    set(LIB_SUFFIX "${CMAKE_STATIC_LIBRARY_SUFFIX}")
    set(ASSIMP_DEBUG_LIBRARIES
            "${LIB_PREFIX}assimp${CMAKE_DEBUG_POSTFIX}${LIB_SUFFIX}"
            "${CMAKE_THREAD_LIBS_INIT}")
    SET(ASSIMP_RELEASE_LIBRARIES
            "${LIB_PREFIX}assimp${LIB_SUFFIX}"
            "${CMAKE_THREAD_LIBS_INIT}")
    set(ASSIMP_IRR_DEBUG_LIBRARIES
            "${LIB_PREFIX}IrrXML${CMAKE_DEBUG_POSTFIX}${LIB_SUFFIX}"
            "${CMAKE_THREAD_LIBS_INIT}")
    SET(ASSIMP_IRR_RELEASE_LIBRARIES
            "${LIB_PREFIX}IrrXML${LIB_SUFFIX}"
            "${CMAKE_THREAD_LIBS_INIT}")
    set(ASSIMP_Z_DEBUG_LIBRARIES
            "${LIB_PREFIX}zlibstatic${CMAKE_DEBUG_POSTFIX}${LIB_SUFFIX}"
            "${CMAKE_THREAD_LIBS_INIT}")
    SET(ASSIMP_Z_RELEASE_LIBRARIES
            "${LIB_PREFIX}zlibstatic${LIB_SUFFIX}"
            "${CMAKE_THREAD_LIBS_INIT}")
    ExternalProject_Get_Property(assimp source_dir)
    include_directories(${source_dir}/include)
    include_directories(${source_dir}/include/assimp)
    ExternalProject_Get_Property(assimp binary_dir)
    link_directories(${binary_dir}/code)
    link_directories(${binary_dir}/code/${RELEASE_LIB_DIR})
    link_directories(${binary_dir}/code/${DEBUG_LIB_DIR})
    link_directories(${binary_dir}/contrib/irrXML/${RELEASE_LIB_DIR})
    link_directories(${binary_dir}/contrib/irrXML/${DEBUG_LIB_DIR})
    link_directories(${binary_dir}/contrib/zlib/${RELEASE_LIB_DIR})
    link_directories(${binary_dir}/contrib/zlib/${DEBUG_LIB_DIR})
    include_directories(${binary_dir}/include)
endif (NOT GIT_FOUND)