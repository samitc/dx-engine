#include"ModelsDemo.h"
#include<DirectXMath.h>
#include"objLoader.h"
struct VertexPos
{
    XMFLOAT3 pos;
};
ModelsDemo::ModelsDemo( ) : textureMapVS_( 0 ), textureMapPS_( 0 ), inputLayout_( 0 ),
                            vertexBuffer_( 0 ), colorMap_( 0 ), colorMapSampler_( 0 ),
                            viewCB_( 0 ), projCB_( 0 ), worldCB_( 0 ), totalVerts_( 0 ),
							cat1(0),cat2(0),dir1(true),dir2(false)
{
    
}
ModelsDemo::~ModelsDemo( )
{

}
bool ModelsDemo::LoadContent()
{
	ID3DBlob* vsBuffer = 0;

	bool compileResult = CompileD3DShader(L"WhiteColor.fx", "VS_Main", "vs_4_0", &vsBuffer);

	if (compileResult == false)
	{
		return false;
	}

	HRESULT d3dResult;

	d3dResult = d3dDevice_->CreateVertexShader(vsBuffer->GetBufferPointer(),
		vsBuffer->GetBufferSize(), 0, &textureMapVS_);

	if (FAILED(d3dResult))
	{

		if (vsBuffer)
			vsBuffer->Release();

		return false;
	}

	D3D11_INPUT_ELEMENT_DESC solidColorLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	unsigned int totalLayoutElements = ARRAYSIZE(solidColorLayout);

	d3dResult = d3dDevice_->CreateInputLayout(solidColorLayout, totalLayoutElements,
		vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &inputLayout_);

	vsBuffer->Release();

	if (FAILED(d3dResult))
	{
		return false;
	}

	ID3DBlob* psBuffer = 0;

	compileResult = CompileD3DShader(L"WhiteColor.fx", "PS_Main", "ps_4_0", &psBuffer);

	if (compileResult == false)
	{
		return false;
	}

	d3dResult = d3dDevice_->CreatePixelShader(psBuffer->GetBufferPointer(),
		psBuffer->GetBufferSize(), 0, &textureMapPS_);

	psBuffer->Release();

	if (FAILED(d3dResult))
	{
		return false;
	}

	{
		// Load the models from the file.
		ObjModel objModel;

		if (objModel.LoadOBJ("Sphere.obj") == false)
		{
			return false;
		}

		totalVerts_ = objModel.GetTotalVerts();

		VertexPos* vertices = new VertexPos[totalVerts_];
		float* vertsPtr = objModel.GetVertices();
		//float* texCPtr = objModel.GetTexCoords();

		for (int i = 0; i < totalVerts_; i++)
		{
			vertices[i].pos = XMFLOAT3(*(vertsPtr + 0), *(vertsPtr + 1), *(vertsPtr + 2));
			vertsPtr += 3;

			/*vertices[i].tex0 = XMFLOAT2(*(texCPtr + 0), *(texCPtr + 1));
			texCPtr += 2;*/
		}

		D3D11_BUFFER_DESC vertexDesc;
		ZeroMemory(&vertexDesc, sizeof(vertexDesc));
		vertexDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc.ByteWidth = sizeof(VertexPos) * totalVerts_;

		D3D11_SUBRESOURCE_DATA resourceData;
		ZeroMemory(&resourceData, sizeof(resourceData));
		resourceData.pSysMem = vertices;

		d3dResult = d3dDevice_->CreateBuffer(&vertexDesc, &resourceData, &vertexBuffer_);

		if (FAILED(d3dResult))
		{
			return false;
		}

		delete[] vertices;
		objModel.Release();

	}
	{
		// Load the models from the file.
		ObjModel objModel;

		if (objModel.LoadOBJ("cat.obj") == false)
		{
			return false;
		}

		totalVerts = objModel.GetTotalVerts();

		VertexPos* vertices = new VertexPos[totalVerts];
		float* vertsPtr = objModel.GetVertices();
		//float* texCPtr = objModel.GetTexCoords();

		for (int i = 0; i < totalVerts; i++)
		{
			vertices[i].pos = XMFLOAT3(*(vertsPtr + 0), *(vertsPtr + 1), *(vertsPtr + 2));
			vertsPtr += 3;

			/*vertices[i].tex0 = XMFLOAT2(*(texCPtr + 0), *(texCPtr + 1));
			texCPtr += 2;*/
		}

		D3D11_BUFFER_DESC vertexDesc;
		ZeroMemory(&vertexDesc, sizeof(vertexDesc));
		vertexDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc.ByteWidth = sizeof(VertexPos) * totalVerts;

		D3D11_SUBRESOURCE_DATA resourceData;
		ZeroMemory(&resourceData, sizeof(resourceData));
		resourceData.pSysMem = vertices;

		d3dResult = d3dDevice_->CreateBuffer(&vertexDesc, &resourceData, &vertexBuffer);

		if (FAILED(d3dResult))
		{
			return false;
		}

		delete[] vertices;
		objModel.Release();

	}
    D3D11_BUFFER_DESC constDesc;
	ZeroMemory( &constDesc, sizeof( constDesc ) );
	constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constDesc.ByteWidth = sizeof( XMMATRIX );
	constDesc.Usage = D3D11_USAGE_DEFAULT;

	d3dResult = d3dDevice_->CreateBuffer( &constDesc, 0, &viewCB_ );

	if( FAILED( d3dResult ) )
    {
        return false;
    }

    d3dResult = d3dDevice_->CreateBuffer( &constDesc, 0, &projCB_ );

	if( FAILED( d3dResult ) )
    {
        return false;
    }

    d3dResult = d3dDevice_->CreateBuffer( &constDesc, 0, &worldCB_ );

	if( FAILED( d3dResult ) )
    {
        return false;
    }

    projMatrix_ = XMMatrixPerspectiveFovLH( XM_PIDIV4, 800.0f / 600.0f, 0.01f, 100.0f );
    projMatrix_ = XMMatrixTranspose( projMatrix_ );


	XMMATRIX viewMat = camera_.GetViewMatrix();
	viewMat = XMMatrixTranspose(viewMat);
	d3dContext_->UpdateSubresource(viewCB_, 0, 0, &viewMat, 0, 0);
	d3dContext_->UpdateSubresource(projCB_, 0, 0, &projMatrix_, 0, 0);
	d3dContext_->VSSetConstantBuffers(1, 1, &viewCB_);
	d3dContext_->VSSetConstantBuffers(2, 1, &projCB_);
	d3dContext_->VSSetConstantBuffers(0, 1, &worldCB_);
	worldMat1 = XMMatrixTranspose(XMMatrixScaling(0.7, 0.7, 0.7)* XMMatrixTranslation(-4, 2.6, 0));
	worldMat2 = XMMatrixTranspose(XMMatrixScaling(0.7, 0.7, 0.7)* XMMatrixTranslation(4, 2.6, 0));
	worldMat3 = XMMatrixTranspose(XMMatrixScaling(0.7, 0.7, 0.7)* XMMatrixTranslation(-4, -2.6, 0));
	worldMat4 = XMMatrixTranspose(XMMatrixScaling(0.7, 0.7, 0.7)* XMMatrixTranslation(4, -2.6, 0));

	d3dContext_->IASetInputLayout(inputLayout_);
	d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	d3dContext_->VSSetShader(textureMapVS_, 0, 0);
	d3dContext_->PSSetShader(textureMapPS_, 0, 0);

    return true;
}


void ModelsDemo::UnloadContent( )
{
    if( colorMapSampler_ ) colorMapSampler_->Release( );
    if( colorMap_ ) colorMap_->Release( );
    if( textureMapVS_ ) textureMapVS_->Release( );
    if( textureMapPS_ ) textureMapPS_->Release( );
    if( inputLayout_ ) inputLayout_->Release( );
    if( vertexBuffer_ ) vertexBuffer_->Release( );
    if( viewCB_ ) viewCB_->Release( );
    if( projCB_ ) projCB_->Release( );
    if( worldCB_ ) worldCB_->Release( );

    colorMapSampler_ = 0;
    colorMap_ = 0;
    textureMapVS_ = 0;
    textureMapPS_ = 0;
    inputLayout_ = 0;
    vertexBuffer_ = 0;
    viewCB_ = 0;
    projCB_ = 0;
    worldCB_ = 0;
}


void ModelsDemo::Update( float dt )
{
	
}


const float max = 1;
const float min = -1;
const float clearColor[4] = { 0.0f, 0.0f, 0.25f, 1.0f };
void ModelsDemo::Render( )
{
    if( d3dContext_ == 0 )
        return;

    d3dContext_->ClearRenderTargetView( backBufferTarget_, clearColor );
    d3dContext_->ClearDepthStencilView( depthStencilView_, D3D11_CLEAR_DEPTH, 1.0f, 0 );

    unsigned int stride = sizeof( VertexPos );
    unsigned int offset = 0;
	XMMATRIX worldMat;

	d3dContext_->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);

    d3dContext_->UpdateSubresource( worldCB_, 0, 0, &worldMat1, 0, 0 );
    d3dContext_->Draw( totalVerts_, 0 );
	d3dContext_->UpdateSubresource(worldCB_, 0, 0, &worldMat2, 0, 0);
	d3dContext_->Draw(totalVerts_, 0);
	d3dContext_->UpdateSubresource(worldCB_, 0, 0, &worldMat3, 0, 0);
	d3dContext_->Draw(totalVerts_, 0);
	d3dContext_->UpdateSubresource(worldCB_, 0, 0, &worldMat4, 0, 0);
	d3dContext_->Draw(totalVerts_, 0);

	d3dContext_->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	worldMat = XMMatrixTranspose(XMMatrixTranslation(cat1, -0.5, 9));
	d3dContext_->UpdateSubresource(worldCB_, 0, 0, &worldMat, 0, 0);
	d3dContext_->Draw(totalVerts, 0);
	worldMat = XMMatrixTranspose(XMMatrixTranslation(cat2, -0.5, 9));
	d3dContext_->UpdateSubresource(worldCB_, 0, 0, &worldMat, 0, 0);
	d3dContext_->Draw(totalVerts, 0);

	if (cat1 >= max)
	{
		dir1 = false;
	}
	else if (cat1 <= min)
	{
		dir1 = true;
	}
	if (dir1)
	{
		cat1 += 0.001;
	}
	else
	{
		cat1 -= 0.001;
	}
	if (cat2 >= max)
	{
		dir2 = false;
	}
	else if (cat2 <= min)
	{
		dir2 = true;
	}
	if (dir2)
	{
		cat2 += 0.001;
	}
	else
	{
		cat2 -= 0.001;
	}

    swapChain_->Present( 0, 0 );
}