#include<stdio.h>
#include<stdint.h>


int main(void)
{
  uint8_t sample_count;
  uint16_t voltage_samples[] =
{
    650U,
    655U,
    648U,
    652U,
    649U
};


sample_count = 
sizeof(voltage_samples) /
sizeof(voltage_samples[0]);

printf("EV Charger Array Size\n");

printf(
  "Total Array Size: %zu Bytes\n", 
  sizeof(voltage_samples));

printf(
  "Size of one Element: %zu Bytes\n", 
  sizeof(voltage_samples[0]));


printf(
  "Number of samples: %zu\n", 
  sample_count);

return 0;
}