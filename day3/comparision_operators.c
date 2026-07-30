#include<stdint.h>
#include<stdio.h>

int main(void)
{
    uint16_t voltage = 450U;
    uint16_t current = 180U;
    int8_t temperature = 65;

    printf("Voltage > 400:%d\n", voltage > 400U);
    printf("Current == 200: %d\n", current == 200u);
    printf("Temperature > 60 : %d\n", temperature > 60);
    printf("Voltage <= 450: %d\n", voltage <= 450U);

    return 0;
}