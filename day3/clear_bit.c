#include<stdint.h>
#include<stdio.h>

int main(void)
{
    uint8_t fault_status = 0x0FU;
    printf("Initial status = %u\n", fault_status);

    //clear bit 2
    fault_status &= ~(1U << 2U);
    
    printf("After cearing Bit 2 = %u\n", fault_status);


   return 0;

}