#include <stdio.h>
#include <stdint.h>

int main(void)
{
    uint16_t charger_data[3]=
    {
        650U,
        120U,
        45U
    };

    uint16_t *ptr = charger_data;

    printf("Voltage = %u V\n", *ptr);
    ptr++;

    printf("Current = %u A \n",*ptr);
    ptr++;

    printf("Tempearture = %u C\n",*ptr);

    return 0;
}