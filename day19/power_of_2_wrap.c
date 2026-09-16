#include <stdio.h>
#include <stdint.h>

#define BUFFER_SIZE 8U

int main(void)
{
  uint8_t buffer[BUFFER_SIZE] = {0};

  uint8_t head = 6U;

  head = (head + 1U) & (BUFFER_SIZE - 1U);
  
  printf("Head = %u\n", head);

  head = (head + 1U) & (BUFFER_SIZE - 1U);
  
  printf("Head = %u\n", head);

  head = (head + 1U) & (BUFFER_SIZE - 1U);
  
  printf("Head = %u\n", head);

  head = (head + 1U) & (BUFFER_SIZE - 1U);
  
  printf("Head = %u\n", head);

  return 0;
}