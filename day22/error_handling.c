#include <stdio.h>
#include <stdint.h>


typedef enum
{
    STATE_IDLE,
    STATE_CHARGING,
    STATE_FAULTED
} ChargerState;

typedef enum
{
    ERROR_NONE,
    ERROR_TIMEOUT,
    ERROR_INVALID_DATA,
    ERROR_SENSOR,
    ERROR_COMMUNICATION,
    ERROR_OVERVOLTAGE,
    ERROR_OVERTEMPERATURE
}ErrorCode;

/*Limits*/
#define MAX_ATTEMPTS 3U

/* Function Prototype */
ErrorCode read_temperature(uint16_t *temperature, uint8_t sensor_ok);
ErrorCode check_temperature(uint16_t *temperature, uint8_t sensor_ok);
ErrorCode handle_error(ErrorCode error);
ErrorCode read_temperature_with_retry(uint16_t *temperature, uint8_t sensor_ok);
ErrorCode recover_temperature_sensor(uint8_t sensor_ok);
ErrorCode fallback_temperature_sensor(ErrorCode recovery_result);
ErrorCode process_temperature(uint16_t *temperature,
                              uint8_t sensor_ok);

/*Global variable*/
ChargerState state = STATE_CHARGING;
uint8_t inject_sensor_error = 0U;
  
int main(void)
{
  
  uint16_t temperature;
  uint8_t sensor_ok = 0U;
  ErrorCode result;


  result = process_temperature(&temperature, sensor_ok);

  result =   handle_error(result);

  if(result == ERROR_NONE)
  {
    printf("Temperature = %u \n", temperature);
  }   
  
  
  return 0;

}




ErrorCode check_temperature(uint16_t *temperature, uint8_t sensor_ok)
{
  ErrorCode result = read_temperature_with_retry(temperature, sensor_ok);
  
  if( result != ERROR_NONE)
  {
    return result;    
  }
  else if(*temperature > 80U)
  {
    return ERROR_OVERTEMPERATURE;
  }
  else
  {
    return ERROR_NONE;
  }  

}

ErrorCode read_temperature_with_retry(uint16_t *temperature, uint8_t sensor_ok)
{ 
    uint8_t attemt_count = 0U;
   
    for( attemt_count = 1; attemt_count <= MAX_ATTEMPTS; attemt_count++)
    {
        ErrorCode result = read_temperature(temperature, sensor_ok);

      
        if( result  == ERROR_NONE)
        {
          return ERROR_NONE;
        }
        else if ( result != ERROR_SENSOR)
        {
            return result;
        }
        else
        {
          printf("Attempts = %u\n", attemt_count);
          
          if(attemt_count == MAX_ATTEMPTS)
          {
            return result;
          }
        }    

    }
    return ERROR_SENSOR;   
    

}

ErrorCode read_temperature(uint16_t *temperature, uint8_t sensor_ok)
{
  if(inject_sensor_error == 1U)
  {    
    return ERROR_SENSOR;
  }
  else
  {
    *temperature = 45U;
     return ERROR_NONE;
  }

}

ErrorCode handle_error(ErrorCode error)
{
  switch(error)
  {
  case ERROR_NONE:
    return ERROR_NONE;
    
  case ERROR_TIMEOUT:
    printf("Retry communication\n");
    return ERROR_TIMEOUT;
    
  case ERROR_COMMUNICATION:
    printf("Communication recovery\n");
    return ERROR_COMMUNICATION;
    
  case ERROR_SENSOR:
    printf("Sensor recovery\n");
    return ERROR_SENSOR;
    
  case ERROR_INVALID_DATA:
    printf("Reject invalid data\n");
    return ERROR_INVALID_DATA;
      
  case ERROR_OVERVOLTAGE:
    printf("Overvoltage Fault\n");
    return ERROR_OVERVOLTAGE;
    
  case ERROR_OVERTEMPERATURE:
    printf("Overtemperaturer Fault\n");
    return ERROR_OVERTEMPERATURE;
      
  default:
    printf("Unknown Error\n");
    return ERROR_INVALID_DATA;
    
  }

}

ErrorCode recover_temperature_sensor(
    uint8_t sensor_ok
)
{
    if(sensor_ok == 1U)
    {
      return ERROR_NONE;
    }
    else
    {
      return ERROR_SENSOR;
    }
}

ErrorCode fallback_temperature_sensor(ErrorCode recovery_result)
{
  if (recovery_result == ERROR_NONE)
  {
    printf("Sensor recovery successful\n");
    return ERROR_NONE;
  }
  else
  {
      printf("Sensor recovery failed\n");
      state = STATE_FAULTED;
      return ERROR_SENSOR;

  }    

}

ErrorCode process_temperature(uint16_t *temperature, uint8_t sensor_ok)
{
   ErrorCode result = check_temperature(temperature, sensor_ok);
  
  if(result == ERROR_SENSOR)
  {
    ErrorCode recovery_result;
    

    recovery_result = recover_temperature_sensor(sensor_ok);

    result = fallback_temperature_sensor(recovery_result);  
  }

  return result;
}

