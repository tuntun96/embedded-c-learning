#include <stdio.h>
#include <stdint.h>

/* Debug Configuration */
#define DEBUG_MODE 0

int main(void)
{
    uint16_t voltage = 650U;
    uint16_t current = 120U;

    printf(
      "EV Charger Started\n"
    );

#if DEBUG_MODE

    printf(
      "DEBUG: Voltage = %u V\n",
      (unsigned int)voltage
    );

    printf(
      "DEBUG: Current = %u A\n",
      (unsigned int)current
    );
#endif

    printf(
      "EV Charger Running\n"
    );

    return 0;

}