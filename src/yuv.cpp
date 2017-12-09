#include <gtest/gtest.h>
#include <fstream>
#include <stdint.h>
#include <algorithm>

class YUV 
{
public:
    enum YUV_FORMAT
    {
        YUV_420,
        YUV_444
    };
    YUV(std::fstream& fs,int width,int height,YUV_FORMAT format)
        :file_(fs),width_(width),height_(height),format_(format)
    {
        init_size();
    }

    void separate(std::ofstream &fy, std::ofstream &fu, std::ofstream& fv)
    {
        if (!fy || !fu || !fv)
        {
            return;
        }

        size_t buffer_size = std::max({ size_y_, size_u_, size_v_ });
        char* buffer = new char[buffer_size];
        for (;file_ && !file_.eof();)
        {
            file_.read(buffer, size_y_);
            if (file_.eof())
            {
                break;
            }
            fy.write(buffer, size_y_);

            file_.read(buffer, size_u_);
            if (file_.eof())
            {
                break;
            }
            fu.write(buffer, size_u_);

            file_.read(buffer, size_v_);
            if (file_.eof())
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
        for (;file_ && !file_.eof();) 
        {
            file_.read(buffer,pic_size_);
            if (file_.eof())
            {
                break;
            }
            memset(buffer + size_y_,128,pic_size_ - size_y_);
            fgray.write(buffer, pic_size_);
        }
    }

    void dark(float rate,std::ofstream& fdark)
    {
        file_.seekp(0);
        if (!fdark)
        {
            return;
        }

        uint8_t *buffer = new uint8_t[pic_size_];
        for (;file_ && !file_.eof();) 
        {
            file_.read((char*)buffer,pic_size_);
            for (size_t i = 0;i < size_y_;i++)
            {
                buffer[i] = static_cast<uint8_t>(buffer[i] * rate);
            }
            fdark.write((char*)buffer, pic_size_);
        }
        delete[] buffer;
    }

    void border(std::ofstream& fborder,int border)
    {
        uint8_t *buffer = new uint8_t[pic_size_];
        for (; file_ && !file_.eof();)
        {
            file_.read((char*)buffer, pic_size_);
            for (int i = 0; i < height_;i++)
            {
                for (int j = 0;j < width_;j++)
                { 
                    int harf_border = border / 2;
                    if (j < border || j > width_ - border || i < border || i > height_ - border)
                    {
                        buffer[i * width_ + j] = 255;
                    }

                    if (j < harf_border || j > width_ - harf_border  || i < harf_border || i > height_ - harf_border)
                    {
                        buffer[i * width_ + j] = 128;
                    }

                }
            }
            fborder.write((char*)buffer,pic_size_);
        }
        delete[] buffer;
    }

    void gray_bar(int min_y,int max_y,int bar_count)
    {
        int y_width = 0;
        int u_width = 0;
        int v_width = 0;

        int y_height = 0;
        int u_height = 0;
        int v_height = 0;

        if (YUV_420 == format_)
        {
            y_width = width_;
            u_width = width_ / 2;
            v_width = width_ / 2;

            y_height = height_;
            u_height = height_ / 2;
            v_height = height_ / 2;
        }
        else if (YUV_444 == format_)
        {
            y_width = width_;
            u_width = width_;
            v_width = width_;

            y_height = height_;
            u_height = height_;
            v_height = height_;
        }
        uint8_t *buffer_y = new uint8_t[y_width * y_height];
        uint8_t *buffer_u = new uint8_t[u_width * u_height];
        uint8_t *buffer_v = new uint8_t[v_width * u_height];

        int bar_width = width_ / bar_count;
        float lum_inc = (float)(max_y - min_y) / (float)(bar_count - 1);
        for (int i = 0; i < y_height; i++)
        {
            for (int j = 0; j < y_width; j++)
            {
                buffer_y[i * y_width + j] = (uint8_t)((j / bar_width) * lum_inc);
            }
        }

        for (int i = 0; i < u_height; i++)
        {
            for (int j = 0; j < u_width; j++)
            {
                buffer_u[i * u_width + j] = 128;
            }
        }

        for (int i = 0; i < v_height; i++)
        {
            for (int j = 0; j < v_width; j++)
            {
                buffer_v[i * u_width + j] = 128;
            }
        }

        file_.write((char*)buffer_y, size_y_);
        file_.write((char*)buffer_u, size_u_);
        file_.write((char*)buffer_v, size_v_);

        delete[] buffer_y;
        delete[] buffer_u;
        delete[] buffer_v;
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
            pic_size_ = width_ * height_ * 3;
            size_y_   = pic_size_ / 3;
            size_u_   = pic_size_ / 3;
            size_v_   = pic_size_ / 3;
        }
    }

    std::fstream& file_;
    int width_;
    int height_;
    YUV_FORMAT  format_;
    size_t pic_size_;
    size_t size_y_;
    size_t size_u_;
    size_t size_v_;
};


std::fstream file_lena_420()
{
    std::fstream fyuv420("lena_256x256_yuv420p.yuv", std::fstream::binary | std::ios_base::in);
    return fyuv420;
}

std::fstream file_lena_444()
{
    std::fstream fyuv444("lena_256x256_yuv444p.yuv", std::fstream::binary | std::ios_base::in);
    return fyuv444;
}

TEST(yuv,separate)
{
    std::fstream fyuv420 = file_lena_420();
    if (fyuv420)
    {
        YUV yuv(fyuv420,256,256,YUV::YUV_420);
        std::ofstream fy("420_y.yuv",std::fstream::binary);
        std::ofstream fu("420_u.yuv",std::fstream::binary);
        std::ofstream fv("420_v.yuv",std::fstream::binary);
        yuv.separate(fy,fu,fv);
    }

    std::fstream fyuv444 = file_lena_444();
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
    std::fstream fyuv420 = file_lena_420();
    if (fyuv420)
    {
        YUV yuv(fyuv420,256,256,YUV::YUV_420);
        std::ofstream fgray("lena_gray_420.yuv",std::fstream::binary);
        yuv.gray(fgray);
    }

    std::fstream fyuv444 = file_lena_444();
    if (fyuv444)
    {
        YUV yuv(fyuv444,256,256,YUV::YUV_444);
        std::ofstream fgray("lena_gray_444.yuv",std::fstream::binary);
        yuv.gray(fgray);
    }
}

TEST(yuv,dark)
{
    std::fstream fyuv420 = file_lena_420();
    if (fyuv420)
    {
        YUV yuv(fyuv420,256,256,YUV::YUV_420);

        std::ofstream fdark("lena_dark_420.yuv",std::fstream::binary);
        yuv.dark(0.2f,fdark);
    } 

    std::fstream fyuv444 = file_lena_444();
    if (fyuv444)
    {
        YUV yuv(fyuv444,256,256,YUV::YUV_444);
        std::ofstream fdark("lena_light_444.yuv",std::fstream::binary);
        yuv.dark(2,fdark);
    } 
}

TEST(yuv,border)
{
    std::fstream fyuv420 = file_lena_420();
    if (fyuv420)
    {
        YUV yuv(fyuv420, 256, 256, YUV::YUV_420);
        std::ofstream fborder("lena_border_420.yuv",std::fstream::binary);
        yuv.border(fborder,20);
    }
}

TEST(yuv, gray_bar)
{
    std::fstream fbar("gray_bar_420.yuv",std::fstream::binary | std::fstream::out);
    if (fbar)
    {
        YUV yuv(fbar,960,540,YUV::YUV_420);
        yuv.gray_bar(0, 255, 10);
    }

    std::fstream fbar1("gray_bar_444.yuv",std::fstream::binary | std::fstream::out);
    if (fbar)
    {
        YUV yuv(fbar1,960,540,YUV::YUV_444);
        yuv.gray_bar(0, 255, 10);
    }
}
