/*
    ������ŵ�дwinsocket��ʼ��
    д��main�����main��������û��socket������д�������Ǻ�ͻأ��
    д�ڵ�һ��ʹ�������ʱ�򣿴����ģԽ��Խ���֪�������ǵ�һ��

    �����Ǵ�webrtc���볭����һ��С����:
    1. ����static������main����ǰ�Զ���������ԣ�����һ����̬�����ڶ���Ĺ��������������
    2. ���캯����private�ģ��������class��û�а취��ʹ����
    3. �ѵ���������class�ڲ��������������ǿ��Ա�����ģ���Ϊ�������캯����������accessble��
    4. ��������Ҫд��public�ģ���Ϊ������ʱ��Ҫ�����ķ���Ȩ(VS2017��д��private��Ҳû�й�ϵ)
*/
#if defined(WIN32)
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib") 
class WinsockInit 
{
    static WinsockInit singleton;
    WinsockInit() 
    {
        WSADATA data;
        WSAStartup(MAKEWORD(1, 0), &data);
    }

public:
    ~WinsockInit() 
    {
        WSACleanup(); 
    }
};
WinsockInit WinsockInit::singleton;
#endif

#include <gtest/gtest.h>
TEST(winsock,construct)
{
    //WinsockInit init; // compile error
}
