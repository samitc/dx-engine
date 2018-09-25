include(ExternalProject)
find_package(Git)
set(MANMEM_PREFIX "${ENGINE_DEPENDENCY_ROOT_DIR}/manmem")
set(MANMEM_CMAKE_ARGS
        "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
        "-DBUILD_TESTS=OFF")
if (NOT GIT_FOUND)
    set(AddMANMEM_FOUND false CACHE BOOL "Was manmem setup correctly?")
else (NOT GIT_FOUND)
    set(AddMANMEM_FOUND true CACHE BOOL "Was manmem setup correctly?")
    ExternalProject_Add(manmem
            URL https://github.com/samitc/manmem/archive/e5f9af8e3fe6c92cdf899a7ca39901d500476248.zip
            TIMEOUT 10
            PREFIX "${MANMEM_PREFIX}"
            CMAKE_ARGS "${MANMEM_CMAKE_ARGS}"
            LOG_DOWNLOAD ON
            LOG_CONFIGURE ON
            LOG_BUILD ON
            INSTALL_COMMAND ""
            )
    set(LIB_PREFIX "${CMAKE_STATIC_LIBRARY_PREFIX}")
    set(LIB_SUFFIX "${CMAKE_STATIC_LIBRARY_SUFFIX}")
    set(MANMEM_DEBUG_LIBRARIES
            "${LIB_PREFIX}manmem${CMAKE_DEBUG_POSTFIX}${LIB_SUFFIX}"
            "${CMAKE_THREAD_LIBS_INIT}")
    SET(MANMEM_RELEASE_LIBRARIES
            "${LIB_PREFIX}manmem${LIB_SUFFIX}"
            "${CMAKE_THREAD_LIBS_INIT}")
    ExternalProject_Get_Property(manmem source_dir)
    include_directories(${source_dir}/Src/Pools)
    ExternalProject_Get_Property(manmem binary_dir)
    link_directories(${binary_dir}/Release)
    link_directories(${binary_dir}/Debug)
endif (NOT GIT_FOUND)