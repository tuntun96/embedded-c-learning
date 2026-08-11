#include <stdio.h>
#include <stdint.h>

struct charger
{
    uint16_t voltage;
    uint16_t current;
    uint16_t temperature;
};

void update_charger(struct charger *ptr);

int main(void)
{
    struct charger data;

    data.voltage = 650U;
    data.current = 120;
    data.temperature = 45;
    
    printf("Before Update:\n");
    printf("Voltage = %u V\n",data.voltage);
    printf("Current = %u A\n",data.current);
    printf("Temperature = %u C\n",data.temperature);
    printf("\n");

    update_charger(&data);

    printf("After Update:\n");
    printf("Voltage = %u V\n",data.voltage);
    printf("Current = %u A\n",data.current);
    printf("Temperature = %u C\n",data.temperature);
    printf("\n");

}
void update_charger(struct charger *ptr)
{
    ptr->voltage = 750U;
    ptr->current = 150U;
}