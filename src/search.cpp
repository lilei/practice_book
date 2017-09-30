#include <gtest/gtest.h>

typedef struct Nameval Nameval;
struct Nameval{
    const char* name;
    int value;
};

Nameval htmlchars[] = {
    {"Aelig",0x00c6},
    {"Aacute",0x00c1},
    {"zeta",0x03b6}
};

int lookup(const char* name,Nameval tab[],int ntab)
{
    int low = 0;
    int hight = ntab - 1;
    int mid = 0;
    
    while(low <= hight)
    {
        mid = (low + hight) / 2;
        int cmp = strcmp(name,tab[mid].name);
        if(cmp > 0)
        {
            low = mid - 1;
        }
        else if(cmp < 0)
        {
            hight = mid + 1;
        }
        else
        {
            return mid;
        }
    }
    return -1;
}

TEST(search,binary)
{
    EXPECT_EQ(0,lookup("Aelig",htmlchars,3));
    EXPECT_EQ(1,lookup("Aacute",htmlchars,3));
    EXPECT_EQ(2,lookup("zeta",htmlchars,3));
}
