#include<stdio.h>
#include<stdint.h>
/*Function declaration*/
uint8_t check_temperature(uint8_t temperature);

int main(void)
{
  uint8_t temperature = 75U;
  uint8_t temperature_status;

  temperature_status = check_temperature(temperature);
  printf("EV charger Temperature Monitor\n");
  printf("Temperature: %u\n", temperature);

  if(temperature_status == 0U)
  {    
    printf("Status: Temperature Normal");
  }
  else if(temperature_status == 1U)
  {   
    printf("Status: High Temperature Warning");
  }
  else
  {
    printf("Status: Overtemperature fault");
  }
  return 0;

}

/* Function Definitation*/
uint8_t check_temperature(uint8_t temperature)
{
  if(temperature > 70U) /*Fault*/
  {
    return 2U;
  }
  else if (temperature > 50) /*Warning*/
  {
    return 1U; /*Normal*/
  }
  else 
  {
    return 0U;
  }
}