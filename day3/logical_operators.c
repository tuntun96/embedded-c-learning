#include<stdint.h>
#include<stdio.h>

int main(void)
{
    uint8_t gun_connected = 1U;
    uint8_t fault_active = 0U;

    if((gun_connected == 1U) &&
       (fault_active == 0U))
    {
        printf("Charging can start\n");
    }

    if((gun_connected == 0U) ||
       (fault_active == 1U))
    {
        printf("Charging cannot start\n");
    }


    if (!fault_active)
    {
        printf("No fault is active\n");
    }


    return 0;
}
