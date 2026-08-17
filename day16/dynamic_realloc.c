#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(void)
{
    uint16_t *data;
    uint16_t *temp;

    data = malloc(3 * sizeof(uint16_t));

    *data = 750U;
    *(data + 1) = 150U;
    *(data + 2) = 65U;

    printf("EV Charger Diagnostic Data\n");
    printf("\n");

    printf("Voltage     = %u V\n", *data);
    printf("Current     = %u A\n", *(data + 1));
    printf("Temperature = %u C\n", *(data + 2));
    printf("\n");

   

    temp = realloc(data, 6 * sizeof(uint16_t));

    if (temp == NULL)
    {
      printf("Memory allocation failed\n");
       
      free(data);
      data = NULL;

      return 1;
    }
    
    data = temp;

    *(data + 4) = 750U;
    *(data + 5) = 50U;
    *(data + 6) = 250U;

    printf("Resizing Buffer\n");
    printf("\n");
    printf("Voltage     = %u V\n", *data);
    printf("Current     = %u A\n", *(data + 1));
    printf("Temperature = %u C\n", *(data + 2));
    printf("Power       = %u kW\n", *(data + 4));
    printf("Frequency   = %u Hz\n", *(data + 5));
    printf("Energy      = %u kWh\n", *(data + 6));

    free(data);
    data = NULL;

    return 0;
    
}