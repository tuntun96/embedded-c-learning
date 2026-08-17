#include<stdio.h>
#include<stdint.h>

/* Function Prototypes */
uint32_t calculate_power(
  uint16_t voltage,
  uint16_t current
);
void display_power(
  uint16_t voltage,
  uint16_t current,
  uint32_t power
);

int main(void)
{
    uint16_t voltage = 650U;
    uint16_t current = 120U;
    uint32_t power;

    power = calculate_power(
      voltage, 
      current
    );

    printf(
      "EV Charger Power Monitor\n"
    );
    printf("\n");

    display_power(
      voltage,
      current,
      power
    );

    return 0;
}

uint32_t calculate_power(
  uint16_t voltage,
  uint16_t current
)
{
    return (uint32_t)voltage * current;
}

void display_power(
  uint16_t voltage,
  uint16_t current,
  uint32_t power
)
{
    printf(
      "Voltage: %u V\n",
      (unsigned int)voltage
    );
    printf(
      "Current: %u A\n",
      (unsigned int)current
    );
    printf(
      "Power: %u W\n",
      (unsigned int)power
    );
  
}
