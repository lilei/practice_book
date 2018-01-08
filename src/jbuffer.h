#include <stdint.h>
#include <vector>

class Frame
{
public:
    Frame() {};
    ~Frame() {};

    uint16_t seq() const
    {
        return seq_;
    }


private:
    char* data_;
    size_t len_;
    uint32_t seq_;
    uint32_t timestamp_;
};

class FrameList
{
public:
    FrameList() {}
    ~FrameList() {}

    uint16_t &origin()
    {
        return origin_;
    }

    const uint16_t &origin() const
    {
        return origin_;
    }

    bool empty()
    {
        return true;
    }

private:
    uint16_t size_;
    uint16_t origin_;
    uint16_t discard_num_;
};

class Jbuffer
{
public:
    Jbuffer() {};
    ~Jbuffer() {};

    bool put_frame(const Frame &frame)
    {

        return true;
    }

    bool get_frame(Frame &frame)
    {
        return true;
    }
private:
    bool put_at(const Frame& frame)
    {
        enum { MAX_MISORDER = 100 };

        //too late or seq restart
        if (frame.seq() < frame_list_.origin())
        {
            if (frame_list_.origin - frame.seq() > MAX_MISORDER)
            {
                //too late
                return false;
            }
            else
            {
                //restart
                frame_list_.origin() = frame.seq();
            }
        }

        //buffer is empty
        if (frame_list_.empty())
        {

        }
        
    }

    FrameList frame_list_;
};

