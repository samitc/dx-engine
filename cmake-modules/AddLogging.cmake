include(ExternalProject)
find_package(Git)
set(LOGGING_PREFIX "${ENGINE_DEPENDENCY_ROOT_DIR}/logging")
set(LOGGING_CMAKE_ARGS
        "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
if (NOT GIT_FOUND)
    set(AddMLOGGING_FOUND false CACHE BOOL "Was logging setup correctly?")
else (NOT GIT_FOUND)
    set(AddLOGGING_FOUND true CACHE BOOL "Was logging setup correctly?")
    ExternalProject_Add(logging
            URL https://github.com/samitc/logging/archive/4bc8b00b7da9655217a0c94d2656b91ece95c8b3.zip
            TIMEOUT 10
            PREFIX "${LOGGING_PREFIX}"
            CMAKE_ARGS "${LOGGING_CMAKE_ARGS}"
            LOG_DOWNLOAD ON
            LOG_CONFIGURE ON
            LOG_BUILD ON
            INSTALL_COMMAND ""
            )
    set(LIB_PREFIX "${CMAKE_STATIC_LIBRARY_PREFIX}")
    set(LIB_SUFFIX "${CMAKE_STATIC_LIBRARY_SUFFIX}")
    set(LOGGING_DEBUG_LIBRARIES
            "${LIB_PREFIX}log${CMAKE_DEBUG_POSTFIX}${LIB_SUFFIX}"
            "${CMAKE_THREAD_LIBS_INIT}")
    SET(LOGGING_RELEASE_LIBRARIES
            "${LIB_PREFIX}log${LIB_SUFFIX}"
            "${CMAKE_THREAD_LIBS_INIT}")
    set(CCONS_DEBUG_LIBRARIES
            "${DEBUG_LIB_DIR}/${LIB_PREFIX}ccons")
    SET(CCONS_RELEASE_LIBRARIES
            "${RELEASE_LIB_DIR}/${LIB_PREFIX}ccons")
    ExternalProject_Get_Property(logging source_dir)
    include_directories(${source_dir}/Log)
    ExternalProject_Get_Property(logging binary_dir)
    include_directories(${binary_dir}/ccons/src/ccons/Src)
    link_directories(${binary_dir})
    link_directories(${binary_dir}/ccons/src/ccons-build/)
endif (NOT GIT_FOUND)