#include <stdint.h>
#include <stdio.h>

#define CHARGING_MASK       (1U << 0U)
#define FAULT_MASK          (1U << 1U)
#define OVER_TEMP_MASK      (1U << 2U)
#define COMM_ERROR_MASK     (1U << 3U)

int main(void)
{
    uint8_t status_reg = 0U;

    status_reg = 0X0DU;

    if(status_reg & CHARGING_MASK)
    {
      printf("Charging            = ON\n");
    }
    else
    {
      printf("Charging            = OFF\n");
    }
    
    if(status_reg & FAULT_MASK)
    {
      printf("Fault               = ON\n");
    }
    else
    {
      printf("Fault               = OFF\n");
    }

    if(status_reg & OVER_TEMP_MASK)
    {
      printf("Over Tempearture    = ON\n");
    }
    else
    {
      printf("Over Tempearture    = OFF\n");
    }
    

    if(status_reg & COMM_ERROR_MASK)
    {
      printf("Communication Error = ON\n");
    }
    else
    {
      printf("Communication Error = OFF\n");
    }


    return 0;

}