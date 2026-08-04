
#include "charger.h"

#include <stdio.h>

/* 
Function: Charger_init
purpose : Initializes the EV charger
*/
void charger_init(void)
{
  printf(
    "EV Charger Initialized\n"
  );
}

/*
Function:read_voltage
purpose :Returns charger voltage
*/
uint16_t read_voltage(void)
{
  return 950U;
}

/*
Function: calculate_power
Purpose :Calculate charger power
*/
uint32_t calculate_power(
  uint16_t voltage,
  uint16_t current
)
{
  return (uint32_t)voltage * current;
}

/*
Function: check_over_voltage
Purpose : Check for an Over Voltage conditon
*/

uint8_t check_over_voltage(
  uint16_t voltage
)
{
    if (voltage > 900U)
    {
        return 1U;
    }
    return 0U;
}

void update_voltage(
  uint16_t *voltage,
   uint16_t new_voltage
)
{
    *voltage = new_voltage;
}