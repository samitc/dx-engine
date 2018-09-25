set(PERFORMANCE_TESTING_SOURCE
    PerformanceTesting.cpp
	objLoader.cpp
	TokenStream.cpp
	SmallGame.cpp
	ModelsDemo.cpp
	EmptyGame.cpp
	Dx11DemoBase.cpp
	ArcCamera.cpp
	)
set(PERFORMANCE_TESTING_HEADER
	ArcCamera.h
	objLoader.h
	TokenStream.h
	ModelsDemo.h
	Dx11DemoBase.h
	)
if (AddGTest_FOUND)
    add_executable(performanceTesting ${COMMON_SOURCE} ${PERFORMANCE_TESTING_SOURCE} ${PERFORMANCE_TESTING_HEADER})
    add_custom_command(TARGET performanceTesting PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/WhiteColor.fx $<TARGET_FILE_DIR:performanceTesting>)
    add_custom_command(TARGET performanceTesting PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/Sphere.obj $<TARGET_FILE_DIR:performanceTesting>)
    add_custom_command(TARGET performanceTesting PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/cat.obj $<TARGET_FILE_DIR:performanceTesting>)
    add_custom_command(TARGET performanceTesting PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PROJECT_SOURCE_DIR}/DxMain/dx.xml $<TARGET_FILE_DIR:performanceTesting>)
    add_custom_command(TARGET performanceTesting PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PROJECT_BINARY_DIR}/assestImport.exe $<TARGET_FILE_DIR:performanceTesting>)
    add_dependencies(performanceTesting dxEngine)
    target_link_libraries(performanceTesting dxEngine)
    add_dependencies(performanceTesting gtest)
    target_link_libraries(performanceTesting 
		debug ${GTEST_DEBUG_LIBRARIES}
		optimized ${GTEST_RELEASE_LIBRARIES})
endif (AddGTest_FOUND)