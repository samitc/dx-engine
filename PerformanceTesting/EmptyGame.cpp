#include "gtest\gtest.h"
#include <Windows.h>
#include <iostream>
#include <D3Dcompiler.h>
#include "ModelsDemo.h"
#include "DXMain.h"
#include "Device.h"
#include "DXObject.h"
#include "DXMain.h"
#include "VideoAdapter.h"
#include "VideoOutput.h"

#include "VideoMode.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Model.h"
#include "Buffer.h"
#include "Matrix.h"
#include "Game.h"
#include "inputLayout.h"
#include "LayoutElement.h"
#include "ResourceManager.h"
#include "PipeShadersParams.h"
#include "DrawCommand.h"
#include "Sampler.h"
#include "ShaderResource.h"
#include "UObject.h"
#include "Data.h"
namespace PerformanceTesting
{
    struct Performance
    {
    public:
        __int64 testStart, startLoadContant, endLoadContant, startGame, endGame, startFinish, endFinish;
    };
    struct VertexPos
    {
        XMFLOAT3 pos;
    };
    LRESULT CALLBACK WndProcEmp(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    VertexPos *createDataV(int *count)
    {
        VertexPos vertices[] =
        {
            { XMFLOAT3(-1.0f, 1.0f, -1.0f)  },
            { XMFLOAT3(1.0f, 1.0f, -1.0f) },
            { XMFLOAT3(1.0f, 1.0f, 1.0f)},
            { XMFLOAT3(-1.0f, 1.0f, 1.0f) },

            { XMFLOAT3(-1.0f, -1.0f, -1.0f) },
            { XMFLOAT3(1.0f, -1.0f, -1.0f) },
            { XMFLOAT3(1.0f, -1.0f, 1.0f)},
            { XMFLOAT3(-1.0f, -1.0f, 1.0f) },

            { XMFLOAT3(-1.0f, -1.0f, 1.0f) },
            { XMFLOAT3(-1.0f, -1.0f, -1.0f) },
            { XMFLOAT3(-1.0f, 1.0f, -1.0f) },
            { XMFLOAT3(-1.0f, 1.0f, 1.0f)},

            { XMFLOAT3(1.0f, -1.0f, 1.0f) },
            { XMFLOAT3(1.0f, -1.0f, -1.0f) },
            { XMFLOAT3(1.0f, 1.0f, -1.0f) },
            { XMFLOAT3(1.0f, 1.0f, 1.0f),},

            { XMFLOAT3(-1.0f, -1.0f, -1.0f) },
            { XMFLOAT3(1.0f, -1.0f, -1.0f) },
            { XMFLOAT3(1.0f, 1.0f, -1.0f),},
            { XMFLOAT3(-1.0f, 1.0f, -1.0f) },

            { XMFLOAT3(-1.0f, -1.0f, 1.0f) },
            { XMFLOAT3(1.0f, -1.0f, 1.0f) },
            { XMFLOAT3(1.0f, 1.0f, 1.0f),},
            { XMFLOAT3(-1.0f, 1.0f, 1.0f) },
        };
        *count = 24;
        VertexPos * ret = new VertexPos[*count];
        memcpy(ret, vertices, *count * sizeof(VertexPos));
        return ret;
    }
    VertexPos *createDataV1(int *count)
    {
        VertexPos vertices[] =
        {
            { XMFLOAT3(-0.5f, 0.5f, -0.5f) },
            { XMFLOAT3(0.5f, 0.5f, -0.5f), },
            { XMFLOAT3(0.5f, 0.5f, 0.5f), },
            { XMFLOAT3(-0.5f, 0.5f, 0.5f), },

            { XMFLOAT3(-0.5f, -0.5f, -0.5f) },
            { XMFLOAT3(0.5f, -0.5f, -0.5f) },
            { XMFLOAT3(0.5f, -0.5f, 0.5f), },
            { XMFLOAT3(-0.5f, -0.5f, 0.5f) },

            { XMFLOAT3(-0.5f, -0.5f, 0.5f) },
            { XMFLOAT3(-0.5f, -0.5f, -0.5f) },
            { XMFLOAT3(-0.5f, 0.5f, -0.5f) },
            { XMFLOAT3(-0.5f, 0.5f, 0.5f), },

            { XMFLOAT3(0.5f, -0.5f, 0.5f), },
            { XMFLOAT3(0.5f, -0.5f, -0.5f) },
            { XMFLOAT3(0.5f, 0.5f, -0.5f), },
            { XMFLOAT3(0.5f, 0.5f, 0.5f), },

            { XMFLOAT3(-0.5f, -0.5f, -0.5f) },
            { XMFLOAT3(0.5f, -0.5f, -0.5f) },
            { XMFLOAT3(0.5f, 0.5f, -0.5f), },
            { XMFLOAT3(-0.5f, 0.5f, -0.5f) },

            { XMFLOAT3(-0.5f, -0.5f, 0.5f) },
            { XMFLOAT3(0.5f, -0.5f, 0.5f), },
            { XMFLOAT3(0.5f, 0.5f, 0.5f), },
            { XMFLOAT3(-0.5f, 0.5f, 0.5f), },
        };
        for (int i = 0; i < 24; i++)
        {
            VertexPos *v = &(vertices[i]);
            v->pos.x -= 1;
            v->pos.y -= 1;
            v->pos.z -= 1;
        }
        *count = 24;
        VertexPos * ret = new VertexPos[*count];
        memcpy(ret, vertices, *count * sizeof(VertexPos));
        return ret;
    }
    VertexPos *createDataV2(int *count)
    {
        VertexPos vertices[] =
        {
            { XMFLOAT3(-0.5f, 0.5f, -0.5f) },
            { XMFLOAT3(0.5f, 0.5f, -0.5f), },
            { XMFLOAT3(0.5f, 0.5f, 0.5f), },
            { XMFLOAT3(-0.5f, 0.5f, 0.5f), },

            { XMFLOAT3(-0.5f, -0.5f, -0.5f) },
            { XMFLOAT3(0.5f, -0.5f, -0.5f) },
            { XMFLOAT3(0.5f, -0.5f, 0.5f), },
            { XMFLOAT3(-0.5f, -0.5f, 0.5f) },

            { XMFLOAT3(-0.5f, -0.5f, 0.5f) },
            { XMFLOAT3(-0.5f, -0.5f, -0.5f) },
            { XMFLOAT3(-0.5f, 0.5f, -0.5f) },
            { XMFLOAT3(-0.5f, 0.5f, 0.5f), },

            { XMFLOAT3(0.5f, -0.5f, 0.5f), },
            { XMFLOAT3(0.5f, -0.5f, -0.5f) },
            { XMFLOAT3(0.5f, 0.5f, -0.5f), },
            { XMFLOAT3(0.5f, 0.5f, 0.5f), },

            { XMFLOAT3(-0.5f, -0.5f, -0.5f) },
            { XMFLOAT3(0.5f, -0.5f, -0.5f) },
            { XMFLOAT3(0.5f, 0.5f, -0.5f), },
            { XMFLOAT3(-0.5f, 0.5f, -0.5f) },

            { XMFLOAT3(-0.5f, -0.5f, 0.5f) },
            { XMFLOAT3(0.5f, -0.5f, 0.5f), },
            { XMFLOAT3(0.5f, 0.5f, 0.5f), },
            { XMFLOAT3(-0.5f, 0.5f, 0.5f), },
        };
        for (int i = 0; i < 24; i++)
        {
            VertexPos *v = &(vertices[i]);
            v->pos.x -= 2;
            v->pos.y += 4;
            v->pos.z += 1;
        }
        *count = 24;
        VertexPos * ret = new VertexPos[*count];
        memcpy(ret, vertices, *count * sizeof(VertexPos));
        return ret;
    }
    VertexPos *createDataV3(int *count)
    {
        VertexPos vertices[] =
        {
            { XMFLOAT3(-0.5f, 0.5f, -0.5f) },
            { XMFLOAT3(0.5f, 0.5f, -0.5f), },
            { XMFLOAT3(0.5f, 0.5f, 0.5f), },
            { XMFLOAT3(-0.5f, 0.5f, 0.5f), },

            { XMFLOAT3(-0.5f, -0.5f, -0.5f) },
            { XMFLOAT3(0.5f, -0.5f, -0.5f) },
            { XMFLOAT3(0.5f, -0.5f, 0.5f), },
            { XMFLOAT3(-0.5f, -0.5f, 0.5f) },

            { XMFLOAT3(-0.5f, -0.5f, 0.5f) },
            { XMFLOAT3(-0.5f, -0.5f, -0.5f) },
            { XMFLOAT3(-0.5f, 0.5f, -0.5f) },
            { XMFLOAT3(-0.5f, 0.5f, 0.5f), },

            { XMFLOAT3(0.5f, -0.5f, 0.5f), },
            { XMFLOAT3(0.5f, -0.5f, -0.5f) },
            { XMFLOAT3(0.5f, 0.5f, -0.5f), },
            { XMFLOAT3(0.5f, 0.5f, 0.5f), },

            { XMFLOAT3(-0.5f, -0.5f, -0.5f) },
            { XMFLOAT3(0.5f, -0.5f, -0.5f) },
            { XMFLOAT3(0.5f, 0.5f, -0.5f), },
            { XMFLOAT3(-0.5f, 0.5f, -0.5f) },

            { XMFLOAT3(-0.5f, -0.5f, 0.5f) },
            { XMFLOAT3(0.5f, -0.5f, 0.5f), },
            { XMFLOAT3(0.5f, 0.5f, 0.5f), },
            { XMFLOAT3(-0.5f, 0.5f, 0.5f), },
        };
        WORD indices[] =
        {
            3, 1, 0, 2, 1, 3,
            6, 4, 5, 7, 4, 6,
            11, 9, 8, 10, 9, 11,
            14, 12, 13, 15, 12, 14,
            19, 17, 16, 18, 17, 19,
            22, 20, 21, 23, 20, 22
        };
        const int l = sizeof(indices) / sizeof(WORD);
        for (int i = 0; i < 24; i++)
        {
            VertexPos *v = &(vertices[i]);
            v->pos.x += 1;
            v->pos.y += 1;
            v->pos.z += 1;
        }
        VertexPos *data = new VertexPos[l];
        for (int i = 0; i < l; i++)
        {
            data[i] = vertices[indices[i]];
        }
        *count = l;
        VertexPos * ret = new VertexPos[*count];
        memcpy(ret, data, *count * sizeof(VertexPos));
        return ret;
    }
    WORD *createDataI(int *count)
    {
        WORD indices[] =
        {
            3, 1, 0, 2, 1, 3,
            6, 4, 5, 7, 4, 6,
            11, 9, 8, 10, 9, 11,
            14, 12, 13, 15, 12, 14,
            19, 17, 16, 18, 17, 19,
            22, 20, 21, 23, 20, 22
        };
        *count = 36;
        WORD * ret = new WORD[*count];
        memcpy(ret, indices, *count * sizeof(WORD));
        return ret;
    }
    const int w = 640;
    const int h = 480;
    const int NUMBER_OF_FRAMES = 100000;
    void CompileD3DShader(WCHAR* filePath, char* entry, char* shaderModel, ID3DBlob** buffer)
    {
        DWORD shaderFlags = 0;
        ID3DBlob* errorBuffer = 0;
        HRESULT result;
        
        result = D3DCompileFromFile(filePath, 0, 0, entry, shaderModel,
            shaderFlags, 0,  buffer, &errorBuffer);
        if (FAILED(result))
        {
            if (errorBuffer != 0)
            {
                OutputDebugStringA((char*)errorBuffer->GetBufferPointer());
                errorBuffer->Release();
            }
        }
        if (errorBuffer != 0)
            errorBuffer->Release();
    }
    Performance startEmptyGame(HINSTANCE hInstance, HWND hwnd)
    {
        Performance p;
        QueryPerformanceCounter((LARGE_INTEGER*)&p.testStart);
        int frames = 0;
        ID3D11Device* d3dDevice_;
        ID3D11DeviceContext* d3dContext_;
        IDXGISwapChain* swapChain_;
        D3D_DRIVER_TYPE driverType_;
        D3D_FEATURE_LEVEL featureLevel_;
        ID3D11RenderTargetView* backBufferTarget_;
        ID3D11Texture2D* depthTexture_;
        ID3D11DepthStencilView* depthStencilView_;
        ID3D11VertexShader* textureMapVS_;
        ID3D11PixelShader* textureMapPS_;
        ID3D11InputLayout* inputLayout_;
        ID3D11Buffer* vBuffers[12];
        ID3D11Buffer* iBuffers[9];
        ID3D11Buffer* viewCB_;
        ID3D11Buffer* projCB_;
        ID3D11Buffer* worldCB_;
        QueryPerformanceCounter((LARGE_INTEGER*)&p.startLoadContant);
        {
            RECT dimensions;
            GetClientRect(hwnd, &dimensions);
            D3D_DRIVER_TYPE driverTypes[] =
            {
                D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_SOFTWARE
            };
            unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);
            D3D_FEATURE_LEVEL featureLevels[] =
            {
                D3D_FEATURE_LEVEL_11_0,
                D3D_FEATURE_LEVEL_10_1,
                D3D_FEATURE_LEVEL_10_0
            };
            unsigned int totalFeatureLevels = ARRAYSIZE(featureLevels);
            DXGI_SWAP_CHAIN_DESC swapChainDesc;
            ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
            swapChainDesc.BufferCount = 1;
            swapChainDesc.BufferDesc.Width = w;
            swapChainDesc.BufferDesc.Height = h;
            swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
            swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.OutputWindow = hwnd;
            swapChainDesc.Windowed = true;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            unsigned int creationFlags = 0;
#ifdef _DEBUG
            creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
            HRESULT result;
            unsigned int driver = 0;
            for (driver = 0; driver < totalDriverTypes; ++driver)
            {
                result = D3D11CreateDeviceAndSwapChain(0, driverTypes[driver], 0, creationFlags,
                    featureLevels, totalFeatureLevels,
                    D3D11_SDK_VERSION, &swapChainDesc, &swapChain_,
                    &d3dDevice_, &featureLevel_, &d3dContext_);

                if (SUCCEEDED(result))
                {
                    driverType_ = driverTypes[driver];
                    break;
                }
            }
            ID3D11Texture2D* backBufferTexture;
            result = swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);
            result = d3dDevice_->CreateRenderTargetView(backBufferTexture, 0, &backBufferTarget_);
            if (backBufferTexture)
                backBufferTexture->Release();
            D3D11_TEXTURE2D_DESC depthTexDesc;
            ZeroMemory(&depthTexDesc, sizeof(depthTexDesc));
            depthTexDesc.Width = w;
            depthTexDesc.Height = h;
            depthTexDesc.MipLevels = 1;
            depthTexDesc.ArraySize = 1;
            depthTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
            depthTexDesc.SampleDesc.Count = 1;
            depthTexDesc.SampleDesc.Quality = 0;
            depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
            depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
            depthTexDesc.CPUAccessFlags = 0;
            depthTexDesc.MiscFlags = 0;
            result = d3dDevice_->CreateTexture2D(&depthTexDesc, NULL, &depthTexture_);
            D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
            ZeroMemory(&descDSV, sizeof(descDSV));
            descDSV.Format = depthTexDesc.Format;
            descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
            descDSV.Texture2D.MipSlice = 0;
            result = d3dDevice_->CreateDepthStencilView(depthTexture_, &descDSV, &depthStencilView_);
            d3dContext_->OMSetRenderTargets(1, &backBufferTarget_, depthStencilView_);
            D3D11_VIEWPORT viewport;
            viewport.Width = static_cast<float>(w);
            viewport.Height = static_cast<float>(h);
            viewport.MinDepth = 0.0f;
            viewport.MaxDepth = 1.0f;
            viewport.TopLeftX = 0.0f;
            viewport.TopLeftY = 0.0f;
            d3dContext_->RSSetViewports(1, &viewport);
            ID3DBlob* vsBuffer = 0;
            CompileD3DShader(L"WhiteColor.fx", "VS_Main", "vs_4_0", &vsBuffer);
            HRESULT d3dResult;
            d3dResult = d3dDevice_->CreateVertexShader(vsBuffer->GetBufferPointer(),
                vsBuffer->GetBufferSize(), 0, &textureMapVS_);
            D3D11_INPUT_ELEMENT_DESC solidColorLayout[] =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
            };
            unsigned int totalLayoutElements = ARRAYSIZE(solidColorLayout);
            d3dResult = d3dDevice_->CreateInputLayout(solidColorLayout, totalLayoutElements,
                vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &inputLayout_);
            vsBuffer->Release();
            ID3DBlob* psBuffer = 0;
            CompileD3DShader(L"WhiteColor.fx", "PS_Main", "ps_4_0", &psBuffer);
            d3dResult = d3dDevice_->CreatePixelShader(psBuffer->GetBufferPointer(),
                psBuffer->GetBufferSize(), 0, &textureMapPS_);
            psBuffer->Release();
            {
                int i = 0;
                {
                    int totalVerts_;
                    int totI;
                    VertexPos* bubuf = createDataV(&totalVerts_);
                    D3D11_BUFFER_DESC vertexDesc;
                    ZeroMemory(&vertexDesc, sizeof(vertexDesc));
                    vertexDesc.Usage = D3D11_USAGE_DEFAULT;
                    vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
                    vertexDesc.ByteWidth = sizeof(VertexPos) * totalVerts_;
                    D3D11_SUBRESOURCE_DATA resourceData;
                    ZeroMemory(&resourceData, sizeof(resourceData));
                    resourceData.pSysMem = bubuf;
                    WORD* bububuf = createDataI(&totI);
                    D3D11_BUFFER_DESC vertexDesc1;
                    ZeroMemory(&vertexDesc1, sizeof(vertexDesc1));
                    vertexDesc1.Usage = D3D11_USAGE_DEFAULT;
                    vertexDesc1.BindFlags = D3D11_BIND_INDEX_BUFFER;
                    vertexDesc1.ByteWidth = sizeof(WORD) * totI;
                    D3D11_SUBRESOURCE_DATA resourceData1;
                    ZeroMemory(&resourceData1, sizeof(resourceData1));
                    resourceData1.pSysMem = bububuf;
                    for (; i < 3; i++)
                    {
                        d3dDevice_->CreateBuffer(&vertexDesc, &resourceData, &vBuffers[i]);
                        d3dDevice_->CreateBuffer(&vertexDesc1, &resourceData1, &iBuffers[i]);
                    }
                    delete[] bubuf;
                    delete[] bububuf;
                }
                {
                    int totalVerts_;
                    int totI;
                    VertexPos* bubuf = createDataV1(&totalVerts_);
                    D3D11_BUFFER_DESC vertexDesc;
                    ZeroMemory(&vertexDesc, sizeof(vertexDesc));
                    vertexDesc.Usage = D3D11_USAGE_DEFAULT;
                    vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
                    vertexDesc.ByteWidth = sizeof(VertexPos) * totalVerts_;
                    D3D11_SUBRESOURCE_DATA resourceData;
                    ZeroMemory(&resourceData, sizeof(resourceData));
                    resourceData.pSysMem = bubuf;
                    WORD* bububuf = createDataI(&totI);
                    D3D11_BUFFER_DESC vertexDesc1;
                    ZeroMemory(&vertexDesc1, sizeof(vertexDesc1));
                    vertexDesc1.Usage = D3D11_USAGE_DEFAULT;
                    vertexDesc1.BindFlags = D3D11_BIND_INDEX_BUFFER;
                    vertexDesc1.ByteWidth = sizeof(WORD) * totI;
                    D3D11_SUBRESOURCE_DATA resourceData1;
                    ZeroMemory(&resourceData1, sizeof(resourceData1));
                    resourceData1.pSysMem = bububuf;
                    for (; i < 6; i++)
                    {
                        d3dDevice_->CreateBuffer(&vertexDesc, &resourceData, &vBuffers[i]);
                        d3dDevice_->CreateBuffer(&vertexDesc1, &resourceData1, &iBuffers[i]);
                    }
                    delete[] bubuf;
                    delete[] bububuf;
                }
                {
                    int totalVerts_;
                    int totI;
                    VertexPos* bubuf = createDataV2(&totalVerts_);
                    D3D11_BUFFER_DESC vertexDesc;
                    ZeroMemory(&vertexDesc, sizeof(vertexDesc));
                    vertexDesc.Usage = D3D11_USAGE_DEFAULT;
                    vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
                    vertexDesc.ByteWidth = sizeof(VertexPos) * totalVerts_;
                    D3D11_SUBRESOURCE_DATA resourceData;
                    ZeroMemory(&resourceData, sizeof(resourceData));
                    resourceData.pSysMem = bubuf;
                    WORD* bububuf = createDataI(&totI);
                    D3D11_BUFFER_DESC vertexDesc1;
                    ZeroMemory(&vertexDesc1, sizeof(vertexDesc1));
                    vertexDesc1.Usage = D3D11_USAGE_DEFAULT;
                    vertexDesc1.BindFlags = D3D11_BIND_INDEX_BUFFER;
                    vertexDesc1.ByteWidth = sizeof(WORD) * totI;
                    D3D11_SUBRESOURCE_DATA resourceData1;
                    ZeroMemory(&resourceData1, sizeof(resourceData1));
                    resourceData1.pSysMem = bububuf;
                    for (; i < 9; i++)
                    {
                        d3dDevice_->CreateBuffer(&vertexDesc, &resourceData, &vBuffers[i]);
                        d3dDevice_->CreateBuffer(&vertexDesc1, &resourceData1, &iBuffers[i]);
                    }
                    delete[] bubuf;
                    delete[] bububuf;
                }
                {
                    int totalVerts_;
                    VertexPos* bubuf = createDataV3(&totalVerts_);
                    D3D11_BUFFER_DESC vertexDesc;
                    ZeroMemory(&vertexDesc, sizeof(vertexDesc));
                    vertexDesc.Usage = D3D11_USAGE_DEFAULT;
                    vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
                    vertexDesc.ByteWidth = sizeof(VertexPos) * totalVerts_;
                    D3D11_SUBRESOURCE_DATA resourceData;
                    ZeroMemory(&resourceData, sizeof(resourceData));
                    resourceData.pSysMem = bubuf;
                    for (; i < 12; i++)
                    {

                        d3dDevice_->CreateBuffer(&vertexDesc, &resourceData, &vBuffers[i]);
                    }
                    delete[] bubuf;
                }
            }
            D3D11_BUFFER_DESC constDesc;
            ZeroMemory(&constDesc, sizeof(constDesc));
            constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            constDesc.ByteWidth = sizeof(XMMATRIX);
            constDesc.Usage = D3D11_USAGE_DEFAULT;
            d3dResult = d3dDevice_->CreateBuffer(&constDesc, 0, &viewCB_);
            d3dResult = d3dDevice_->CreateBuffer(&constDesc, 0, &projCB_);
            d3dResult = d3dDevice_->CreateBuffer(&constDesc, 0, &worldCB_);
            {
                XMMATRIX projMatrix_ = XMMatrixTranspose(XMMatrixPerspectiveFovLH(0.785398163f, 640.0f / 480, 0.01f, 100));
                XMMATRIX worldMat = XMMatrixTranspose(XMMatrixMultiply(XMMatrixRotationRollPitchYaw(0, 0.7f, 0.7f), XMMatrixTranslation(0, 0, 6)));
                XMMATRIX viewMat = XMMatrixTranspose(XMMatrixIdentity());
                d3dContext_->UpdateSubresource(viewCB_, 0, 0, &viewMat, 0, 0);
                d3dContext_->UpdateSubresource(projCB_, 0, 0, &projMatrix_, 0, 0);
                d3dContext_->UpdateSubresource(worldCB_, 0, 0, &worldMat, 0, 0);
                d3dContext_->VSSetConstantBuffers(1, 1, &viewCB_);
                d3dContext_->VSSetConstantBuffers(2, 1, &projCB_);
                d3dContext_->VSSetConstantBuffers(0, 1, &worldCB_);
            }
            d3dContext_->IASetInputLayout(inputLayout_);
            d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            d3dContext_->VSSetShader(textureMapVS_, 0, 0);
            d3dContext_->PSSetShader(textureMapPS_, 0, 0);
        }
        QueryPerformanceCounter((LARGE_INTEGER*)&p.endLoadContant);
        MSG msg = { 0 };
        while (true)
        {
            if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            frames++;
            const float clearColor[] = { 0.2f,0.532f,0.7345f,1 };
            d3dContext_->ClearRenderTargetView(backBufferTarget_, clearColor);
            d3dContext_->ClearDepthStencilView(depthStencilView_, D3D11_CLEAR_DEPTH, 1.0f, 0);
            const unsigned int stride = sizeof(VertexPos);
            const unsigned int offset = 0;
            for (size_t j = 0; j < 9; j++)
            {
                d3dContext_->IASetVertexBuffers(0, 1, &vBuffers[j], &stride, &offset);
                d3dContext_->IASetIndexBuffer(iBuffers[j], DXGI_FORMAT::DXGI_FORMAT_R16_UINT, 0);
                d3dContext_->DrawIndexed(36, 0, 0);
            }
            for (size_t j = 9; j < 12; j++)
            {
                d3dContext_->IASetVertexBuffers(0, 1, &vBuffers[j], &stride, &offset);
                d3dContext_->Draw(36, 0);
            }
            swapChain_->Present(0, 0);
            if (frames == NUMBER_OF_FRAMES)
            {
                break;
            }
        }
        QueryPerformanceCounter((LARGE_INTEGER*)&p.endGame);
        // Demo Shutdown
        {
            worldCB_->Release();
            projCB_->Release();
            viewCB_->Release();
            for (size_t i = 0; i < 9; i++)
            {
                iBuffers[i]->Release();
            }
            for (size_t i = 0; i < 12; i++)
            {
                vBuffers[i]->Release();
            }
            inputLayout_->Release();
            textureMapPS_->Release();
            textureMapVS_->Release();
            depthStencilView_->Release();
            depthTexture_->Release();
            backBufferTarget_->Release();
            swapChain_->Release();
            d3dContext_->Release();
            d3dDevice_->Release();
        }
        QueryPerformanceCounter((LARGE_INTEGER*)&p.endFinish);
        p.startFinish = p.endGame;
        p.startGame = p.endLoadContant;
        return p;
    }
    Performance myEmptyGame(HINSTANCE hInstance, HWND hwnd)
    {
        Performance p;
        {
            QueryPerformanceCounter((LARGE_INTEGER*)&p.testStart);
            DXMain main;
            QueryPerformanceCounter((LARGE_INTEGER*)&p.startLoadContant);
            VideoAdapter adapter = VideoAdapter::getAdapters()[0];
            FeatureLevel features[] =
            {
                FeatureLevel::DX_11_0,
                FeatureLevel::DX_10_1,
                FeatureLevel::DX_10_0
            };
            main.initDXObject(&adapter, 1, features, 3, true);
            main.initDevice();
            std::vector<VideoMode> vid = VideoOutput(adapter.getAllOutput()[0]).getVideosMode(DataFormat::R8G8B8A8_UNORM);
            size_t videoIndex = 0;
            for (; videoIndex < vid.size() && !(vid[videoIndex].getWidth() == w && vid[videoIndex].getHeight() == h); videoIndex++);
            RenderDevice renderDevice = main.createRenderDevice(hwnd, vid[videoIndex]);
            renderDevice.setBackgroundColor(51, 135, 187);
            Data vsDat = Shader::compileShader(L"WhiteColor.fx", "VS_Main", VertexShader::convertVsModelToString(VsModel::VS4_0));
            VertexShader vs = VertexShader::createVertexShader(main, vsDat);
            PixelShader ps = PixelShader::createPixelShader(main, L"WhiteColor.fx", "PS_Main", PsModel::PS4_0);
            InputLayout::InputLayoutBuilder inb;
            inb.addLayoutElement(LayoutElement("POSITION", DataFormat::R32G32B32_FLOAT, 0));
            Matrix projMatrix_ = Matrix::PerspectiveFovLH(0.785398163f, 640.0f / 480, 0.01f, 100);
            Matrix viewMatrix_;
            Matrix a, b;
            a = a.rotationXYZ(0, 0.7f, 0.7f);
            b = b.translation(0, 0, 6);
            Matrix worldMat = a * b;
            worldMat = worldMat.transpose();
            projMatrix_ = projMatrix_.transpose();
            viewMatrix_ = viewMatrix_.transpose();
            Buffer bufproj = main.getResourceManager().createBuffer(1, sizeof(Matrix), ResourceFormat::CONSTANT_BUFFER, (Access)(Access::GpuRead | Access::GpuWrite), 0);
            Buffer bufview = main.getResourceManager().createBuffer(1, sizeof(Matrix), ResourceFormat::CONSTANT_BUFFER, (Access)(Access::GpuRead | Access::GpuWrite), 0);
            Buffer bufworld = main.getResourceManager().createBuffer(1, sizeof(Matrix), ResourceFormat::CONSTANT_BUFFER, (Access)(Access::GpuRead | Access::GpuWrite), 0);
            bufproj.updateResourceData(main, &projMatrix_);
            bufview.updateResourceData(main, &viewMatrix_);
            bufworld.updateResourceData(main, &worldMat);
            PipeShadersParams pipeShaderParams;
            pipeShaderParams.getvertexParams().addConstantBuffer(bufworld);
            pipeShaderParams.getvertexParams().addConstantBuffer(bufview);
            pipeShaderParams.getvertexParams().addConstantBuffer(bufproj);
            DrawCommand drawCommand(PrimitiveTopology::TRIANGLELIST, PipeState::PipeStateBuilder().addShader(vs).addShader(ps).build(), inb.build(main, vsDat));
            Game g(renderDevice);
            {
                GOEntity* ent;
                int i = 0;
                {
                    int totalV;
                    int totalI;
                    VertexPos* vert = createDataV(&totalV);
                    WORD* inde = createDataI(&totalI);
                    for (; i < 3; i++)
                    {
                        ent = new GOEntity(main.getResourceManager().createBuffer(totalV, sizeof(VertexPos), ResourceFormat::VERTEX_BUFFER, Access::GpuRead, vert), main.getResourceManager().createBuffer(totalI, sizeof(WORD), ResourceFormat::INDEX_BUFFER, Access::GpuRead, inde));
                        g.addObject(ent, drawCommand);
                    }
                    delete[] vert;
                    delete[] inde;
                }
                {
                    int totalV;
                    int totalI;
                    VertexPos* vert = createDataV1(&totalV);
                    WORD* inde = createDataI(&totalI);
                    for (; i < 6; i++)
                    {
                        ent = new GOEntity(main.getResourceManager().createBuffer(totalV, sizeof(VertexPos), ResourceFormat::VERTEX_BUFFER, Access::GpuRead, vert), main.getResourceManager().createBuffer(totalI, sizeof(WORD), ResourceFormat::INDEX_BUFFER, Access::GpuRead, inde));
                        g.addObject(ent, drawCommand);
                    }
                    delete[] vert;
                    delete[] inde;
                }
                {
                    int totalV;
                    int totalI;
                    VertexPos* vert = createDataV2(&totalV);
                    WORD* inde = createDataI(&totalI);
                    for (; i < 9; i++)
                    {
                        ent = new GOEntity(main.getResourceManager().createBuffer(totalV, sizeof(VertexPos), ResourceFormat::VERTEX_BUFFER, Access::GpuRead, vert), main.getResourceManager().createBuffer(totalI, sizeof(WORD), ResourceFormat::INDEX_BUFFER, Access::GpuRead, inde));
                        g.addObject(ent, drawCommand);
                    }
                    delete[] vert;
                    delete[] inde;
                }
                {
                    int totalV;
                    int totalI;
                    VertexPos* vert = createDataV3(&totalV);
                    WORD* inde = createDataI(&totalI);
                    for (; i < 12; i++)
                    {
                        ent = new GOEntity(main.getResourceManager().createBuffer(totalV, sizeof(VertexPos), ResourceFormat::VERTEX_BUFFER, Access::GpuRead, vert));
                        g.addObject(ent, drawCommand);
                    }
                    delete[] vert;
                    delete[] inde;
                }
            }
            int frames = 0;
            QueryPerformanceCounter((LARGE_INTEGER*)&p.endLoadContant);
            MSG msg = { 0 };
            while (true)
            {
                if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
                frames++;
                renderDevice.beginRender();
                pipeShaderParams.apply(renderDevice.getdx().getDevice());
                g.update();
                g.render();
                renderDevice.endRender();
                if (frames == NUMBER_OF_FRAMES)
                {
                    break;
                }
                // Update and Draw
            }
            QueryPerformanceCounter((LARGE_INTEGER*)&p.endGame);
        }
        QueryPerformanceCounter((LARGE_INTEGER*)&p.endFinish);
        p.startFinish = p.endGame;
        p.startGame = p.endLoadContant;
        return p;
    }
    TEST(SmallGame, emptyGame)
    {
        const wchar_t *CLASS_NAME = L"DXTestingEm";
        HINSTANCE hInstance = GetModuleHandle(NULL);
        WNDCLASSEX wndClass = { 0 };
        wndClass.cbSize = sizeof(WNDCLASSEX);
        wndClass.style = CS_HREDRAW | CS_VREDRAW;
        wndClass.lpfnWndProc = WndProcEmp;
        wndClass.hInstance = hInstance;
        wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wndClass.lpszMenuName = NULL;
        wndClass.lpszClassName = CLASS_NAME;
        ASSERT_TRUE(RegisterClassEx(&wndClass)) << "Error register class";
        HWND hwnd = CreateWindow(CLASS_NAME, L"test outer game", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, w, h, NULL, NULL, hInstance, NULL);
        ASSERT_TRUE(hwnd) << "Error creating window";
        ShowWindow(hwnd, SW_SHOW);
        Performance outerGame = startEmptyGame(hInstance, hwnd);
        DestroyWindow(hwnd);
        HWND hwnd1 = CreateWindow(CLASS_NAME, L"test my engine", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, w, h, NULL, NULL, hInstance, NULL);
        ASSERT_TRUE(hwnd1) << "Error creating window";
        ShowWindow(hwnd1, SW_SHOW);
        Performance intGame = myEmptyGame(hInstance, hwnd1);
        DestroyWindow(hwnd1);
        std::wstring s(L"The loading took too much time:");
        s.append(std::to_wstring(intGame.endLoadContant - intGame.startLoadContant));
        s.append(L"<");
        s.append(std::to_wstring(outerGame.endLoadContant - outerGame.startLoadContant));
        EXPECT_LT(intGame.endLoadContant - intGame.startLoadContant, outerGame.endLoadContant - outerGame.startLoadContant) << s.data();
        s = std::wstring(L"The game loop took too much time:");
        s.append(std::to_wstring(intGame.endGame - intGame.startGame));
        s.append(L"<");
        s.append(std::to_wstring(outerGame.endGame - outerGame.startGame));
        EXPECT_LT(intGame.endGame - intGame.startGame, outerGame.endGame - outerGame.startGame) << s.data();
        s = std::wstring(L"The finishing took too much time:");
        s.append(std::to_wstring(intGame.endFinish - intGame.startFinish));
        s.append(L"<");
        s.append(std::to_wstring(outerGame.endFinish - outerGame.startFinish));
        EXPECT_LT(intGame.endFinish - intGame.startFinish, outerGame.endFinish - outerGame.startFinish) << s.data();
    }
}