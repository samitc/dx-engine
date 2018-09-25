set(GENEARL_TESTING_SOURCE
	MathTesting.cpp
	TimeTesting.cpp
	InputLayoutTesting.cpp
	GObjectTesting.cpp
    GeneralTesting.cpp
	)
set(GENEARL_TESTING_HEADER
	GraphicStateMock.h
	GameTimeMock.h
	GObjectMock.h
	GUbjectMock.h
    GUMock.h
	)
if (AddGTest_FOUND)
    add_executable(generalTesting ${COMMON_SOURCE} ${GENEARL_TESTING_SOURCE} ${GENEARL_TESTING_HEADER})
    add_custom_command(TARGET generalTesting PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PROJECT_SOURCE_DIR}/DxMain/dx.xml $<TARGET_FILE_DIR:generalTesting>)
    add_dependencies(generalTesting dxEngine)
    target_link_libraries(generalTesting dxEngine)
    add_dependencies(generalTesting gtest)
    target_link_libraries(generalTesting 
		debug ${GTEST_DEBUG_LIBRARIES}
		optimized ${GTEST_RELEASE_LIBRARIES})
endif (AddGTest_FOUND)