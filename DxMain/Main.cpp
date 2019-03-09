#include "Defines.h"
#include<windows.h>
#include<tchar.h>
#include "Global.h"
#include "DXMain.h"
#include "Game.h"
#include "DrawCommand.h"
#include "PipeState.h"
#include "GOEntity.h"
#include "LayoutElement.h"
#include "Sampler.h"
#include "Buffer.h"
#include "VideoAdapter.h"
#include "VideoOutput.h"
#include "inputLayout.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "GameTime.h"
#include "VideoMode.h"
#include "Texture2D.h"
#include "ShaderResource.h"
#include "GraphicState.h"
#include "RenderState.h"
#include "BlendState.h"
#include "MapData.h"
#include "Color.h"
#include "Vector.h"
#include "Matrix.h"
#include "UObject.h"
#include "ShaderResourceInfo.h"
#include "Model.h"
#include "Effect.h"
#include "ResourceManager.h"
#include "GpuResourceManager.h"
#include "ShaderParams.h"
#include "Device.h"
#include "PipeShadersParams.h"
using namespace DirectX;
namespace Colors
{
    XMGLOBALCONST XMVECTORF32 White = { 1.0f, 1.0f, 1.0f, 1.0f };
    XMGLOBALCONST XMVECTORF32 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
    XMGLOBALCONST XMVECTORF32 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
    XMGLOBALCONST XMVECTORF32 Green = { 0.0f, 1.0f, 0.0f, 1.0f };
    XMGLOBALCONST XMVECTORF32 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
    XMGLOBALCONST XMVECTORF32 Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
    XMGLOBALCONST XMVECTORF32 Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
    XMGLOBALCONST XMVECTORF32 Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };

    XMGLOBALCONST XMVECTORF32 Silver = { 0.75f, 0.75f, 0.75f, 1.0f };
    XMGLOBALCONST XMVECTORF32 LightSteelBlue = { 0.69f, 0.77f, 0.87f, 1.0f };
}
struct VertexPos
{
    XMFLOAT3 pos;
    XMFLOAT2 tex0;
};
struct Vertex
{
    XMFLOAT3 Pos;
    XMFLOAT4 Color;
};
#define S _T
HINSTANCE hInstance;
int nCmdShow;
DXMain *main;
RenderDevice* renderDevice;
Buffer createBuffer(UINT numOfEle, UINT sizeOfEle, ResourceFormat format, Access access, const void* data)
{
    return main->getResourceManager().createBuffer(numOfEle, sizeOfEle, format, access, data);
}
LRESULT CALLBACK WindowProcedure(HWND winhan, UINT uint_Message, WPARAM parameter1, LPARAM parameter2)
{
    int w, h;
    switch (uint_Message) {
    case WM_KEYDOWN:
        if (parameter1 == 27)
        {
            PostMessage(winhan, WM_CLOSE, 0, 0);
        }
        else if (parameter1 == 70)
        {
            if (renderDevice != 0)
            {
                renderDevice->setFullScreenMode(!renderDevice->getFullScreenMode());
            }
        }
        break;
    case WM_SIZE:
        w = LOWORD(parameter2);
        h = HIWORD(parameter2);
        if (main != NULL && renderDevice != 0)
        {
            renderDevice->resizeWindow(w, h, NULL);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(winhan, uint_Message, parameter1, parameter2);
}
HWND NewWindow(LPCTSTR str_Title, int int_XPos, int int_YPos, int int_Width, int int_Height)
{
    wchar_t * szWindowClass = S("dxgamewin");
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WindowProcedure;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, (IDI_APPLICATION));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, (IDI_APPLICATION));

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Win32 Guided Tour"),
            NULL);

        return 0;
    }


    // The parameters to CreateWindow explained:
    // szWindowClass: the name of the application
    // szTitle: the text that appears in the title bar
    // WS_OVERLAPPEDWINDOW: the type of window to create
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
    // 500, 100: initial size (width, length)
    // NULL: the parent of this window
    // NULL: this application dows not have a menu bar
    // hInstance: the first parameter from WinMain
    // NULL: not used in this application
    HWND hWnd = CreateWindow(
        szWindowClass,
        str_Title,
        WS_OVERLAPPEDWINDOW,
        int_XPos, int_YPos,
        int_Width, int_Height,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Win32 Guided Tour"),
            NULL);

        return 0;
    }

    // The parameters to ShowWindow explained:
    // hWnd: the value returned from CreateWindow
    // nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd,
        nCmdShow);
    UpdateWindow(hWnd);
    return hWnd;
}
Buffer createDataV(Device &device)
{
    VertexPos vertices[] =
    {
        { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
    { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
    { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

    { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
    { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
    { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

    { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
    { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
    { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

    { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
    { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
    { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

    { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
    { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
    { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },

    { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
    { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
    { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
    };
    Buffer buf = createBuffer(24, sizeof(VertexPos), ResourceFormat::VERTEX_BUFFER, (Access)(Access::GpuRead | Access::GpuWrite), vertices);
    return buf;
}
void createDataV1Data(VertexPos* vp)
{
    VertexPos vertices[] =
    {
        { XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT2(1.0f, 0.0f) },
    { XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT2(1.0f, 1.0f) },
    { XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT2(0.0f, 1.0f) },

    { XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT2(1.0f, 0.0f) },
    { XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT2(1.0f, 1.0f) },
    { XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT2(0.0f, 1.0f) },

    { XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT2(1.0f, 0.0f) },
    { XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT2(1.0f, 1.0f) },
    { XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT2(0.0f, 1.0f) },

    { XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT2(1.0f, 0.0f) },
    { XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT2(1.0f, 1.0f) },
    { XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT2(0.0f, 1.0f) },

    { XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT2(1.0f, 0.0f) },
    { XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT2(1.0f, 1.0f) },
    { XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT2(0.0f, 1.0f) },

    { XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT2(1.0f, 0.0f) },
    { XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT2(1.0f, 1.0f) },
    { XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT2(0.0f, 1.0f) },
    };
    for (int i = 0; i < 24; i++)
    {
        VertexPos *v = &(vertices[i]);
        v->pos.x -= 1;
        v->pos.y -= 1;
        v->pos.z -= 1;
        vp[i] = *v;
    }
}
Buffer createDataV1(DXMain &main)
{
    VertexPos vertices[24];
    createDataV1Data(vertices);
    Buffer buf = createBuffer(24, sizeof(VertexPos), ResourceFormat::VERTEX_BUFFER, (Access)(Access::GpuRead | Access::CpuWrite), vertices);
    return buf;
}
Buffer createDataV2(DXMain&main)
{
    VertexPos vertices[] =
    {
        { XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT2(1.0f, 0.0f) },
    { XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT2(1.0f, 1.0f) },
    { XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT2(0.0f, 1.0f) },

    { XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT2(1.0f, 0.0f) },
    { XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT2(1.0f, 1.0f) },
    { XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT2(0.0f, 1.0f) },

    { XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT2(1.0f, 0.0f) },
    { XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT2(1.0f, 1.0f) },
    { XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT2(0.0f, 1.0f) },

    { XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT2(1.0f, 0.0f) },
    { XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT2(1.0f, 1.0f) },
    { XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT2(0.0f, 1.0f) },

    { XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT2(1.0f, 0.0f) },
    { XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT2(1.0f, 1.0f) },
    { XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT2(0.0f, 1.0f) },

    { XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT2(1.0f, 0.0f) },
    { XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT2(1.0f, 1.0f) },
    { XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT2(0.0f, 1.0f) },
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
    Buffer buf = createBuffer(l, sizeof(VertexPos), ResourceFormat::VERTEX_BUFFER, (Access)(Access::GpuRead | Access::GpuWrite), data);
    return buf;
}
Buffer createDataI(DXMain &main)
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
    Buffer buf = createBuffer(36, sizeof(WORD), ResourceFormat::INDEX_BUFFER, (Access)(Access::GpuRead | Access::GpuWrite), indices);
    return buf;
}
class MModel :public Model
{
public:
	MModel(PipeShadersParams &shadParams):shadParams(shadParams)
	{
	}
    void prepareRendering(const RenderDevice &renderDevice) override
    {
        shadParams.apply(renderDevice.getdx().getDevice());
        Model::prepareRendering(renderDevice);
    }
	PipeShadersParams &shadParams;
};
class UModel : public UObject
{
public:
    UModel(MapData &data) :mapData(data)
    {
        createDataV1Data(vp);
    }
    virtual bool update(const GameTime &time)
    {
        VertexPos* ch = reinterpret_cast<VertexPos*>(mapData.getData(*main, MapType::WRITE_DISCARD));
        for (int i = 0; i < 24; i++)
        {
            ch[i] = vp[i];
            ch[i].pos.x += 0.00001f*(time.getTotalGameTime() * 5);
        }
        mapData.applyData(*main);
        return true;
    }
    MapData& mapData;
    VertexPos vp[24];
};
int WINAPI WinMain(HINSTANCE Instance, HINSTANCE hPreviousInstance, LPSTR lpcmdline, int CmdShow)
{
    {
#pragma region data
        VertexPos vertices[] =
        {
            { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
        { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
        { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
        { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
        { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

        { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
        { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
        { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

        { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
        { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
        { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
        { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
        { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },

        { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
        { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
        { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
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
#pragma endregion
        hInstance = Instance;
        nCmdShow = CmdShow;
        MSG msg_Message;
        //create a new window
        HWND MainWindow = NewWindow(S("lol title"), 0, 0, width, height);
        main = new DXMain();

        VideoAdapter *ada = new VideoAdapter(VideoAdapter::getAdapters()[0]);
        VideoOutput *out = new VideoOutput(ada->getAllOutput()[0]);
        VideoMode* mode = new VideoMode(out->getVideosMode(DataFormat::R8G8B8A8_UNORM)[out->getVideosMode(DataFormat::R8G8B8A8_UNORM).size() - 1]);
        delete out;
        std::vector<VideoMode> modes = VideoAdapter::getAdapters()[0].getAllOutput()[0].getVideosMode(DataFormat::R8G8B8A8_UNORM);
        if (modes.size() == 0)
        {
            return 0;
        }
        delete mode; int modeNumber = modes.size() - 1;
        for (size_t i = 0; i < modes.size(); i++)
        {
            if (modes[i].getWidth() == width && modes[i].getHeight() == height)
            {
                modeNumber = i;
                break;
            }
        }
        //main->InitAll(DXDriverType::HARDWARE, FeatureLevel::DX_10_0, MainWindow, modes[modeNumber], true);
        main->initDXObject(*ada, FeatureLevel::DX_10_0, true);
        delete ada;
        main->initDevice();
        RenderDevice renderevice = main->createRenderDevice(MainWindow, modes[modeNumber]);
        renderDevice = &renderevice;
        Game game = Game(renderevice);
        renderDevice->setBackgroundColor(255, 0, 0);
        renderDevice->setAllowAltEnterFullScreen(false);
        {
            PipeShadersParams shadParams;
            RenderState rs = RenderState::RenderStateBuilder(RenderState::RenderStateBuilder::FillMode::WIREFRAME).build(*main);
            PixelShader ps = PixelShader::createPixelShader(*main, L"pinkColor.fx", "PS_Main", PsModel::PS4_0);
            VertexShader vs = VertexShader::createVertexShader(*main, L"pinkColor.fx", "VS_Main", VsModel::VS4_0);
            MModel *m = new MModel(shadParams);
            Model::ModelBuilderFromResource("Box.fbx").build(*main, m);
            Buffer bbuf = createDataV1(*main);
            MapData mapData = MapData(bbuf);
            GOEntity *obj1 = new  GOEntity(bbuf, createDataI(*main));
            GOEntity *obj2 = new  GOEntity(createDataV2(*main));
            Sampler sam = Sampler::SamplerBuilder(TextureAddressMode::WRAP, TextureAddressMode::WRAP, TextureAddressMode::WRAP, Filter::MIN_MAG_MIP_LINEAR, Comparison::NEVER).build(*main);
            shadParams.getpixelParams().addSampler(sam);
            Matrix projMatrix_ = Matrix::PerspectiveFovLH(0.785398163f, 640.0f / height, 0.01f, 100.0f).transpose();
            Matrix viewMatrix_;
            Matrix a, b;
            a = a.rotationXYZ(0.0f, 0.7f, 0.7f);
            b = b.translation(0, 0, 6);
            Matrix worldMat = (a*b).transpose();
            viewMatrix_ = viewMatrix_.transpose();
            Buffer bufproj = createBuffer(1, sizeof(Matrix), ResourceFormat::CONSTANT_BUFFER, (Access)(Access::GpuRead | Access::GpuWrite), 0);
            Buffer bufview = createBuffer(1, sizeof(Matrix), ResourceFormat::CONSTANT_BUFFER, (Access)(Access::GpuRead | Access::GpuWrite), 0);
            Buffer bufworld = createBuffer(1, sizeof(Matrix), ResourceFormat::CONSTANT_BUFFER, (Access)(Access::GpuRead | Access::GpuWrite), 0);
            bufproj.updateResourceData(*main, &projMatrix_);
            bufview.updateResourceData(*main, &viewMatrix_);
            bufworld.updateResourceData(*main, &worldMat);
            shadParams.getvertexParams().addConstantBuffer(bufworld);
            shadParams.getvertexParams().addConstantBuffer(bufview);
            shadParams.getvertexParams().addConstantBuffer(bufproj);
            shadParams.getpixelParams().addConstantBuffer(bufworld);
            shadParams.getpixelParams().addConstantBuffer(bufview);
            shadParams.getpixelParams().addConstantBuffer(bufproj);
            InputLayout::InputLayoutBuilder ibuilder;
            ibuilder.addLayoutElement(LayoutElement("POSITION", DataFormat::R32G32B32_FLOAT, 0));
            ibuilder.addLayoutElement(LayoutElement("TEXCOORD", DataFormat::R32G32_FLOAT, 0));
            InputLayout il = ibuilder.build(*main, L"pinkColor.fx", "VS_Main", VsModel::VS4_0);
            BlendState::BlendStateBuilder::BlendStateTarget tt[1];
            BlendState::BlendStateBuilder::BlendStateTarget blendTarget;
            blendTarget.enable = true;
            blendTarget.srcBlend = Blend::SRC_COLOR;
            blendTarget.destBlend = Blend::INV_SRC_COLOR;
            blendTarget.blendOp = BlendOp::OP_ADD;
            tt[0] = blendTarget;
            RenderState tempState = RenderState::RenderStateBuilder(RenderState::RenderStateBuilder::FillMode::SOLID).build(*main);
            auto psb = PipeState::PipeStateBuilder().addShader(vs).addShader(ps).addState(BlendState::BlendStateBuilder(false, false, 1, tt, Color((unsigned char)0, 0, 0)).build(*main));
            game.addObject(m, DrawCommand(PrimitiveTopology::TRIANGLELIST, PipeState::PipeStateBuilder(psb).addState(tempState).build(), il));
            game.addObject(obj1, DrawCommand(PrimitiveTopology::TRIANGLELIST, PipeState::PipeStateBuilder(psb).addState(tempState).build(), il));
            game.addObject(new UModel(mapData));
            tempState = RenderState::RenderStateBuilder(RenderState::RenderStateBuilder::FillMode::WIREFRAME).build(*main);
            game.addObject(obj2, DrawCommand(PrimitiveTopology::TRIANGLELIST, PipeState::PipeStateBuilder(psb).addState(tempState).build(), il));
#pragma region data
            Vertex vertices[] =
            {
                { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4((const float*)&Colors::White) },
            { XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4((const float*)&Colors::Black) },
            { XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4((const float*)&Colors::Red) },
            { XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4((const float*)&Colors::Green) },
            { XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4((const float*)&Colors::Blue) },
            { XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4((const float*)&Colors::Yellow) },
            { XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4((const float*)&Colors::Cyan) },
            { XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4((const float*)&Colors::Magenta) }
            };
            UINT indices[] = {
                // front face
                0, 1, 2,
                0, 2, 3,

                // back face
                4, 6, 5,
                4, 7, 6,

                // left face
                4, 5, 1,
                4, 1, 0,

                // right face
                3, 2, 6,
                3, 6, 7,

                // top face
                1, 5, 6,
                1, 6, 2,

                // bottom face
                4, 0, 3,
                4, 3, 7
            };
#pragma endregion
            Buffer vert = createBuffer(8, sizeof(Vertex), ResourceFormat::VERTEX_BUFFER, Access::GpuRead, vertices);
            Buffer inde = createBuffer(36, sizeof(UINT), ResourceFormat::INDEX_BUFFER, Access::GpuRead, indices);
            GOEntity *entity5 = new GOEntity(vert, inde);
            Effect e = Effect::EffectBuilder(L"color.fx", FsModel::FS5_0).build(*main, "ColorTech");
            InputLayout::InputLayoutBuilder il4B;
            il4B.addLayoutElement(LayoutElement("POSITION", DataFormat::R32G32B32_FLOAT, 0));
            il4B.addLayoutElement(LayoutElement("COLOR", DataFormat::R32G32B32A32_FLOAT, 0));
            InputLayout il4 = il4B.build(*main, e);
            auto &shaderVec = game.addObjectS(entity5, DrawCommand(PrimitiveTopology::TRIANGLELIST, PipeState::PipeStateBuilder().addShader(e).build(), il4));
            Effect *eff;
            for (auto ef : shaderVec)
            {
                if (ef->checkIsMe("effect"))
                {
                    eff = (Effect*)ef;
                }
            }
            Matrix proj = Matrix::PerspectiveFovLH(0.25f*3.14f, 1, 1.0f, 1000.0f);
            Matrix view;
            Matrix world;
            Matrix projMatrix_1 = Matrix::PerspectiveFovLH(0.785398163f, 640.0f / height, 0.01f, 100.0f);
            Matrix viewMatrix_1;
            Matrix aa, bb;
            Matrix worldMat1 = Matrix().scale(0.15, 0.15, 0.15).rotationXYZ(0.0f, 0.7f, 0.7f).translation(0, 0, 2);
            eff->setMatrixVar("gWorldViewProj", worldMat1*viewMatrix_1*projMatrix_1);

            msg_Message = { 0 };
            unsigned int counter = 0;
            while (true)
            {
                if (PeekMessage(&msg_Message, NULL, 0, 0, PM_REMOVE))
                {
                    TranslateMessage(&msg_Message);
                    DispatchMessage(&msg_Message);
                }
                else
                {
                    renderDevice->beginRender();
                    game.update();
                    game.render();
                    renderDevice->endRender();
                }
                if (msg_Message.message == WM_QUIT)
                {
                    break;
                }
            }
        }
        DestroyWindow(MainWindow);
    }
    delete main;
    return 0;
}