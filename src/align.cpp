#include <gtest/gtest.h>
#include <stdint.h>

TEST(align,align)
{
   struct A
   {
       int8_t a;
   }; 
   EXPECT_EQ(1,sizeof(A));

   struct B
   {
       int8_t a;
       int8_t b;
   }; 
   EXPECT_EQ(2,sizeof(B));
   
   struct C
   {
       int8_t a;
       int8_t b;
       int8_t c;
   };
   EXPECT_EQ(3,sizeof(C));

   struct D
   {
       int8_t a;
       int8_t b;
       int8_t c;
       int8_t d;
       int8_t e;
   };
   EXPECT_EQ(5,sizeof(D));

   struct E
   {
       int8_t a;
       int8_t b;
       int8_t c;
       int8_t d;
       int16_t e;
   };
   EXPECT_EQ(6,sizeof(E));


   struct F
   {
       int8_t a;
       int8_t b;
       int8_t c;
       int16_t e;
       int8_t d;
   };
   EXPECT_EQ(8,sizeof(F));
}