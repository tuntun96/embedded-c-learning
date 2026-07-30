#include<stdint.h>
#include<stdio.h>

int main(void)
{
  uint16_t energy_wh = 1500U;
  float energy_kWh;

  energy_kWh = (float)energy_wh / 1000.0f;
  printf("Energy = %.2f kWh\n", energy_kWh);

  return 0;
}