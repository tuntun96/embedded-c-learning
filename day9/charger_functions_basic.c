#include<stdio.h>
#include<stdint.h>

/* Function Prototype */
void charger_init(void);
uint16_t read_voltage(void);
void display_voltage(uint16_t voltage);

int main(void)
{
    uint16_t voltage;
    charger_init();
    printf("\n");
    voltage = read_voltage();
   
    display_voltage(voltage);

    return 0;
}

void charger_init(void)
{
  printf(
    "EV Charger Initialized\n"
  );
}


uint16_t read_voltage(void)
{
  return 650U;
}

void display_voltage(uint16_t voltage)
{
  printf(
    "Charging Voltage: %u V\n", voltage
  );
}