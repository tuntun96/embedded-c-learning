#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(void)
{
    uint16_t *charger_data;
    
    charger_data = calloc(3, sizeof(uint16_t));

    if (charger_data == NULL)
    {
      printf("Allocation is Failed\n");

      return 1;
    }

    printf("EV Charger Diagnostic Data\n");
    printf("\n");

    printf("Inital Data\n");
    printf("Voltage     = %u V\n",
      *(charger_data));
    printf("Current     = %u A\n",
      *(charger_data + 1));
    printf("Temperature = %u C\n",
      *(charger_data + 2));
    printf("\n");

    *(charger_data) = 750U;
    *(charger_data + 1) = 150U;
    *(charger_data + 2) = 65U;

    printf("After Update Data\n");  
    printf("Voltage     = %u V\n",
      *(charger_data));
    printf("Current     = %u A\n",
      *(charger_data + 1));
    printf("Temperature = %u C\n",
      *(charger_data + 2));

    free(charger_data);
    charger_data = NULL;

    return 0;
    
}