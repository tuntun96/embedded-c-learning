#include<stdio.h>
#include<stdint.h>

/* Function prototype*/
uint8_t check_over_voltage(uint16_t voltage);
void display_fault_status(uint8_t fault_status);

#define OVER_VOLTAGE_LIMIT 900U

int main(void)
{   
    uint16_t charger_voltage = 950U;
    uint8_t fault_status;

    printf(
      "EV Charger Fault Monitor\n"
    );
    printf("\n");

    printf(
      "Charger Voltage: %u V\n",
      (unsigned int)charger_voltage
    );

    fault_status = check_over_voltage(charger_voltage);
    display_fault_status(fault_status);

    return 0;
}

uint8_t check_over_voltage(uint16_t charger_voltage)
{
    if(charger_voltage > OVER_VOLTAGE_LIMIT)
    {
      return 1U;
    }  
          
    return 0U;    

}

void display_fault_status(uint8_t fault_status)
{
    if(fault_status == 1U)
    {
      printf(
        "Fault Status: OVER-VOLTAGE-FAULT\n"
      );
    }
    else{
      printf(
        "Fault Status: NORMAL\n"
      );
    }

}


