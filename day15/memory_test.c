#include <stdio.h>
#include <stdint.h>

uint16_t global_init = 100U;
uint16_t global_uninit;

static uint16_t static_init = 200U;
static uint16_t static_uninit;

const char messeage[] = "EV Charger";

void test_function(void)
{
    uint16_t local = 50U;
    printf("%u\n",local);
}

int main(void)
{
    test_function();
    return 0;
}
