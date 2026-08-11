#include <stdio.h>
#include <stdint.h>

void update_current(uint16_t *cur_pinter);
    

int main(void)
{
    uint16_t current = 100U;

    
    printf("Before Update: %u\n", current);

    update_current(&current);
    printf("After Update: %u\n", current);
    

    return 0;
}

void update_current(uint16_t *cur_pinter)
{
    *cur_pinter = 120U;
}

