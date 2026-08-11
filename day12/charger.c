#include <stdio.h>

#include "charger.h"

/* Global variable defination */
uint16_t charger_voltage = 650U;

void display_charger_status(void)
{
  printf(
    "Charger Voltage: %u V\n",
    (unsigned int)charger_voltage
  );
}