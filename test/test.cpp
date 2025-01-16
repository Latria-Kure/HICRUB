#include <iostream>
#include <stdint.h>
#include "gtest/gtest.h"

class CTest : public ::testing::Test {
protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

TEST_F(CTest, Create)
{
    EXPECT_EQ(1, 2);
}
