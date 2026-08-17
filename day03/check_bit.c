#include<stdint.h>
#include<stdio.h>

#define FAULT_OVERTEMPERATURE (1U << 2U)

int main(void)
{
  uint8_t fault_status = 00000110;
 
  if ((fault_status & FAULT_OVERTEMPERATURE) != 0U)
  {
    printf("Overtemperature fault active\n");
  }

  return 0;


}

