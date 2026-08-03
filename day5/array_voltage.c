#include <stdio.h>
#include <stdint.h>

int main(void)
{
    uint16_t voltage[5] =  {650U, 655U, 648U, 652U, 649U
                            };
    uint8_t index;

    printf("EV charger Voltage Sample\n");

    for(index = 0; index < 5; index++)
    {
      printf("Sample %u:%u V\n", index, voltage[index]);
    }

    return 0;

}