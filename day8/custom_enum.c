#include<stdio.h>

enum Charger_Fault
{
    FAULT_NONE = 0,
    FAULT_OVER_VOLTAGE = 10,
    FAULT_OVER_CURRENT,
    FAULT_OVER_TEMPERATURE,
    FAULT_COMMUNICATION = 20
};

int main(void)
{
    enum Charger_Fault charger_fault;
    charger_fault = FAULT_OVER_CURRENT;
    printf(
      "EV Charger Fault Code: %d\n",
      charger_fault
    );

    return 0;
}