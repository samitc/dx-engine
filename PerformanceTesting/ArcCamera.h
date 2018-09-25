/*
    Beginning DirectX 11 Game Programming
    By Allen Sherrod and Wendy Jones

    ArcCamera - Demonstrates a camera with arc rotation.
*/


#ifndef _ARC_CAM_H_
#define _ARC_CAM_H_

#include<DirectXMath.h>
using namespace DirectX;

class ArcCamera
{
    public:
        ArcCamera( );
private:

public:
        XMMATRIX GetViewMatrix( );

    private:
        XMFLOAT3 position_;
        XMFLOAT3 target_;

        float distance_, minDistance_, maxDistance_;
        float xRotation_, yRotation_, yMin_, yMax_;
};


#endif