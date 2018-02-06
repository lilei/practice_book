#include <gtest/gtest.h>
#include <fstream>

TEST(fstream,construct)
{
    std::fstream f0;
    std::fstream f1("test.bin", std::ios::binary | std::ios::out);
    f1 << "xxx";
    std::fstream f2("example.txt");
    std::fstream f3(std::move(f1));

    std::ofstream fout("out");
    std::ifstream fin("in");
}
