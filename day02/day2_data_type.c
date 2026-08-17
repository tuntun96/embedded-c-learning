#include <stdio.h>
#include <stdint.h>

int main(void)
{
   uint16_t soc = 80;
   uint16_t voltage = 750;
   uint16_t current = 200;
   int16_t temperature = -10;
   uint32_t energy = 125000;
   printf("SOC : %u %%\n", soc);
   printf("Voltage : %u V\n", voltage);
   printf("Current : %u A\n", current);
   printf("Temperature :%d C\n", temperature);
   printf("Energy : %u Wh\n", energy);
   printf("\nMemory size :\n");
   printf("uint8_t : %zu byte\n", sizeof(uint8_t));
   printf("uint16_t : %zu byte\n", sizeof(uint16_t));
   printf("uint32_t : %zu byte\n", sizeof(uint32_t));

   
   printf("Task -1");
   
   uint8_t value = 250;
   value = value + 10;
   printf("%u\n", value);

   return 0;

}