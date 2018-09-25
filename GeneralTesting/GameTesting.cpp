#include "stdafx.h"
#include "CppUnitTest.h"

#include "Game.h"
#include "DXMain.h"
#include "VideoMode.h"
#include "Device.h"
#include "DrawCommand.h"
#include "PipeState.h"
#include "GUMock.h"
#include "GameTimeMock.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace GeneralTesting
{
	TEST_CLASS(GameTesting)
	{
	public:
		TEST_METHOD(ObjectTesting)
		{
			const int GAME_OBJECT_NUM = 5;
			const int CONTAINER_NUM = 2;
			const int UPDATE_CHECK = 4;
			const long double SECOND = 10;
			const long double DIFFRENCE = 0.001;
            Game g = Game(DXMain().createRenderDevice(nullptr, VideoMode(DataFormat::B8G8R8A8_UNORM)));
            DrawCommand d(PrimitiveTopology::TRIANGLELIST, PipeState::PipeStateBuilder().build(), InputLayout());
			GUMock* objects[CONTAINER_NUM][GAME_OBJECT_NUM];
			for (size_t i = 0; i < CONTAINER_NUM; i++)
			{
				for (size_t j = 0; j < GAME_OBJECT_NUM; j++)
				{
					g.addObject(objects[i][j],d);
				}
			}
			g.update();
			const RenderDevice *dev = objects[0][0]->getLastDevice();
			for (size_t i = 0; i < UPDATE_CHECK; i++)
			{
				g.update();
				GameTime time = objects[0][0]->getLastGameTime();
				for (size_t j = 0; j < CONTAINER_NUM; j++)
				{
					for (size_t z = 0; z < GAME_OBJECT_NUM; z++)
					{
						Assert::IsTrue(dev == objects[j][z]->getLastDevice());
					}
				}
			}
			g.update();
			GameTime bef = objects[0][0]->getLastGameTime();
			__int64 a, b, f;
			QueryPerformanceCounter((LARGE_INTEGER*)&a);
			QueryPerformanceFrequency((LARGE_INTEGER*)&f);
			b = a;
			while ((b - a) / f < SECOND)
			{
				QueryPerformanceCounter((LARGE_INTEGER*)&b);
			}
			g.update();
			GameTime aft = objects[0][0]->getLastGameTime();
		}
		TEST_METHOD(GameTimeTesting)
		{
			const double DIFFRENCE = 0.00001;
            Game g = Game(DXMain().createRenderDevice(nullptr, VideoMode(DataFormat::B8G8R8A8_UNORM)));
            DrawCommand d(PrimitiveTopology::TRIANGLELIST, PipeState::PipeStateBuilder().build(), InputLayout());
			GUMock *e = new GUMock();
			g.addObject(e,d);
			GameTimeMock::TestClock::time_point s, en;
			s = GameTimeMock::TestClock::now();
			g.startGame();
			g.update();
			GameTimeMock stime(e->getLastGameTime());
			const int count = 100000;
			for (size_t i = 0; i < count; i++)
			{

			}
			en = GameTimeMock::TestClock::now();
			g.endGame();
			g.update();
			GameTimeMock etime(e->getLastGameTime());
			std::wstring str;
			str.append(L"s=");
			str.append(std::to_wstring(s.time_since_epoch().count()));
			str.append(L",stime=");
			str.append(std::to_wstring(stime.getStartTime().time_since_epoch().count()));
			Assert::IsTrue(std::chrono::duration_cast<std::chrono::duration<long double, std::ratio<1, 1>>>(s - stime.getStartTime()).count()<DIFFRENCE, str.c_str());
			str.clear();
			str.append(L"en=");
			str.append(std::to_wstring(en.time_since_epoch().count()));
			str.append(L",etime=");
			str.append(std::to_wstring(etime.getEndTime().time_since_epoch().count()));
			Assert::IsTrue(std::chrono::duration_cast<std::chrono::duration<long double, std::ratio<1, 1>>>(en - etime.getEndTime()).count()<DIFFRENCE, str.c_str());
			str.clear();
			str.append(L"s=");
			str.append(std::to_wstring(s.time_since_epoch().count()));
			str.append(L",etime=");
			str.append(std::to_wstring(etime.getStartTime().time_since_epoch().count()));
			Assert::IsTrue(std::chrono::duration_cast<std::chrono::duration<long double, std::ratio<1, 1>>>(s - etime.getStartTime()).count()<DIFFRENCE, str.c_str());
		}
		TEST_METHOD(GameUpdate)
		{
			const long double SECOND = 10;
			const double DIFFRENCE = 0.0001;
            Game g = Game(DXMain().createRenderDevice(nullptr, VideoMode(DataFormat::B8G8R8A8_UNORM)));
            DrawCommand d(PrimitiveTopology::TRIANGLELIST, PipeState::PipeStateBuilder().build(), InputLayout());
			GObjectMock *e = new GObjectMock();
			g.addObject(e,d);
			__int64 a, b, f;
			QueryPerformanceFrequency((LARGE_INTEGER*)&f);
			QueryPerformanceCounter((LARGE_INTEGER*)&a);
			g.update();
			b = a;
			while ((b - a) / f < SECOND)
			{
				QueryPerformanceCounter((LARGE_INTEGER*)&b);
			}
			g.update();
			std::wstring str;
			str.append(L"g=");
			str.append(std::to_wstring(g.getSecondSinceLastUpdate()));
			Assert::IsTrue(SECOND - g.getSecondSinceLastUpdate() < DIFFRENCE, str.c_str());
		}
		TEST_METHOD(GameRender)
		{
			const long double SECOND = 10;
			const double DIFFRENCE = 0.0001;
            RenderDevice * r;
            Game g = Game(*r);
            DrawCommand d(PrimitiveTopology::TRIANGLELIST, PipeState::PipeStateBuilder().build(), InputLayout());
			GObjectMock *e = new GObjectMock();
			g.addObject(e,d);
			__int64 a, b, f;
			QueryPerformanceFrequency((LARGE_INTEGER*)&f);
			QueryPerformanceCounter((LARGE_INTEGER*)&a);
			g.render();
			b = a;
			while ((b - a) / f < SECOND)
			{
				QueryPerformanceCounter((LARGE_INTEGER*)&b);
			}
			g.render();
			std::wstring str;
			str.append(L"g=");
			str.append(std::to_wstring(g.getSecondSinceLastRender()));
			Assert::IsTrue(SECOND - g.getSecondSinceLastRender() < DIFFRENCE, str.c_str());
		}
	};
}