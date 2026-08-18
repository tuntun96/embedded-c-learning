#include <stdio.h>
#include <stdint.h>

/* Control Bit MASK */
#define CHARGER_ENABLE_MASK (1U << 0U)
#define GUN1_ENABLE_MASK (1U << 1U)
#define GUN2_ENABLE_MASK (1U << 2U)
#define FAN_ENABLE_MASK (1U << 3U)

/* Status Bit MASK */
#define CHARGING_STATUS_MASK (1U << 0U)
#define FAULT_STATUS_MASK (1U << 1U)
#define OVER_TEMPERATURE_MASK (1U << 2U)
#define COMM_ERROR_MASK (1U << 3U)

int main(void)
{
  uint8_t control_reg = 0U;
  uint8_t status_reg = 0U;

  /*Control Register */
  control_reg |= CHARGER_ENABLE_MASK;
  control_reg |= GUN1_ENABLE_MASK;
  control_reg &= ~GUN2_ENABLE_MASK;
  control_reg |= FAN_ENABLE_MASK;

  /* Status Register */
  status_reg |= CHARGING_STATUS_MASK;
  status_reg &= ~FAULT_STATUS_MASK;
  status_reg &= ~OVER_TEMPERATURE_MASK;
  status_reg &= ~COMM_ERROR_MASK;

  
  if ((control_reg & CHARGER_ENABLE_MASK) != 0U)
  {
    printf("Charger Enable     = ON\n");
  }
  else
  {
    printf("Charger Enable     = OFF\n");
  }

  if ((control_reg & GUN1_ENABLE_MASK) != 0U)
  {
    printf("Gun 1 Enable       = ON\n");
  }

  else
  {
    printf("Gun 1 Enable       = OFF\n");
  }

  if ((status_reg & CHARGING_STATUS_MASK) != 0U)
  {
    printf("Charging Status    = ON\n");
  }
  else
  {
    printf("Charging Status    = OFF\n");  
  }

  if ((status_reg & FAULT_STATUS_MASK) != 0U)
  {
    printf("Fault              = ON\n");
  }
  else
  {
    printf("Fault              = OFF\n");
  }

  if ((status_reg & OVER_TEMPERATURE_MASK) != 0U)
  {
    printf("Over Temperature   = ON\n");
  }

  else
  {
    printf("Over Temperature   = OFF\n");    
  }

  if ((status_reg & COMM_ERROR_MASK) != 0U )
  {
    printf("Communication      = Not OK\n");
  }
  else
  {
    printf("Communication      = OK\n");
  }

  if((control_reg & CHARGER_ENABLE_MASK) && 
     (control_reg & GUN1_ENABLE_MASK) && 
     (status_reg & CHARGING_STATUS_MASK) &&
     !(status_reg & FAULT_STATUS_MASK) && 
     !(status_reg & OVER_TEMPERATURE_MASK) &&
     !(status_reg & COMM_ERROR_MASK)
    )
  {
    printf("Charging Operation = Allowed\n");
  }
  else
  {
    printf("Charging Operation = Not Allowed\n ");
  }

  return 0;
  
}

