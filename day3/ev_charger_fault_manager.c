#include <stdio.h>
#include <stdint.h>

/* Fault bit definitions */
#define FAULT_OVERVOLTAGE     (1U << 0U)
#define FAULT_OVERCURRENT     (1U << 1U)
#define FAULT_OVERTEMPERATURE (1U << 2U)
#define FAULT_ESTOP           (1U << 3U)

int main(void)
{
uint8_t fault_status = 0U;


printf("Initial fault status: %u\n", fault_status);

/* Overcurrent fault occurs */
fault_status |= FAULT_OVERCURRENT;
printf("After overcurrent fault: %u\n", fault_status);

/* Overtemperature fault occurs */
fault_status |= FAULT_OVERTEMPERATURE;
printf("After overtemperature fault: %u\n", fault_status);

/* Check overcurrent fault */
if ((fault_status & FAULT_OVERCURRENT) != 0U)
{
    printf("Overcurrent fault is ACTIVE\n");
}

/* Overcurrent fault is resolved */
fault_status &= ~FAULT_OVERCURRENT;
printf("After clearing overcurrent: %u\n", fault_status);

/* Check E-stop fault */
if ((fault_status & FAULT_ESTOP) != 0U)
{
    printf("E-stop fault is ACTIVE\n");
}
else
{
    printf("E-stop fault is NOT active\n");
}

return 0;


}
