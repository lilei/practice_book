#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>

TEST(lambda,sort)
{
    std::vector<int> v = {3,4,5,1};

    //default
    std::sort(v.begin(),v.end());
    EXPECT_EQ(v, std::vector<int>({1,3,4,5}));

    //lambda
    std::sort(v.begin(), v.end(), [](int a, int b) { return (a > b); });
    EXPECT_EQ(v, std::vector<int>({5,4,3,1}));

    //standard library compare function
    std::sort(v.begin(), v.end(), std::less<int>());
    EXPECT_EQ(v, std::vector<int>({1,3,4,5}));

    std::sort(v.begin(), v.end(), std::greater<int>());
    EXPECT_EQ(v, std::vector<int>({5,4,3,1}));


    //function object
    struct 
    {
        bool operator()(int a,int b) const
        {
            return a < b;
        }
    }comp;
    std::sort(v.begin(),v.end(),comp);
    EXPECT_EQ(v, std::vector<int>({1,3,4,5}));


    for (auto a : v)
    {
        std::cout << a << " ";
    }
    std::cout << std::endl;
}


TEST(lambda, sort1)
{
    struct A
    {
        int a;
        int b;
    };

    std::vector<A> v;
    v.push_back({1,1});
    v.push_back({3,1});
    v.push_back({4,1});
    v.push_back({2,1});
    v.push_back({5,1});

    struct 
    {
        bool operator()(A a,A b) const
        {
            return a.a < b.a;
        }
    }comp;

    std::sort(v.begin(), v.end(), comp);

    std::sort(v.begin(), v.end(), [](A a, A b) {return a.a > b.a; });
}
