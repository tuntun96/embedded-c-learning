#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(void)
{
    uint16_t *charger_data;

    charger_data = malloc(3 * sizeof(uint16_t));

    if ( charger_data == NULL)
    {
      printf("Memory allocation failed\n");

      return 1;
    }

    *(charger_data) = 750U;
    *(charger_data + 1) = 150U;
    *(charger_data + 2) = 65U;

    printf("EV Charger Diagnostic Data\n");
    printf("Voltage     = %u V\n",
      *charger_data);
    printf("Current     = %u A\n",
      *(charger_data + 1));
    printf("Temperature = %u A\n",
      *(charger_data + 2));

    free(charger_data);
    charger_data = NULL;

    return 0;
}