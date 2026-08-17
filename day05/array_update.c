#include<stdio.h>
#include<stdint.h>

int main(void)
{
    uint8_t index;
    uint16_t voltage_samples[5]=
    { 
        650U,
        655U,
        648U,
        652U,
        649U

    };
    printf(
      "EV Charger Voltage Update\n"
    );

    printf(
      "Before Update:\n"
    );

    for(index = 0; index < 5U; index++)
    {
      printf("Sample %u: %u V\n", index, voltage_samples[index]);
    }
    printf("\n");

    voltage_samples[2] = 660U;
    printf(
      "After Update:\n"
    );
    
    for(index = 0; index < 5U; index++)
    {
      printf("Sample %u: %u V\n", index, voltage_samples[index]);
    }

    return 0;
}
