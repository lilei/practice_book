#include <gtest/gtest.h>
#include <fstream>

/*
H264������һ����һ����NALU���
VCL: Video Coding Layer ��Ƶ�����
NAL: Network Abstraction layer  ������ȡ��
NALU: NAl��Ԫ 
RBSP:Raw Byte Sequence Payload

+-----+-------------+------+-------------+------+-------------+------+-------------+------+-----+
| ... | nalu_header | RBSP | nalu_header | RBSP | nalu_header | RBSP | nalu_header | RBSP | ... |
+-----+-------------+------+-------------+------+-------------+------+-------------+------+-----+
      |<-  1 Byte ->|

+-------+          +--------+          +--------------+          +--------------+
| frame | -+-----> | slice1 | -+-----> |  slice data  | -+-----> | macro block1 |
+-------+  |       +--------+  |       +--------------+  |       +--------------+
           |       +--------+  |       +--------------+  |       +--------------+
           +-----> | slice2 |  +-----> | slice header |  +-----> | macro block2 |
           |       +--------+          +--------------+  |       +--------------+
           |       +--------+                            |       +--------------+
           +-----> | slice3 |                            +-----> | macro block3 |
                   +--------+                                    +--------------+

slice ���ͣ�
0 P-slice  �ɰ���P��I���
1 B-slice  �ɰ���B��I���
2 I-slice  ֻ����I���
3 SP-slice ���ڲ�ͬ������֮����л�������P��/��I���
4 SI-slice ����һ���������͵ı�����,����SI���
*/
class H264
{
    struct NaluHeader
    {
        uint8_t type               : 5;
        uint8_t ref_idc            : 2;
        uint8_t forbidden_zero_bit : 1;
    };

    enum NaluType
    {
        NALU_TYPE_SLICE    = 1,
        NALU_TYPE_DPA      = 2,
        NALU_TYPE_DPB      = 3,
        NALU_TYPE_DPC      = 4,
        NALU_TYPE_IDR      = 5,
        NALU_TYPE_SEI      = 6,
        NALU_TYPE_SPS      = 7,          //���в�����������IDRͼ��������ͼ����Ϣ����ͼ��ߴ磬��Ƶ��ʽ�ȣ� 
        NALU_TYPE_PPS      = 8,          //ͼ�������(һ��ͼ������з�Ƭ�����������Ϣ������ͼ�����ͣ����кŵ�)
        NALU_TYPE_AUD      = 9,
        NALU_TYPE_EOSEQ    = 10,
        NALU_TYPE_EOSTREAM = 11,    
        NALU_TYPE_FILL     = 12,        //�ֽڶ���
    };
    enum NaluPriority
    {
        NALU_PRIORITY_DISPOSABLE = 0,
        NALU_PRIRITY_LOW         = 1,
        NALU_PRIORITY_HIGH       = 2,
        NALU_PRIORITY_HIGHEST    = 3
    };

public:
    H264(std::fstream& fs)
        :fs_(fs),state_(State_0),last_start_pos_(-1)
    {}

    void parse()
    {
        if (!fs_)
        {
            return;
        }
        std::cout << "----+" << "--------+" << "--------+" << "-------+" << "-----+" << std::endl;
        std::cout << "NUM |" << "    POS |" << "    IDC |" << "  TYPE |" << " LEN |" << std::endl;
        std::cout << "----+" << "--------+" << "--------+" << "-------+" << "-----+" << std::endl;
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
                if (State_2 == state_)
                {
                    got_nalu();
                    state_ = State_0;
                }
                else if (State_3 == state_)
                {
                    got_frame();
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
    {
        //the last nalue length
        if (-1 != last_start_pos_)
        {
            if (State_2 == state_)
            {
                std::cout << std::setw(5) << (int64_t)fs_.tellg() - last_start_pos_ - 3 << "|" << std::endl;
            }
            else if (State_3 == state_)
            {
                std::cout << std::setw(5) << (int64_t)fs_.tellg() - last_start_pos_ - 4 << "|" << std::endl;
            }

        }

        static int num = 0;
        std::cout <<std::setw(4) << num++ <<"|";

        last_start_pos_ = fs_.tellg();
        std::cout <<std::setw(8) << last_start_pos_ <<"|";

        NaluHeader header;
        fs_.read((char*)&header, 1);
        switch (header.ref_idc)
        {
        case NALU_PRIORITY_DISPOSABLE:
            std::cout << std::setw(9) << "DISPOS|";
            break;
        case NALU_PRIRITY_LOW:
            std::cout << std::setw(9)<<"LOW|";
            break;
        case NALU_PRIORITY_HIGH:
            std::cout << std::setw(9)<<"HIGH|";
            break;
        case NALU_PRIORITY_HIGHEST:
            std::cout << std::setw(9)<<"HIGHEST|";
            break;
        default:
            break;
        }

        switch (header.type)
        {
        case NALU_TYPE_SLICE:
            std::cout <<std::setw(8) << "SLICE|";
            break;
        case NALU_TYPE_DPA:
            std::cout <<std::setw(8) << "DPA|";
            break;
        case NALU_TYPE_DPB:
            std::cout <<std::setw(8) << "DPB|";
            break;
        case NALU_TYPE_DPC:
            std::cout <<std::setw(8) << "DPC|";
            break;
        case NALU_TYPE_IDR:
            std::cout <<std::setw(8) << "IDR|";
            break;
        case NALU_TYPE_SEI:
            std::cout <<std::setw(8) << "SEI|";
            break;
        case NALU_TYPE_SPS:
            std::cout <<std::setw(8) << "SPS|";
            break;
        case NALU_TYPE_PPS:
            std::cout <<std::setw(8) << "PPS|";
            break;
        case NALU_TYPE_AUD:
            std::cout <<std::setw(8) << "AUD|";
            break;
        case NALU_TYPE_EOSEQ:
            std::cout <<std::setw(8) << "EOSEQ|";
            break;
        case NALU_TYPE_EOSTREAM:
            std::cout <<std::setw(8) << "EOSTREAM|";
            break;
        case NALU_TYPE_FILL:
            std::cout <<std::setw(8) << "FILL|";
            break;
        default:
            std::cout << "unkonwn nalu type";
            break;
        }
    }

    void got_frame()
    {
        got_nalu();
    }
    std::fstream &fs_;
    enum State
    {
        State_0,
        State_1,
        State_2,
        State_3
    };
    State state_;
    int64_t last_start_pos_;
};

TEST(h264,parse)
{
    std::fstream fin("../resource/sintel.h264",std::ios_base::binary | std::ios_base::in);
    H264 h264(fin);
    h264.parse();
}
