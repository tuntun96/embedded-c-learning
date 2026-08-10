#include <stdio.h>
#include <stdint.h>

enum type
{
    DATA_VOLTAGE,
    DATA_CURRENT,
    DATA_TEMPERATURE,
    DATA_FAULT,
};

union value
{
    uint16_t voltage;
    uint16_t current;
    uint8_t temperature;
    uint8_t fault;
};

struct diagnostic
{
    enum type dtype;
    union value dvalue;
};

void print_diagnostic(struct diagnostic data);

int main(void)
{
    struct diagnostic data_diagnostic;
    
    
    printf("EV Charger Diagnostic Monitor\n");
    printf("\n");

    data_diagnostic.dtype = DATA_VOLTAGE;
    data_diagnostic.dvalue.voltage = 750U;
    print_diagnostic(data_diagnostic);
    printf("\n");

    data_diagnostic.dtype = DATA_CURRENT;
    data_diagnostic.dvalue.current = 150U;
    print_diagnostic(data_diagnostic);
    printf("\n");

    data_diagnostic.dtype = DATA_TEMPERATURE;
    data_diagnostic.dvalue.temperature = 65U;
    print_diagnostic(data_diagnostic);
    printf("\n");

    data_diagnostic.dtype = DATA_FAULT;
    print_diagnostic(data_diagnostic);
    

    return 0;

}

void print_diagnostic(struct diagnostic data)
{
    switch (data.dtype)
    {
        case DATA_VOLTAGE:
          printf("Type: Voltage\n");
          printf("Value: %u V\n",
            data.dvalue.voltage);       
          break;

        case DATA_CURRENT:
          printf("Type: Current\n");
          printf("Value: %u A\n",
            data.dvalue.current);       
          break;
        
        case DATA_TEMPERATURE:
          printf("Type: Temperature\n");
          printf("Value: %u C\n",
            data.dvalue.temperature);       
          break;
        
        case DATA_FAULT:
        printf("Type: FAULT\n");
        printf("Value: Invalid diagnostic data\n"
        );       
        break;
  
        default:
        printf("Invalid Diagnostic State\n");
          break;
    }
}


