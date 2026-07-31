#include<stdio.h>
#include<stdint.h>

int main(void)
{
    uint16_t voltage = 650U;
    uint8_t sample;

    for( sample = 1U; sample <= 5; sample++)
    {
      printf("Voltage Sample %u: %u\n", sample, voltage);
    }
}