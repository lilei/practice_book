#include <gtest/gtest.h>
#include <fstream>

class Pcm
{
public:
    Pcm(std::fstream& fs) 
        :fs_(fs)
    {
    }

    void seprate(std::ofstream& fleft,std::ofstream& fright)
    {
        if (!fleft || !fright)
        {
            return;
        }

        int8_t *buffer = new int8_t[4];
        while (fs_ && !fs_.eof())
        {
            fs_.read    ((char*)buffer,4);
            fleft.write ((char*)buffer, 2);
            fright.write((char*)buffer + 2, 2);
        }
        delete[] buffer;
    }


    enum Track
    {
        Tack_Left,
        Tack_Right,
        Tack_Both
    };
    void volume(std::ofstream& out,Track track,float rate)
    {
        if (!out)
        {
            return;
        }
        int8_t *buffer = new int8_t[4];
        while (fs_ && !fs_.eof())
        {
            fs_.read((char*)buffer,4);
            if (fs_.eof())
            {
                break;
            }
            int16_t left = *(int16_t*)buffer;
            int16_t right = *(int16_t*)(buffer + 2);
            switch (track)
            {
            case Tack_Left:
                left = left / 2;
                break;
            case Tack_Right:
                right = right / 2;
                break;
            case Tack_Both:
                left = left / 2;
                right = right / 2;
                break;
            }
            out.write((char*)&left,2);
            out.write((char*)&right,2);
        }
        delete[] buffer;
    }

private:
    std::fstream& fs_;
};

std::fstream file_pcm()
{
    std::fstream fpcm("../resource/NocturneNo2inEflat_44.1k_s16le.pcm", std::fstream::binary | std::ios_base::in);
    return fpcm;
}

TEST(pcm,seprate)
{
    std::fstream fpcm = file_pcm();
    Pcm pcm(fpcm);
    std::ofstream fleft("left.pcm",std::ios::binary);
    std::ofstream frignt("right.pcm",std::ios::binary);
    pcm.seprate(fleft,frignt);
}

TEST(pcm,volume1)
{
    std::fstream fpcm = file_pcm();
    Pcm pcm(fpcm);
    std::ofstream left_down("left_down.pcm",std::ios::binary);
    pcm.volume(left_down, Pcm::Tack_Left, 0.5);
}

TEST(pcm,volume2)
{
    std::fstream fpcm = file_pcm();
    Pcm pcm(fpcm);
    std::ofstream right_down("right_down.pcm",std::ios::binary);
    pcm.volume(right_down, Pcm::Tack_Right, 0.5);
}

TEST(pcm,volume3)
{
    std::fstream fpcm = file_pcm();
    Pcm pcm(fpcm);
    std::ofstream both_down("both_down.pcm",std::ios::binary);
    pcm.volume(both_down, Pcm::Tack_Both, 0.5);
}
