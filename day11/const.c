#include <stdio.h>
#include <stdint.h>

const uint16_t max_voltage = 900U;
const uint16_t max_current = 200U;
const uint8_t max_temperature = 80U;

int main(void)
{
    uint16_t voltage = 650U;
    uint16_t current = 120U;
    uint8_t temperature = 45U;

    printf(
      "EV Charger Safety Monitor\n"
    );
    printf("\n");

    printf(
      "Maximum Voltage: %u V\n",
      (unsigned int)max_voltage
    );
    printf(
      "Maximum Current: %u A\n",
      (unsigned int)max_current
    );
    printf(
      "Maximum Temperature: %u C\n",
      (unsigned int)max_temperature
    );
     printf("\n");

     printf(
      "Measured Voltage: %u V\n",
      (unsigned int)voltage
    );
    printf(
      "Measured Current: %u A\n",
      (unsigned int)current
    );
    printf(
      "Measured Temperature: %u C\n",
      (unsigned int)temperature
    );
    printf("\n");

    if (voltage > max_voltage)
    {
      printf(
        "Voltage Status: OVER-VOLTAGE FAULT\n"
      );
    }
    else
    {
      printf(
        "Voltage Status: NORMAL\n"
      );
    }
    if (current > max_current)
    {
      printf(
        "Current Status: OVER-CURRENT FAULT\n"
      );
    }
    else
    {
      printf(
        "Current Status: NORMAL\n"
      );
    }
    if (temperature > max_temperature)
    {
      printf(
        "Temperature Status: OVER-TEMPERATURE FAULT\n"
      );
    }
    else
    {
      printf(
        "Temperature Status: NORMAL\n"
      );
    }

    return 0;
}