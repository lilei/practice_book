#include <gtest/gtest.h>
#include <vector>
#include <numeric>

//std::accumulate����ֵ������ȡ���ڵ���������������
//����1��
//����ԭ��������Ȿ���⻹��û��ע�����������
//û��ע������������ԭ������̫����洫����ľ���
//������洫��������һ��
TEST(accumulate,float)
{
    std::vector<float> array({ 0.1f,0.2f,0.3f,0.4f });
    float sum = std::accumulate(array.begin(), array.end(), 0.0f);
    float average = std::accumulate(array.begin(), array.end(), 0.0f) / array.size();
}