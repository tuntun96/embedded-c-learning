#include<stdio.h>
#include<stdint.h>

int main(void)
{
    uint16_t voltage = 650U;
    uint16_t *voltage_pointer;
    voltage_pointer = &voltage;

    printf(
      "Voltage Value: %u V\n",
      (unsigned int)voltage
    );

    printf(
      "Voltage address: %p\n",
      (void *)&voltage
    );
    
    printf(
      "Address stored in pointer: %p\n",
      (void *)voltage_pointer
    );

    printf(
    "Voltage through pointer: %u V\n",
    (unsigned int)*voltage_pointer
);

    return 0;



}