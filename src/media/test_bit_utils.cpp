#include "bit_utils.h"
#include <gtest/gtest.h>
#include <array>

TEST(bit_utils,extract_from_byte)
{
    unsigned char data1[] = { 0x80 };
    EXPECT_EQ(std::vector<bool>({ 1,0,0,0,0,0,0,0 }), extract_bits(data1, 0, 8));

    unsigned char data2[] = { 0xa5 };
    EXPECT_EQ(std::vector<bool>({ 1,0,1,0,0,1,0,1 }), extract_bits(data2, 0, 8));


    unsigned char data3[] = { 0xf0 };
    EXPECT_EQ(std::vector<bool>({ 1,1,1,1 }), extract_bits(data3, 0, 4));


    unsigned char data4[] = { 0x9f };
    EXPECT_EQ(std::vector<bool>({ 1,0,0,1 }), extract_bits(data4, 0, 4));


    unsigned char data5[] = { 0xbf };
    EXPECT_EQ(std::vector<bool>({ 1,0 }), extract_bits(data5, 0, 2));


    unsigned char data6[] = { 0xb9 };
    EXPECT_EQ(std::vector<bool>({ 1,0,0,1 }), extract_bits(data6, 4, 4));


    unsigned char data7[] = { 0xa9 };
    EXPECT_EQ(std::vector<bool>({ 1,0,1 }), extract_bits(data7, 2, 3));
}

TEST(bit_utils, extract_from_2char)
{
    unsigned char data[] = { 0xf2,0xaf };
    EXPECT_EQ(std::vector<bool>({ 0,0,1,0,1,0,1,0 }), extract_bits(data, 4, 8));
}

TEST(bit_utils, extract_from_3char)
{
    unsigned char data[] = { 0xf5,0xf2,0xaf };
    EXPECT_EQ(std::vector<bool>({ 1,0,1,1,1,1,1,0,0,1,0,1,0,1 }), extract_bits(data, 5, 14));
}

TEST(bit_utils, extract_from_3char_with_prefix)
{
    unsigned char data[] = { 0xff,0xf5,0xf2,0xaf };
    EXPECT_EQ(std::vector<bool>({ 1,0,1,1,1,1,1,0,0,1,0,1,0,1 }), extract_bits(data, 13, 14));
}

