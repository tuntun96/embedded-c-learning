#include<stdio.h>
#include<stdint.h>

struct Charger_Status
{
    uint16_t voltage;
    uint16_t current;
    uint8_t temperature;
    uint8_t fault_status;
};

int main(void)
{
    struct Charger_Status charger =
    {
      650U,
      120U,
      45U,
      0U
    };

    struct Charger_Status *charger_pointer;
    charger_pointer = &charger;

    printf(
      "EV Charger Status Through Structure Pointer\n"
    );
    printf("\n");

    printf(
      "Voltage: %u V\n",
      (unsigned int)charger_pointer->voltage
    );
    printf(
      "Current: %u A\n",
      (unsigned int)charger_pointer->current
    );
    printf(
      "Temperature: %u C\n",
      (unsigned int)charger_pointer->temperature
    );
    printf(
      "Fault Status: %u\n",
      (unsigned int)charger_pointer->fault_status
    );

    return 0;
}