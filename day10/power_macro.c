#include <stdio.h>
#include <stdint.h>


/* Function-Like a Macro*/
#define CALCULATE_POWER(voltage, current) \
      ((uint32_t)(voltage) * (uint32_t)(current))


int main(void)
{
    uint16_t voltage = 800U;
    uint16_t current = 200U;
    uint32_t power;

    power = CALCULATE_POWER(
      voltage,
      current
    );

    printf(
      "EV Charger Power Monitor\n"
      );    
    printf("\n");


    printf(
      "Voltage: %u V\n",
      (unsigned int)voltage
    );
    printf(
      "Current: %u A\n",
      (unsigned int)current
    );
    printf(
      "Power: %u W\n",
      (unsigned int)power
    );


    return 0;
}