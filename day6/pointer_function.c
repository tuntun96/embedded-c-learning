#include<stdio.h>
#include<stdint.h>

void update_voltage(uint16_t *voltage);
int main(void)
{
    uint16_t charger_voltage =  650U;
    printf(
      "Before Update: %u V\n",
      (unsigned int)charger_voltage
    );
    update_voltage(&charger_voltage);
    
    printf(
      "After Update: %u V\n",
      (unsigned int)charger_voltage
    );
    return 0;
}

void update_voltage(uint16_t *voltage)
{
    *voltage = 700U;
}