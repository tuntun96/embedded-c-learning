#include <stdio.h>
#include <stdint.h>

int main(void)
{
    uint8_t charging_active = 1U;
    uint8_t reading = 1U;

    uint16_t voltage = 650U;
    uint16_t current = 120U;

    while (charging_active == 1U)
    {
        printf("Reading %u\n", reading);
        printf("Voltage: %u V\n", voltage);
        printf("Current: %u A\n", current);
        printf("\n");

        reading++;

        if (reading == 6U)
        {
            charging_active = 0U;
        }
    }

    printf("Charging session stopped\n");

    return 0;
}