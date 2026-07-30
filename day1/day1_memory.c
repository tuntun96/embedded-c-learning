#include <stdio.h>

int global_voltage = 400;

void check_current(void)
{
    int current = 150;

    printf("Address of current = %p\n",
           (void *)&current);
}

int main(void)
{
    printf("Address of global_voltage = %p\n",
           (void *)&global_voltage);

    check_current();

    return 0;
}