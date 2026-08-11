#include <stdio.h>

#include "charger.h"

int main(void)
{
    printf(
      "EV Charger Started\n"
    );

    charger_voltage = 720U;

    display_charger_status();
    printf(
      "Voltage form main(): %u V\n",
      (unsigned int)charger_voltage
    );

    return 0;
}