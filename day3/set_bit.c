#include<stdio.h>
#include<stdint.h>

int main(void)
{
  uint8_t fault_status = 0U;

  // Set a Bit 1: overcurrent
  fault_status |= (1U << 1U);

  printf("Fault Status = %u\n", fault_status);

  // Set bit 2: Overtemperature
  fault_status |= (1U << 2U);
  printf("Fault Status = %u\n", fault_status);

  return 0;
}