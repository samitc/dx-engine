include(FetchContent)
set(LZ4_PREFIX "${ENGINE_DEPENDENCY_ROOT_DIR}/lz4")
FetchContent_Declare(lz4
        URL https://github.com/lz4/lz4/archive/v1.8.2.zip
        )
FetchContent_GetProperties(lz4)
if(NOT lz4_POPULATED)
  FetchContent_Populate(lz4)
endif()
set(LZ4_CPP ${lz4_SOURCE_DIR}/lib/lz4.c)
include_directories(${lz4_SOURCE_DIR}/lib)