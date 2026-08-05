#include <stdio.h>
#include <stdint.h>

int main(void)
{
    volatile uint16_t charger_voltage = 950U;
    const uint16_t max_voltage = 900U;

    printf(
      "EV Charger Voltage Monitor\n"
    );
    printf("\n");

    printf(
      "Measured Voltage: %u V\n",
      (unsigned int)charger_voltage
    );
    printf(
      "Maximum Voltage: %u V\n",
      (unsigned int)max_voltage
    );
    if (charger_voltage > max_voltage)
    {
      printf(
        "Voltage Status: OVER-VOLTAGE FAULT\n"
      );
    }
    else
    {
      printf("Voltage Status: NORMAL\n"
      );
    }
    

}
