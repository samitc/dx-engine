#include "gtest\gtest.h"
#include <string>
#include "Device.h"
#include "inputLayout.h"
#include "LayoutElement.h"
#include "DxEnums.h"

namespace GeneralTesting
{
    TEST(LayoutElementTesting, TestCtor)
    {
        DataFormat format = DataFormat::R8G8B8A8_UNORM;
        const int size = 4;
        const int index = 0;
        const char *name = "elm1";
        LayoutElement le = LayoutElement(name, format, index);
        ASSERT_TRUE(format == le.getFormat()) << "Error in format";
        ASSERT_EQ(index, le.getIndex());
        ASSERT_TRUE(strcmp(name, le.getSemanticName()) == 0) << "Error while checking semantic name";
        ASSERT_EQ(size, le.getSize());
    }
    TEST(LayoutElementTesting, TestCopyCtor)
    {
        DataFormat format = DataFormat::R32_TYPELESS;
        const int size = 4;
        const int index = 543;
        const char *name = "elm241";
        LayoutElement leC = LayoutElement(name, format, index);
        LayoutElement le = LayoutElement(leC);
        ASSERT_TRUE(format == le.getFormat()) << "Error in format";
        ASSERT_EQ(index, le.getIndex());
        ASSERT_TRUE(strcmp(name, le.getSemanticName()) == 0) << "Error while checking semantic name";
        ASSERT_EQ(size, le.getSize());
    }
    TEST(LayoutElementTesting, TestInit)
    {
        DataFormat format = DataFormat::R32_TYPELESS;
        int size = 4;
        int index = 543;
        char *name = "elm241";
        LayoutElement le = LayoutElement(name, format, index);
        ASSERT_TRUE(format == le.getFormat()) << "Error in format";
        ASSERT_EQ(index, le.getIndex());
        ASSERT_TRUE(strcmp(name, le.getSemanticName()) == 0) << "Error while checking semantic name";
        ASSERT_EQ(size, le.getSize());
        format = DataFormat::R32G32B32A32_UINT;
        size = 16;
        index = 4543;
        name = "gfdrth";
        le = LayoutElement(name, format, index);
        ASSERT_TRUE(format == le.getFormat()) << "Error in format";
        ASSERT_EQ(index, le.getIndex());
        ASSERT_TRUE(strcmp(name, le.getSemanticName()) == 0) << "Error while checking semantic name";
        ASSERT_EQ(size, le.getSize());
    }
}