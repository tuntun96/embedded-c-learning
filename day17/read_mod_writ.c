#include <stdio.h>
#include <stdint.h>

/* Bit MASK */
#define CHARGER_ENABLE_MASK (1U << 0U)
#define GUN1_ENABLE_MASK (1U << 1U)
#define GUN2_ENABLE_MASK (1U << 2U)
#define FAN_ENABLE_MASK (1U << 3U)

int main(void)
{
    uint8_t control_reg = 0x0BU;

    printf("Initial Register = %u\n", control_reg);

    /* Enable Gun 2 bit 2 */
    control_reg |= GUN2_ENABLE_MASK;

    printf("After Gun 2 Enable = %u\n", control_reg);

    /* Disable Gun 1 bit 1 */
    control_reg &= ~GUN1_ENABLE_MASK;

    printf("After Gun 1 Disable = %u\n",control_reg);

    /* Togal Fan bit 3 */
    control_reg ^= FAN_ENABLE_MASK;

    if((control_reg & CHARGER_ENABLE_MASK) != 0U)
    {
      printf("Charger = ON\n");
    }
    else
    {
      printf("Charger = OFF\n");
    }

    if((control_reg & GUN1_ENABLE_MASK) != 0U)
    {
      printf("Gun 1   = ON\n");
    }
    else
    {
      printf("Gun 1   = OFF\n");
    }
    
    if((control_reg & GUN2_ENABLE_MASK) != 0U)
    {
      printf("Gun 2   = ON\n");
    }
    else
    {
      printf("Gun 2   = OFF\n");
    }

    if((control_reg & FAN_ENABLE_MASK) != 0U)
    {
      printf("Fan     = ON\n");
    }
    else
    {
      printf("Fan     = OFF\n");
    }


    return 0;

}