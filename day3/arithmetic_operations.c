#include<stdint.h>
#include<stdio.h>

int main(void)
{
    uint16_t voltage = 400U;
    uint16_t current = 100U;

    uint32_t power = (uint32_t)voltage * current;

    printf("Voltage = %u V\n", voltage);
    printf("Current = %u A\n", current);
    printf("Power = %lu W\n", (unsigned long)power);

    printf("\nArithmetic Opearation\n");

    printf("100 + 50 = %u\n", 100U + 50U);
    printf("200 - 150 = %u\n", 200U - 150U);
    printf("10 * 5 = %u\n", 10U * 5U);
    printf("10 / 3 = %u\n", 10U / 3U);
    printf("10 %% 3 = %u\n", 10U % 3U );


    return 0;
}

