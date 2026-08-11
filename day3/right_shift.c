#include<stdint.h>
#include<stdio.h>

int main(void)
{

    uint8_t value = 16U;
    printf("Original value = %u\n", value);
    value = value >> 2;
    printf("After right shift = %u\n", value);
    
    return 0;
}