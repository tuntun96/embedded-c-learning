#include <stdio.h>
#include <stdint.h>

int main(void)
{
    uint8_t temperature = 65U;

    printf("Charger Temperature: %u C\n", temperature);

    if (temperature > 70U)
    {
        printf("Status: Overtemperature fault\n");
    }
    else if (temperature > 50U)
    {
        printf("Status: High temperature warning\n");
    }
    else
    {
        printf("Status: Temperature normal\n");
    }

    return 0;
}