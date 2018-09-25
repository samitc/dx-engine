#include<d3d11.h>
#include"ArcCamera.h"
ArcCamera::ArcCamera( ) : target_( XMFLOAT3( 0.0f, 0.0f, 0.0f ) ),
    position_( XMFLOAT3( 0.0f, 0.0f, 0.0f ) )
{
	distance_ = 12;
	minDistance_ = 4;
	maxDistance_ = 20;

	xRotation_ = 0;
	yRotation_ = 0;
	yMin_ = -XM_PIDIV2;
	yMax_ = XM_PIDIV2;
}
XMMATRIX ArcCamera::GetViewMatrix( )
{
    XMVECTOR zoom = XMVectorSet( 0.0f, 0.0f, distance_, 1.0f );
    XMMATRIX rotation = XMMatrixRotationRollPitchYaw( xRotation_, -yRotation_, 0.0f );

    zoom = XMVector3Transform( zoom, rotation );

    XMVECTOR pos = XMLoadFloat3( &position_ );
    XMVECTOR lookAt = XMLoadFloat3( &target_ );

    pos = lookAt + zoom;
    XMStoreFloat3( &position_, pos );

    XMVECTOR up = XMVectorSet( 0.0f, 1.0f, 0.0f, 1.0f );
    up = XMVector3Transform( up, rotation );

    XMMATRIX viewMat = XMMatrixLookAtLH( pos, lookAt, up );

    return viewMat;
}