#include <gtest/gtest.h>
#include <vector>
#include <numeric>

//std::accumulate返回值的类型取决于第三个参数的类型
//掉坑1次
//掉坑原因除了问题本身外还有没有注意编译器警告
//没有注意编译器警告的原因是有太多的祖传代码的警告
//必须把祖传代码修正一下
TEST(accumulate,float)
{
    std::vector<float> array({ 0.1f,0.2f,0.3f,0.4f });
    float sum = std::accumulate(array.begin(), array.end(), 0.0f);
    float average = std::accumulate(array.begin(), array.end(), 0.0f) / array.size();
}