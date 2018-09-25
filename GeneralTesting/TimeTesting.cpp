#include "gtest\gtest.h"
#include "GameTime.h"
#include <thread>

namespace GeneralTesting
{
    const long double TIME_DIFREENCE = 0.5;
    TEST(TimeTesting, reg)
    {
        const long double INIT_VALUE = 0;
        GameTime gt = GameTime();
        gt.GameStart();
        ASSERT_TRUE(gt.getSecondSinceLastRender() - TIME_DIFREENCE < 0) << "Error getting second since last render";
        ASSERT_TRUE(gt.getSecondSinceLastUpdate() - TIME_DIFREENCE < 0) << "Error getting second since last update";
        ASSERT_EQ((unsigned __int64)0, gt.GetFrameRNum());
        ASSERT_EQ((unsigned __int64)0, gt.GetFrameUNum());
        ASSERT_TRUE(INIT_VALUE == gt.gettotalTimeRender()) << "Error getting total render";
        ASSERT_TRUE(INIT_VALUE == gt.gettotalTimeUpdate()) << "Error getting total update";
    }
    TEST(TimeTesting, manyFrames)
    {
        const int TOTAL_TIME = 80;
        GameTime gt = GameTime();
        gt.GameStart();
        auto nowTime = std::chrono::steady_clock::now();
        auto endTime = std::chrono::steady_clock::now();
        endTime += std::chrono::duration<int>(TOTAL_TIME);
        unsigned __int64 count = 0;
        do
        {
            count++;
            gt.StartUpdate();
            gt.StartRender();
            gt.EndUpdate();
            gt.StartUpdate();
            gt.EndRender();
            gt.EndUpdate();
            nowTime = std::chrono::steady_clock::now();
        } while (endTime > nowTime);
        gt.StartUpdate();
        gt.StartRender();
        gt.EndUpdate();
        gt.EndRender();
        ASSERT_TRUE(gt.getSecondSinceLastUpdate() - TIME_DIFREENCE < 0) << "Error getting second since last update";
        ASSERT_TRUE(gt.getSecondSinceLastRender() - TIME_DIFREENCE < 0) << "Error getting second since last render";
        ASSERT_EQ(count + 1, gt.GetFrameRNum());
        ASSERT_EQ(count * 2 + 1, gt.GetFrameUNum());
        const int WAIT_TIME = 5;
        std::this_thread::sleep_for(std::chrono::seconds(WAIT_TIME));
        ASSERT_TRUE(gt.getSecondSinceLastUpdate() - TIME_DIFREENCE - WAIT_TIME < 0) << "Error getting second since last update";
        ASSERT_TRUE(gt.getSecondSinceLastRender() - TIME_DIFREENCE - WAIT_TIME < 0) << "Error getting second since last render";
    }
    TEST(TimeTesting, totalTime)
    {
        const int TOTAL_TIME = 120;
        GameTime gt = GameTime();
        gt.GameStart();
        auto nowTime = std::chrono::steady_clock::now();
        auto endTime = std::chrono::steady_clock::now();
        endTime += std::chrono::duration<int>(TOTAL_TIME);
        gt.StartUpdate();
        gt.StartRender();
        std::this_thread::sleep_for(std::chrono::milliseconds(TOTAL_TIME * 1000 - 100));
        do
        {
            nowTime = std::chrono::steady_clock::now();
        } while (endTime > nowTime);
        gt.EndUpdate();
        gt.EndRender();
        ASSERT_NEAR(gt.gettotalTimeUpdate(), TOTAL_TIME, TIME_DIFREENCE) << "Error getting total update";
        ASSERT_NEAR(gt.gettotalTimeRender(), TOTAL_TIME, TIME_DIFREENCE) << "Error getting total render";
    }
    TEST(TimeTesting, testUpdateTimeDiff)
    {
        const int TOTAL_TIME = 5;
        GameTime gt = GameTime();
        gt.GameStart();
        auto nowTime = std::chrono::steady_clock::now();
        auto endTime = std::chrono::steady_clock::now();
        endTime += std::chrono::duration<int>(TOTAL_TIME);
        gt.StartUpdate();
        gt.EndUpdate();
        do
        {
            nowTime = std::chrono::steady_clock::now();
        } while (endTime > nowTime);
        gt.StartUpdate();
        ASSERT_NEAR((double)TOTAL_TIME, (double)gt.getSecondSinceLastUpdate(), (double)TIME_DIFREENCE) << "Error getting total update";
        gt.EndUpdate();
    }
    TEST(TimeTesting, testRenderTimeDiff)
    {
        const int TOTAL_TIME = 5;
        GameTime gt = GameTime();
        gt.GameStart();
        auto nowTime = std::chrono::steady_clock::now();
        auto endTime = std::chrono::steady_clock::now();
        endTime += std::chrono::duration<int>(TOTAL_TIME);
        gt.StartRender();
        gt.EndRender();
        do
        {
            nowTime = std::chrono::steady_clock::now();
        } while (endTime > nowTime);
        gt.StartRender();
        ASSERT_NEAR((double)TOTAL_TIME, (double)gt.getSecondSinceLastRender(), (double)TIME_DIFREENCE) << "Error getting total update";
        gt.EndRender();
    }
    TEST(TimeTesting, startStop)
    {
        //TOOD
    }
}