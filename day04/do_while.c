#include <stdio.h>
#include <stdint.h>

int main(void)
{
  uint8_t retry = 1u;
  do
  {
    printf("Server Connection Attempt %u\n", retry);
    retry++;
  } while(retry <= 4);

  printf("Maxium retry atemps completed");

  return 0;
}