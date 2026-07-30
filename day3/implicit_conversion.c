#include<stdio.h>
#include<stdint.h>

int main(void)
{
  uint16_t voltage = 400U;
  float voltage_float;

  float charging_current = 125.75f;
  uint16_t current_integer;

  voltage_float = voltage;
  current_integer = charging_current;
  
  printf("Voltage as float = %.1f\n", voltage_float);
  printf("Current as integer= %u\n", current_integer);
  
  return 0;

}