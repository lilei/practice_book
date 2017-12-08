#include <gtest/gtest.h>
#include <fstream>
#include <stdint.h>

class YUV 
{
public:
    enum YUV_FORMAT
    {
        YUV_420,
        YUV_444
    };
    YUV(std::ifstream& fin,int width,int height,YUV_FORMAT format)
        :fin_(fin),width_(width),height_(height),format_(format)
    {
    }

    void separate(std::ofstream &fy, std::ofstream &fu, std::ofstream& fv)
    {
        if (!fy || !fu || !fv)
        {
            return;
        }

        size_t pic_size = 0;
        size_t size_y   = 0;
        size_t size_u   = 0;
        size_t size_v   = 0;
        char* buffer    = nullptr;

        if (YUV_420 == format_)
        {
            pic_size = width_ * height_ * 3 / 2;
            size_y   = pic_size * 2 / 3;
            size_u   = pic_size / 6;
            size_v   = pic_size / 6;
        }
        else if (YUV_444 == format_)
        {
            pic_size = width_ * height_;
            size_y   = pic_size;
            size_u   = pic_size;
            size_v   = pic_size;
        }
        buffer = new char[pic_size];

        for (;fin_ && !fin_.eof();)
        {
            fin_.read(buffer, size_y);
            if (fin_.eof())
            {
                break;
            }
            fy.write(buffer, size_y);

            fin_.read(buffer, size_u);
            if (fin_.eof())
            {
                break;
            }
            fu.write(buffer, size_u);

            fin_.read(buffer, size_v);
            if (fin_.eof())
            {
                break;
            }
            fv.write(buffer, size_v);
        }
        delete[] buffer;
    }
private:
    std::ifstream& fin_;
    int width_;
    int height_;
    YUV_FORMAT  format_;
};

TEST(yuv,separate)
{
    std::ifstream fyuv420("lena_256x256_yuv420p.yuv", std::fstream::binary);
    if (fyuv420)
    {
        YUV yuv(fyuv420,256,256,YUV::YUV_420);
        std::ofstream fy("420_y.yuv",std::fstream::binary);
        std::ofstream fu("420_u.yuv",std::fstream::binary);
        std::ofstream fv("420_v.yuv",std::fstream::binary);
        yuv.separate(fy,fu,fv);
    }

    std::ifstream fyuv444("lena_256x256_yuv444p.yuv", std::fstream::binary);
    if (fyuv444)
    {
        YUV yuv(fyuv444,256,256,YUV::YUV_444);
        std::ofstream fy("444_y.yuv",std::fstream::binary);
        std::ofstream fu("444_u.yuv",std::fstream::binary);
        std::ofstream fv("444_v.yuv",std::fstream::binary);
        yuv.separate(fy,fu,fv);
    }
}
