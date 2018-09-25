set(MATH_SOURCE
	Vector4D.cpp
	Vector3D.cpp
	Vector2D.cpp
	Vector.cpp
	Matrix.cpp
	Color.cpp
	)
set(MATH_HEADER
	inc/Color.h
	inc/MathB.h
	inc/matrix.h
	inc/Vector.h
	Vector2D.h
	Vector4D.h
	Vector3D.h
	)
ADD_LIBRARY(dxMath STATIC ${COMMON_SOURCE} ${MATH_SOURCE} ${MATH_HEADER})
add_dependencies(dxMath manmem logging)
target_include_directories(dxMath PRIVATE ${DxEngine_SOURCE_DIR}/dxgame/inc)#for GeneralException
target_include_directories(dxMath PUBLIC ${DxEngine_SOURCE_DIR}/DxMath/inc)