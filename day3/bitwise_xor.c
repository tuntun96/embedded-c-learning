#include<stdint.h>
#include<stdio.h>

int main(void)
{
   uint8_t a = 10U;
   uint8_t b = 12;

   uint8_t result = a ^ b;

   printf("A = %u\n", a);
   printf("B = %u\n", b);
   printf("A ^ B = %u\n", result);

  return 0;
}