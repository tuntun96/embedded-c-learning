#include<stdio.h>
#include<stdint.h>

int main(void)
{
    uint8_t charger_status = 0x01U; 

    charger_status = charger_status | 0x04U;

    printf("Charger Status = %u\n", charger_status);
    return 0;
}