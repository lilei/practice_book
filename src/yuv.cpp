#include <gtest/gtest.h>
#include <fstream>
#include <stdint.h>

int yuv420_separate(std::ifstream& fin, int width, int height, int num)
{
    std::ofstream fy("y.yuv",std::fstream::binary);
    std::ofstream fu("u.yuv",std::fstream::binary);
    std::ofstream fv("v.yuv",std::fstream::binary);

    size_t pic_size = width * height * 3 / 2;
    size_t size_y = pic_size * 2 / 3;
    size_t size_u = pic_size / 6;
    size_t size_v = pic_size / 6;
    char *buffer = new char[pic_size];
    
    for (int i = 0; i < num; i++) 
    {
        fin.read(buffer, size_y);
        fy.write(buffer, size_y);

        fin.read(buffer, size_u);
        fu.write(buffer, size_u);

        fin.read(buffer, size_v);
        fv.write(buffer, size_v);
    }
    delete[] buffer;
    return 0;
}

TEST(yuv,split)
{
    std::ifstream fin ("lena_256x256_yuv420p.yuv",std::fstream::binary);
    if (fin)
    {
        yuv420_separate(fin,256,256,1);
    }
}