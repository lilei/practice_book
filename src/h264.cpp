#include <gtest/gtest.h>
#include <fstream>

class H264
{
public:
    H264(std::fstream& fs)
        :fs_(fs),state_(State_0)
    {}

    void parse()
    {
        char buffer;
        while (fs_ && !fs_.eof())
        {
            fs_.read(&buffer,1);
            if (buffer == 0x00)
            {
                switch (state_)
                {
                case State_0:
                    state_ = State_1;
                    break;
                case State_1:
                    state_ = State_2;
                    break;
                case State_2:
                    state_ = State_3;
                    break;
                default:
                    break;
                }
            }
            else if (buffer == 0x01)
            {
                if (State_2 == buffer)
                {
                    got_frame();
                    state_ = State_0;
                }
                else if (State_3 == buffer)
                {
                    got_nalu();
                    state_ = State_0;
                }
            }
            else
            {
                state_ = State_0;
            }
        }
    }
private:
    void got_nalu()
    {}
    void got_frame()
    {}
    std::fstream &fs_;
    enum State
    {
        State_0,
        State_1,
        State_2,
        State_3
    };
    State state_;
};
