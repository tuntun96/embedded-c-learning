#include<stdio.h>
#include<stdint.h>

/*Function declaration*/
uint32_t calculate_power(
  uint16_t voltage,
  uint16_t current
);

int main(void)
{
    uint16_t voltage = 650U;
    uint16_t current = 120U;
    uint32_t power;

    power = calculate_power(voltage, current);

    printf("Power: %lu W\n", (unsigned long)power);

    return 0;
}

/* Function definitaion*/

uint32_t calculate_power(uint16_t voltage, uint16_t current)
{
  uint32_t power;

  power = (uint32_t)voltage * (uint32_t)current;

  return power;
}