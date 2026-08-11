#include <stdio.h>
#include <stdint.h>

int main(void)
{
    uint16_t charger_data[3] = {650U, 120U, 45U};
    uint16_t *ptr = charger_data;

    printf("Before Update\n");
    printf("Voltage = %u V\n", *(ptr + 0));
    printf("Current = %u A\n", *(ptr +1));
    printf("Tempearature = %u C\n", *(ptr + 2));

    /* Update using pointer */
    *ptr = 700U;
    *(ptr + 1) = 150U;
    *(ptr + 2) = 50U;

    printf("After Update\n");
    printf("Voltage = %u V\n", *(ptr + 0));
    printf("Current = %u A\n", *(ptr +1));
    printf("Tempearature = %u C\n", *(ptr + 2));


    return 0;
}