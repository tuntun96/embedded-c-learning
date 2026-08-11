#include <stdio.h>

int main(void)
{
    int voltage = 650;
    int *ptr = &voltage;

    printf("Voltage = %d\n", voltage);
    printf("Addres = %p\n",(void *)&voltage);
    printf("Pointer = %p\n",(void *)ptr);
    printf("Value through pointer = %d\n", *ptr);

    return 0;
}