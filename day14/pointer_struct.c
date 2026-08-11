#include <stdio.h>
#include <stdint.h>

struct charger
{
    uint16_t voltage;
    uint16_t current;
    uint16_t temperature;
};

int main(void)
{
    struct charger data;
    struct charger *ptr = &data;

    ptr->voltage = 750U;
    ptr->current = 150U;
    ptr->temperature = 65U;

    printf("Voltage     = %u V\n", ptr->voltage);
    printf("Current     = %u A\n", ptr->current);
    printf("Temperature = %u C\n", ptr->temperature);
    
    return 0;

}