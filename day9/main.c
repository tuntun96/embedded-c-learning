#include <stdio.h>
#include <stdint.h>


#include "charger.h"

int main(void)
{
    uint16_t voltage;
    uint16_t current = 120U;
    uint32_t power;
    uint8_t fault_status;

    printf(
      "EV Charger Multi-File Project\n"
    );
    printf("\n");

    charger_init();
    
    voltage = read_voltage();

    update_voltage(
      &voltage,
      700U
    );

    fault_status = check_over_voltage(
      voltage
    );
    
    power = calculate_power(
      voltage,
      current
    );

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

    if (fault_status == 1U)
    {
        printf(
          "Fault Status: OVER-VOLTAGE\n"
        );
    }
    else
    {
        printf(
          "Fault Status: NORMAL\n"
        );

    }

    return 0;

}