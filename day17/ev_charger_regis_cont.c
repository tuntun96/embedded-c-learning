#include <stdio.h>
#include <stdint.h>

#define CHARGER_ENABLE_MASK (1U << 0U)
#define GUN1_ENABLE_MASK (1U << 1U)
#define GUN2_ENABLE_MASK (1U << 2U)
#define FAN_ENABLE_MASK (1U << 3U)

int main(void)
{
    uint8_t control_reg = 0U;

    /* Charger on bit */

    control_reg |= CHARGER_ENABLE_MASK;

    /* Enable gun 1 charging */

    control_reg |= GUN1_ENABLE_MASK;

    /* Enable fan*/

    control_reg |= FAN_ENABLE_MASK;

    printf("Register Value = %u \n", control_reg);

    /* Desible Gun 1 */
    
    control_reg &= ~GUN1_ENABLE_MASK;

    /*Enable Gun 2*/

    control_reg |= GUN2_ENABLE_MASK;

    /* Toggle Fan*/

    control_reg ^= FAN_ENABLE_MASK;

    printf("Final Register Value = %u\n", control_reg);

    if( control_reg & CHARGER_ENABLE_MASK)
    {
      printf("Charger = ON\n");
    }
    else
    {
      printf("Charger = OFF\n");
    }

    if(control_reg & GUN1_ENABLE_MASK)
    {
      printf("Gun 1   = ON\n");
    }
    else
    {
      printf("Gun 1   = OFF\n");
    }

    if(control_reg & GUN2_ENABLE_MASK)
    {
      printf("Gun 2   = ON\n");
    }
    else
    {
      printf("Gun 2   = OFF\n");
    }

    if(control_reg & FAN_ENABLE_MASK)
    {
      printf("Fan     = ON\n");
    }
    else
    {
      printf("Fan     = OFF\n");
    }

    return 0;
}