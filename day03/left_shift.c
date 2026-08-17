#include<stdio.h>
#include<stdint.h>

int main(void)
{
    uint8_t value = 1U;
    printf("Original Value = %u\n", value);

    value = value << 1;

    printf("After shift = %u\n", value);


    return 0;
}