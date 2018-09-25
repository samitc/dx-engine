set(ASSEST_IMPORT_SOURCE
	ModelImporter.cpp
	Main.cpp
	Importer.cpp
	File.cpp
	Exporter.cpp
	ExternalImporter.cpp
	Command.cpp
	AssimpT.cpp
	)
set(ASSEST_IMPORT_HEADER
	ModelImporter.h
	File.h
	ExternalImporter.h
	AssimpT.h
	Importer.h
	Exporter.h
	Command.h
	)
include(AddAssimp)
if(AddAssimp_FOUND)
    add_executable(assestImport WIN32 ${COMMON_SOURCE} ${ASSEST_IMPORT_SOURCE} ${ASSEST_IMPORT_HEADER})
    add_custom_command(TARGET assestImport POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:assestImport> ${PROJECT_BINARY_DIR})
    target_link_libraries(assestImport dxResourceManager)
    add_dependencies(assestImport assimp)
    target_link_libraries(assestImport
		debug ${ASSIMP_DEBUG_LIBRARIES}
		optimized ${ASSIMP_RELEASE_LIBRARIES})
    target_link_libraries(assestImport
		debug ${ASSIMP_IRR_DEBUG_LIBRARIES}
		optimized ${ASSIMP_IRR_RELEASE_LIBRARIES})
    target_link_libraries(assestImport
		debug ${ASSIMP_Z_DEBUG_LIBRARIES}
		optimized ${ASSIMP_Z_RELEASE_LIBRARIES})
endif()