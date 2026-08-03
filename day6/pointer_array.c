#include<stdio.h>
#include<stdint.h>

int main(void)
{
    uint16_t voltage_samples[5] = 
    { 
        650U, 
        660U, 
        670U, 
        680U, 
        690U};
    
    uint16_t *voltage_pointer;
    voltage_pointer = voltage_samples;

    printf(
      "EV Charger Voltage Buffer\n"
    );

    printf(
      "First Voltage: %u V\n",
      (unsigned int)*voltage_pointer
    );

    printf(
      "Third Voltage: %u V\n",
      (unsigned int)*(voltage_pointer + 2)
    );
    

    return 0;
}