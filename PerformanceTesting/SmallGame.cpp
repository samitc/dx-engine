#include "gtest\gtest.h"
#include <Windows.h>
#include <iostream>
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
    LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    class BalModel :public Model
    {
    public:
        Matrix m;
        PipeShadersParams param;
        int handle;
        static const int ALIGNE = 16;
        void* operator new(size_t size)
        {
            if (size == 0)
            {
                size = 1;
            }
            return _aligned_malloc(size, ALIGNE);
        }
        void operator delete(void* p)
        {
            if (p != 0)
            {
                _aligned_free(p);
            }
        }
        void* operator new[](size_t size)
        {
            if (size == 0)
            {
                size = 1;
            }
            return _aligned_malloc(size, ALIGNE);
        }
            void operator delete[](void* p)
        {
            if (p != 0)
            {
                _aligned_free(p);
            }
        }
            BalModel(float startX, float startY, PipeShadersParams &shadParams, int hnd) :Model()
        {
            m = Matrix().scale(0.7f, 0.7f, 0.7f);
            m = m.translation(startX, startY, 0);
            m = m.transpose();
            param = shadParams;
            handle = hnd;
        }
        void prepareRendering(const RenderDevice &renderDevice) override
        {
            Buffer newData = renderDevice.getdx().getResourceManager().createBuffer(1, sizeof(Matrix), ResourceFormat::CONSTANT_BUFFER, Access::GpuRead, &m);
            param.getvertexParams().updateConstantBuffer(handle, newData);
            param.apply(renderDevice.getdx().getDevice());
            Model::prepareRendering(renderDevice);
        }
    };
    class CatModel :public Model, public UObject
    {
        const float MAX = 1;
        const float MIN = -1;
        bool dir;
        float loc;
        PipeShadersParams param;
        int hnd;
    public:
        static const int ALIGNE = 16;
        void* operator new(size_t size)
        {
            if (size == 0)
            {
                size = 1;
            }
            return _aligned_malloc(size, ALIGNE);
        }
        void operator delete(void* p)
        {
            if (p != 0)
            {
                _aligned_free(p);
            }
        }
        void* operator new[](size_t size)
        {
            if (size == 0)
            {
                size = 1;
            }
            return _aligned_malloc(size, ALIGNE);
        }
            void operator delete[](void* p)
        {
            if (p != 0)
            {
                _aligned_free(p);
            }
        }
            CatModel(bool startDir, float startLoc, PipeShadersParams &shadParams, int handle) :Model()
        {
            dir = startDir;
            loc = startLoc;
            param = shadParams;
            hnd = handle;
        }
        void prepareRendering(const RenderDevice &renderDevice) override
        {
            Matrix m;
            m = m.translation(loc, -0.5, 9);
            m = m.transpose();
            Buffer newData = renderDevice.getdx().getResourceManager().createBuffer(1, sizeof(Matrix), ResourceFormat::CONSTANT_BUFFER, Access::GpuRead, &m);
            param.getvertexParams().updateConstantBuffer(hnd, newData);
            param.apply(renderDevice.getdx().getDevice());
            Model::prepareRendering(renderDevice);
        }
        bool update(const GameTime &time) override
        {
            if (loc >= MAX)
            {
                dir = false;
            }
            else if (loc <= MIN)
            {
                dir = true;
            }
            loc += dir ? 0.001f : -0.001f;
            return true;
        }
    };
    const int w = 640;
    const int h = 480;
    const int NUMBER_OF_FRAMES = 40000;
    Performance startSmallGame(HINSTANCE hInstance, HWND hwnd)
    {
        Performance p;
        QueryPerformanceCounter((LARGE_INTEGER*)&p.testStart);
        ModelsDemo demo;
        int frames = 0;
        QueryPerformanceCounter((LARGE_INTEGER*)&p.startLoadContant);
        bool result = demo.Initialize(hInstance, hwnd);
        QueryPerformanceCounter((LARGE_INTEGER*)&p.endLoadContant);
        if (result != false)
        {
            MSG msg = { 0 };
            while (true)
            {
                if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
                frames++;
                demo.Render();
                if (frames == NUMBER_OF_FRAMES)
                {
                    break;
                }
            }
            QueryPerformanceCounter((LARGE_INTEGER*)&p.endGame);
            // Demo Shutdown
            demo.Shutdown();
        }
        QueryPerformanceCounter((LARGE_INTEGER*)&p.endFinish);
        p.startFinish = p.endGame;
        p.startGame = p.endLoadContant;
        return p;
    }
    Performance mySmallGame(HINSTANCE hInstance, HWND hwnd)
    {
        Performance p;
        {
            QueryPerformanceCounter((LARGE_INTEGER*)&p.testStart);
            p.startLoadContant = p.testStart;
            DXMain main;
            VideoAdapter adapter = VideoAdapter::getAdapters()[0];
            main.initDXObject(adapter, FeatureLevel::DX_10_0, true);
            main.initDevice();
            std::vector<VideoMode> vid = VideoOutput(adapter.getAllOutput()[0]).getVideosMode(DataFormat::R8G8B8A8_UNORM);
            size_t videoIndex = 0;
            for (; videoIndex < vid.size() && !(vid[videoIndex].getWidth() == w && vid[videoIndex].getHeight() == h); videoIndex++);
            RenderDevice renderDevice = main.createRenderDevice(hwnd, vid[videoIndex]);
            Game g(renderDevice);
            {
                renderDevice.setBackgroundColor(0, 0, 63);
                Data vsDat = Shader::compileShader(L"WhiteColor.fx", "VS_Main", VertexShader::convertVsModelToString(VsModel::VS4_0));
                VertexShader vs = VertexShader::createVertexShader(main, vsDat);
                PixelShader ps = PixelShader::createPixelShader(main, L"WhiteColor.fx", "PS_Main", PsModel::PS4_0);
                InputLayout::InputLayoutBuilder inb;
                inb.addLayoutElement(LayoutElement("POSITION", DataFormat::R32G32B32_FLOAT, 0));
                DrawCommand drawCommand(PrimitiveTopology::TRIANGLELIST, PipeState::PipeStateBuilder().addShader(vs).addShader(ps).build(), inb.build(main, vsDat));
                Buffer bufproj = main.getResourceManager().createBuffer(1, sizeof(Matrix), ResourceFormat::CONSTANT_BUFFER, (Access)(Access::GpuRead | Access::GpuWrite), 0);
                Buffer bufview = main.getResourceManager().createBuffer(1, sizeof(Matrix), ResourceFormat::CONSTANT_BUFFER, (Access)(Access::GpuRead | Access::GpuWrite), 0);
                Buffer bufworld = main.getResourceManager().createBuffer(1, sizeof(Matrix), ResourceFormat::CONSTANT_BUFFER, (Access)(Access::GpuRead | Access::GpuWrite), 0);
                ArcCamera cam;
                Matrix mat(cam.GetViewMatrix());
                mat = mat.transpose();
                bufview.updateResourceData(main, &mat);
                mat = Matrix::PerspectiveFovLH(XM_PIDIV4, 800.0f / 600.0f, 0.01f, 100.0f).transpose();
                bufproj.updateResourceData(main, &mat);
                PipeShadersParams shaderParams;
                int worldHandle = shaderParams.getvertexParams().addConstantBuffer(bufworld);
                shaderParams.getvertexParams().addConstantBuffer(bufview);
                shaderParams.getvertexParams().addConstantBuffer(bufproj);
                Model::ModelBuilderFromResource ballBuilder("Sphere.obj");
                BalModel *b = new BalModel(-4, 2.6f, shaderParams, worldHandle);
                ballBuilder.build(main, b);
                g.addObject(b, drawCommand);
                b = new BalModel(-4, -2.6f, shaderParams, worldHandle);
                ballBuilder.build(main, b);
                g.addObject(b, drawCommand);
                b = new BalModel(4, -2.6f, shaderParams, worldHandle);
                ballBuilder.build(main, b);
                g.addObject(b, drawCommand);
                b = new BalModel(4, 2.6f, shaderParams, worldHandle);
                ballBuilder.build(main, b);
                g.addObject(b, drawCommand);
                CatModel * m = new CatModel(true, 0, shaderParams, worldHandle);
                Model::ModelBuilderFromResource catBuilder("cat.obj");
                catBuilder.build(main, m);
                g.addObject(m, drawCommand);
                g.addObject(m);
                m = new CatModel(false, 0, shaderParams, worldHandle);
                catBuilder.build(main, m);
                g.addObject(m, drawCommand);
                g.addObject(m);
            }
            QueryPerformanceCounter((LARGE_INTEGER*)&p.endLoadContant);
            int frames = 0;
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
    TEST(SmallGame, smallGame)
    {
        const wchar_t *CLASS_NAME = L"DXTestingSm";
        HINSTANCE hInstance = GetModuleHandle(NULL);
        WNDCLASSEX wndClass = { 0 };
        wndClass.cbSize = sizeof(WNDCLASSEX);
        wndClass.style = CS_HREDRAW | CS_VREDRAW;
        wndClass.lpfnWndProc = WndProc;
        wndClass.hInstance = hInstance;
        wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wndClass.lpszMenuName = NULL;
        wndClass.lpszClassName = CLASS_NAME;
        ASSERT_TRUE(RegisterClassEx(&wndClass)) << "Error register class";
        HWND hwnd = CreateWindow(CLASS_NAME, L"test outer game", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, w, h, NULL, NULL, hInstance, NULL);
        ASSERT_TRUE(hwnd) << "Error creating window";
        ShowWindow(hwnd, SW_SHOW);
        Performance outerGame = startSmallGame(hInstance, hwnd);
        DestroyWindow(hwnd);
        HWND hwnd1 = CreateWindow(CLASS_NAME, L"test my engine", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, w, h, NULL, NULL, hInstance, NULL);
        ASSERT_TRUE(hwnd1) << "Error creating window";
        ShowWindow(hwnd1, SW_SHOW);
        Performance intGame = mySmallGame(hInstance, hwnd1);
        DestroyWindow(hwnd1);
        EXPECT_LT(intGame.endLoadContant - intGame.startLoadContant, outerGame.endLoadContant - outerGame.startLoadContant);
        EXPECT_LT(intGame.endGame - intGame.startGame, outerGame.endGame - outerGame.startGame);
        EXPECT_LT(intGame.endFinish - intGame.startFinish, outerGame.endFinish - outerGame.startFinish);
    }
}