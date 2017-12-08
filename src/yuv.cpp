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
        init_size();
    }

    void separate(std::ofstream &fy, std::ofstream &fu, std::ofstream& fv)
    {
        if (!fy || !fu || !fv)
        {
            return;
        }

        char* buffer = new char[pic_size_];
        for (;fin_ && !fin_.eof();)
        {
            fin_.read(buffer, size_y_);
            if (fin_.eof())
            {
                break;
            }
            fy.write(buffer, size_y_);

            fin_.read(buffer, size_u_);
            if (fin_.eof())
            {
                break;
            }
            fu.write(buffer, size_u_);

            fin_.read(buffer, size_v_);
            if (fin_.eof())
            {
                break;
            }
            fv.write(buffer, size_v_);
        }
        delete[] buffer;
    }

    void gray(std::ofstream& fgray)
    {
        if (!fgray)
        {
            return;
        }

        char* buffer = new char[pic_size_];
        for (;fin_ && !fin_.eof();) 
        {
            fin_.read(buffer,pic_size_);
            if (fin_.eof())
            {
                break;
            }
            memset(buffer + size_y_,128,pic_size_ - size_y_);
            fgray.write(buffer, pic_size_);
        }
    }

private:
    void init_size()
    {
        if (YUV_420 == format_)
        {
            pic_size_ = width_ * height_ * 3 / 2;
            size_y_   = pic_size_ * 2 / 3;
            size_u_   = pic_size_ / 6;
            size_v_   = pic_size_ / 6;
        }
        else if (YUV_444 == format_)
        {
            pic_size_ = width_ * height_;
            size_y_   = pic_size_;
            size_u_  = pic_size_;
            size_v_  = pic_size_;
        }
    }

    std::ifstream& fin_;
    int width_;
    int height_;
    YUV_FORMAT  format_;
    size_t pic_size_;
    size_t size_y_;
    size_t size_u_;
    size_t size_v_;
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

TEST(yuv,gray)
{
    std::ifstream fyuv420("lena_256x256_yuv420p.yuv", std::fstream::binary);
    if (fyuv420)
    {
        YUV yuv(fyuv420,256,256,YUV::YUV_420);
        std::ofstream fgray("lena_gray_420.yuv",std::fstream::binary);
        yuv.gray(fgray);
    }
}
