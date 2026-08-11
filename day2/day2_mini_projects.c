#include<stdio.h>
#include<stdint.h>

int main(void)
{
  uint16_t voltage = 600;
  uint16_t current = 150;
  uint8_t soc = 56;
  int8_t temperature = -5;
  uint32_t energy = 132444;

  printf("Charger parameter\n");
  printf("-----------------\n");

  printf("Charger Voltage : %u V\n", voltage);
  printf("Charger Current : %u A \n", current);
  printf("SOC : %u %%\n", soc);
  printf("Temperature : %d C\n", temperature);
  printf("Energy : %u Wh \n", energy);

  printf("Size of variables\n");
  printf("-----------------\n");

  printf("Size of Voltage : %zu bytes\n", sizeof(voltage));
  printf("Size of Current : %zu bytes\n", sizeof(current));
  printf("Size of SOC : %zu bytes\n", sizeof(soc));
  printf("Size of Temperature : %zu bytes\n", sizeof(temperature));
  printf("Size of Energy : %zu bytes\n", sizeof(energy));

  return 0;
}