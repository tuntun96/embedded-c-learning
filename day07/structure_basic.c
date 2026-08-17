#include<stdio.h>
#include<stdint.h>

struct Charger_status
{
    uint16_t voltage;
    uint16_t current;
    uint8_t temperature;
    uint8_t fault_status;

};

int main(void)
{
  struct Charger_status charger;

  charger.voltage = 650U;
  charger.current = 120U;
  charger.temperature = 45U;
  charger.fault_status = 0U;

  printf(
    "EV Charger Status\n"
  );
  printf(
    "Voltage: %u V\n", (unsigned int)charger.voltage
  );

  printf(
    "Current: %u A\n", (unsigned int)charger.current
  );

  printf(
    "Temperature: %u C\n", (unsigned int)charger.temperature
  );

  printf(
    "Fault Status: %u\n", (unsigned int)charger.fault_status
  );

  return 0;
}