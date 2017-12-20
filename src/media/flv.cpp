#include <gtest/gtest.h>
#include <fstream>
#include <assert.h>

struct FlvHeader
{
    char signature[3];
    int8_t version;
    struct Flags
    {
        uint8_t video : 1;
        uint8_t zero2 : 1;
        uint8_t audio : 1;
        uint8_t zero1 : 5;
    }flags;
    uint32_t header_size;
};

struct TagHeader
{
    uint32_t type;         
    uint32_t data_size;    
    uint32_t timestamp;    
    uint32_t timestamp_ex; 
    uint32_t stream_id;    
};

struct AudioDataHead
{
    uint8_t audio_type  : 1;
    uint8_t accuracy    : 1;
    uint8_t sample_rate : 2;
    uint8_t code_type   : 4;
};

struct VideoDataHead
{
    uint8_t code_type  :4;
    uint8_t frame_type :4;
};

enum TagType
{
    AUDIO_TAG  = 0x08,
    VIDEO_TAG  = 0x09,
    SCRIPT_TAG = 0x12,
};

class Flv
{
public:
    Flv(std::fstream& fs)
        :fs_(fs)
    {}

    bool parse()
    {
        if (!fs_)
        {
            return false;
        }
        if (!header())
        {
            return false;
        }
        if (!body())
        {
            return false;
        }
        return true;
    }

private:
    bool header() 
    {
        FlvHeader flv_header = { 0 };
        if (!fs_.read((char*)&flv_header,9))
        {
            return false;
        }
        if (0 != memcmp(flv_header.signature,"FLV",3))
        {
            return false;
        }
        if (0x01 != flv_header.version)
        {
            return false;
        }
        if (0 != flv_header.flags.zero1)
        {
            return false;
        }
        if (1 == flv_header.flags.audio)
        {
            //audio
        }
        if (0 != flv_header.flags.zero2)
        {
            return false;
        }
        if (1 == flv_header.flags.video)
        {
            //video
        }
        if (9 != flv_header.header_size)
        {
            return false;
        }
        return true;
    }

    bool body() 
    {
        int64_t prev_size = 0;
        while (fs_ && !fs_.eof())
        {
            if (prev_size != prev_tag_size())
            {
                return false;
            }

            prev_size = tag();
            if (-1 == prev_size)
            {
                return false;
            }
        }
        return true;
    }

    int prev_tag_size()
    {
        unsigned char temp[4] = { 0 };
        if (!fs_.read((char*)temp,4))
        {
            return -1;
        }

        uint32_t prev_size = reverse_bytes(temp, 4);
        return prev_size;
    }

    int64_t tag()
    {
        TagHeader header = {0};
        if (!tag_header(header))
        {
            return -1;
        }
        if (!tag_data(header.type,header.data_size))
        {
            return -1;
        }
        std::cout << std::endl;
        return (int64_t)(header.data_size + 11);
    }

    bool tag_header(TagHeader& tag_header)
    {
        if (!fs_.read((char*)&tag_header.type, 1))
        {
            return false;
        }

        unsigned char temp[3] = { 0 };

        if (!fs_.read((char*)temp, 3))
        {
            return false;
        }
        tag_header.data_size = reverse_bytes(temp, 3);

        if (!fs_.read((char*)temp, 3))
        {
            return false;
        }
        tag_header.timestamp = reverse_bytes(temp, 3);

        if (!fs_.read((char*)temp, 1))
        {
            return false;
        }
        tag_header.timestamp_ex = (uint32_t)temp[0];

        if (!fs_.read((char*)temp, 3))
        {
            return false;
        }
        tag_header.stream_id = reverse_bytes(temp, 3);

        switch (tag_header.type)
        {
        case AUDIO_TAG :
            std::cout <<std::setw(8) << "[audio]";
            break;
        case VIDEO_TAG:
            std::cout << std::setw(8) << "[video]";
            break;
        case SCRIPT_TAG:
            std::cout << std::setw(8) << "[script]";
            break;
        default:
            std::cout << std::setw(8) << "[unkonwn]";
            break;
        }

        std::cout << std::setw(12) << tag_header.data_size;

        std::cout << std::setw(12) << tag_header.timestamp;

        return true;
    }

    bool tag_data(uint32_t tag_type,size_t len)
    {
        switch (tag_type)
        {
        case AUDIO_TAG:
            audio_data(len);
        break;
        case VIDEO_TAG:
            video_data(len);
            break;
        case SCRIPT_TAG:
            script_data(len);
            break;
        default:
            fs_.seekg(len,std::ios_base::cur);
            break;
        }
        return true;
    }

    bool audio_data(size_t len)
    {
        AudioDataHead head = {0};
        if (!fs_.read((char*)&head,1))
        {
            return false;
        }
        fs_.seekg(len - 1,std::ios_base::cur);

        std::string code_type;
        switch (head.code_type)
        {
            case 0 :code_type =  "Linear PCM, platform endian" ; break;
            case 1 :code_type =  "ADPCM"                       ; break;
            case 2 :code_type =  "MP3"                         ; break;
            case 3 :code_type =  "Linear PCM, little endian"   ; break;
            case 4 :code_type =  "Nellymoser 16-kHz mono"      ; break;
            case 5 :code_type =  "Nellymoser 8-kHz mono"       ; break;
            case 6 :code_type =  "Nellymoser"                  ; break;
            case 7 :code_type =  "G.711 A-law logarithmic PCM" ; break;
            case 8 :code_type =  "G.711 mu-law logarithmic PCM"; break;
            case 9 :code_type =  "reserved"                    ; break;
            case 10:code_type =  "AAC"                         ; break;
            case 11:code_type =  "Speex"                       ; break;
            case 14:code_type =  "MP3 8-Khz"                   ; break;
            case 15:code_type =  "Device-specific sound"       ; break;
            default:code_type =  "UNKNOWN"                     ; break;
        }

        std::string sample_rate;
        switch (head.sample_rate)
        {
        case 0:
            sample_rate = "5.5HZ";
            break;
        case 1:
            sample_rate = "11HZ";
            break;
        case 2:
            sample_rate = "22HZ";
            break;
        case 3:
            sample_rate = "44HZ";
            break;
        default:
            break;
        }

        std::string accuracy;
        switch (head.accuracy)
        {
        case 0:
            accuracy = "8bits";
            break;
        case 1:
            accuracy = "16bits";
            break;
        default:
            accuracy = "unkonwn";
            break;
        }

        std::string audio_type;
        switch (head.audio_type)
        {
        case 0:
            audio_type = "sndMono";
            break;
        case 1:
            audio_type = "sndStereo";
            break;
        default:
            break;
        }


        std::cout << std::setw(10) << code_type
            << " | " << sample_rate
            << " | " << accuracy
            << " | " << audio_type;

        return true;
    }

    bool video_data(size_t len)
    {
        VideoDataHead head = {0};
        fs_.read((char*)&head, 1);
        fs_.seekg(len - 1,std::ios_base::cur);

        std::string frame_type;
        switch (head.frame_type)
        {
        case 1 : frame_type =  "key frame  "             ; break;
        case 2 : frame_type =  "inter frame"             ; break;
        case 3 : frame_type =  "disposable inter frame"  ; break;
        case 4 : frame_type =  "generated keyframe"      ; break;
        case 5 : frame_type =  "video info/command frame"; break;
        default: frame_type =  "UNKNOWN"                 ; break;
        }

        std::string code_type;
        switch (head.code_type)
        {
        case 1 : code_type =  "JPEG (currently unused)"   ; break;
        case 2 : code_type =  "Sorenson H.263"            ; break;
        case 3 : code_type =  "Screen video"              ; break;
        case 4 : code_type =  "On2 VP6"                   ; break;
        case 5 : code_type =  "On2 VP6 with alpha channel"; break;
        case 6 : code_type =  "Screen video version 2"    ; break;
        case 7 : code_type =  "AVC"                       ; break;
        default: code_type =  "UNKNOWN"                   ; break;
        }

        std::cout << std::setw(16) << frame_type << " | " << code_type;
        return true;
    }

    bool script_data(size_t len)
    {
        //AMF1 "onMetaData"
        unsigned char AMF1_filed1 = 0;
        unsigned char AMF1_filed2[2] = {0};
        unsigned char AMF1_filed3[10];

        if (!fs_.read((char*)&AMF1_filed1,1))
        {
            return false;
        }
        if (0x02 != AMF1_filed1)
        {
            return false;
        }

        if (!fs_.read((char*)AMF1_filed2,2))
        {
            return false;
        }
        if (10 != reverse_bytes(AMF1_filed2,2))
        {
            return false;
        }

        if (!fs_.read((char*)AMF1_filed3,10))
        {
            return false;
        }
        if (0 != memcmp("onMetaData",AMF1_filed3,10))
        {
            return false;
        }
            
        //AMF2
        unsigned char AMF2_filed1 = 0;
        unsigned char AMF2_filed2[4] = { 0 };
        unsigned char AMF2_filed3[4] = { 0 };

        if (!fs_.read((char*)&AMF2_filed1,1))
        {
            return false;
        }
        if (0x08 != AMF2_filed1)
        {
            return false;
        }

        if (!fs_.read((char*)&AMF2_filed2,4))
        {
            return false;
        }
        uint16_t array_size = reverse_bytes(AMF2_filed2, 4);
        fs_.seekg(len - 18,std::ios_base::cur);
        return true;
    }

    uint32_t reverse_bytes(unsigned char *temp, int size)
    {
        assert(size <= 4);
        uint32_t r = 0;
        for (int i = 0; i < size; i++)
        {
            int x = temp[i];
            x = x << ((size - 1 - i) * 8);
            r |= x;
        }
        return r;
    }

    std::fstream& fs_;
};

TEST(flv,prase)
{
    std::fstream fin("../resource/cuc_ieschool.flv",std::ios_base::binary | std::ios_base::in);
    Flv flv(fin);
    flv.parse();
}

