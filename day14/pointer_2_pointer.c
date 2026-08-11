
#include <stdio.h>
#include <stdint.h>

int main(void)
{
    uint16_t voltage = 650U;
    uint16_t *ptr = &voltage;
    uint16_t **pptr = &ptr;

    printf("Voltage = %u V\n", voltage);
    printf("Voltage = %u V\n", *ptr);
    printf("Voltage = %u V\n", **pptr);

    printf("Addres of Voltage = %p\n",(void *)&voltage);
    printf("Addres of ptr = %p\n",(void *)&ptr);
    printf("Addres of pptr = %p\n",(void *)*pptr);

    **pptr = 700U;
    printf("Updated Voltage = %u V\n", voltage);

    return 0;

}