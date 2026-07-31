#include<stdio.h>
#include<stdint.h>

int main(void)
{
    uint8_t charger_state = 8U;
    switch (charger_state)
    {
    case 0U:
      printf("Power ON\n");
      break;

    case 1U:
      printf("Self-test\n");
      break;
          
    case 2U:
      printf("Charger ready\n");
      break;

    case 3U:
      printf("Charging in progress\n");
      break;

    case 4U:
      printf("Charging complete\n");
      break;

    case 5U:
      printf("Fault active\n");
      break;
      
    default:
      printf("Invalid charger state");
      break;
    }

  return 0;
}