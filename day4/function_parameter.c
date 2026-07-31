#include<stdio.h>
#include<stdint.h>

void display_charger_parameter(
  uint16_t voltage,
  uint16_t current,
  uint8_t soc
);

int main(void)
{
  display_charger_parameter(650U, 120U, 75U);
  return 0;
}

/* Function definition*/
void display_charger_parameter(
  uint16_t voltage,
  uint16_t current,
  uint8_t soc
)
{
  printf("Charger Parameter\n");
  printf("Voltage: %u V\n", voltage);
  printf("Current: %u A\n", current);
  printf("Battery SOC: %u%%\n", soc);
}
