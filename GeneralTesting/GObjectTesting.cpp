#include "gtest\gtest.h"
#include "GObjectMock.h"
#include "GUbjectMock.h"
#include "GraphicStateMock.h"
namespace GeneralTesting
{
    TEST(GObjectTesting, TestDraw)
    {
        GObjectMock *mock = new GObjectMock();
        ASSERT_TRUE(mock->getShouldDrawen());
        mock->setShouldDrawen(false);
        ASSERT_FALSE(mock->getShouldDrawen());
        mock->setShouldDrawen(true);
        ASSERT_TRUE(mock->getShouldDrawen());
    }
    TEST(GObjectTesting, TestUpdate)
    {
        GUbjectMock *mock = new GUbjectMock();
        ASSERT_TRUE(mock->getShouldUpdate());
        mock->setShouldUpdate(false);
        ASSERT_FALSE(mock->getShouldUpdate());
        mock->setShouldUpdate(true);
        ASSERT_TRUE(mock->getShouldUpdate());
    }
}