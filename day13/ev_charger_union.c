#include <stdio.h>
#include <stdint.h>

enum datatype
{
  DATA_VOLTAGE,
  DATA_CURRENT,
  DATA_TEMPERATURE,
};

union uniondata
{
  uint16_t voltage;
  uint16_t current;
  uint16_t temperature;
};

struct diagnostic
{
  enum datatype type;
  union uniondata data;
};

void print_diagnostic(struct diagnostic data);

int main(void)
{   
    struct diagnostic diagnostic_data;

    diagnostic_data.type = DATA_VOLTAGE;
    diagnostic_data.data.voltage = 600U;       
    print_diagnostic(diagnostic_data);
    printf("\n");

    diagnostic_data.type = DATA_CURRENT;
    diagnostic_data.data.current = 200U;
    print_diagnostic(diagnostic_data);
    printf("\n");

    diagnostic_data.type = DATA_TEMPERATURE;
    diagnostic_data.data.temperature = 35U;
    print_diagnostic(diagnostic_data);
    printf("\n");    
        

    return 0;

}

void print_diagnostic(struct diagnostic data)
{
  switch (data.type)
    {
    case DATA_VOLTAGE:
      printf("Type : Voltage\n");
      printf( "Value: %u V\n",
      (unsigned int)data.data.voltage
    );
      break;

    case DATA_CURRENT:
      printf("Type : Current\n");
      printf( "Value: %u  A\n",
      (unsigned int)data.data.voltage
    );
      break;
    
    case DATA_TEMPERATURE:
    printf("Type : Temperature\n");
    printf( "Value: %u C\n",
      (unsigned int)data.data.voltage
    );
    break;   
    
    default:
    printf("Invalid Data Type\n");
      break;
    }  

}