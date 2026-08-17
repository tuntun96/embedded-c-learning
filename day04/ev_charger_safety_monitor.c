#include<stdio.h>
#include<stdint.h>

uint8_t check_temperature(uint8_t temperature);
uint8_t is_charging_allowed(uint8_t fault_status,uint8_t temperature_status);
void display_charger_status(uint8_t temperature_status,uint8_t charging_allowed);

int main(void)
{
    uint8_t temperature = 40U;
    uint8_t fault_status = 1U;

    uint8_t temperature_status;
    uint8_t charging_aallowd;
    temperature_status = check_temperature(temperature);
    charging_aallowd = is_charging_allowed(fault_status,temperature_status);

    printf("EV Charger Safety Monitor\n");
    printf("Temperature: %u C\n", temperature);

    display_charger_status(temperature_status,charging_aallowd);
    return 0;

    
  
}

/*Function Definition*/

uint8_t check_temperature(uint8_t temperature)
{
  if(temperature > 70U)
  {
    return 2U;
  }
  else if (temperature > 50U)
  {
    return 1U;
  }
  else
  {
    return 0U;
  }
}

/*Charging allow*/
uint8_t is_charging_allowed(
uint8_t fault_status,
uint8_t temperature_status
)
{
  if(fault_status == 0U && temperature_status != 2U)
  {
    return 1U;
  }
  else
  {
    return 0U;
  }
}

void display_charger_status(
    uint8_t temperature_status,
    uint8_t charging_allowed
)
{
    if (temperature_status == 0U)
    {
        printf("Temperature Status: Normal\n");
    }
    else if (temperature_status == 1U)
    {
        printf("Temperature Status: Warning\n");
    }
    else
    {
        printf("Temperature Status: Fault\n");
    }

    if (charging_allowed == 1U)
    {
        printf("Charging Status: Allowed\n");
    }
    else
    {
        printf("Charging Status: Blocked\n");
    }
}