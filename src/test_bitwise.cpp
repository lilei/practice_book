#include "bitwise.h"
#include <gtest/gtest.h>

TEST(bitwise,to_uint16)
{
    unsigned char buff[] = { 0xff, 0xf9, 0x50, 0x80, 0x2e, 0x7f };
    bitwise b(buff, sizeof(buff));
    EXPECT_EQ(371,b.to_uint16(30,13));
}