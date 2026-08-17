#include <stdio.h>
#include <stdint.h>

/* Function declaration */
uint8_t is_charging_allowed(uint8_t fault_status);

int main(void)
{
    uint8_t fault_status = 0U;
    uint8_t charging_allowed;

    /* Check whether charging is allowed */
    charging_allowed = is_charging_allowed(fault_status);

    /* Display charging status */
    if (charging_allowed == 1U)
    {
        printf("Charging allowed\n");
    }
    else
    {
        printf("Charging blocked due to fault\n");
    }

    return 0;
}

/* Function definition */
uint8_t is_charging_allowed(uint8_t fault_status)
{
    if (fault_status == 0U)
    {
        return 1U;
    }
    else
    {
        return 0U;
    }
}