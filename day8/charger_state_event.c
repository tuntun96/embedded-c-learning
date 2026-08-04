#include<stdio.h>
#include<stdint.h>

typedef enum
{
    CHARGER_IDLE,
    CHARGER_CONNECTED,
    CHARGER_CHARGING,
    CHARGER_COMPLETED,
    CHARGER_FAULT
}Charger_State;

int main(void)
{
  /*event variable*/
    uint8_t vehicle_connected = 1U;
    uint8_t authorization_success = 1U;

    Charger_State charger_state = CHARGER_IDLE;
    printf(
      "EV Charger Event-Based State Machine\n"
    );
    printf("\n");

    printf(
      "Initial State: IDLE\n"
    );
    printf("\n");

  
    if(vehicle_connected == 1U)
    {
        charger_state = CHARGER_CONNECTED;
        printf(
          "Vehicle Connected\n"
        );
        printf(
          "Current State: CONNECTED\n"
        );
        printf("\n");

        if (authorization_success == 1U)
        {
            charger_state = CHARGER_CHARGING;
            printf(
              "Authorization Successful\n"
            );
            printf(
              "Current State: CHARGING\n"
            );
        }
        else
        {
          ptintf(
            "Authorization Failed\n"
          );
          printf(
            "Current State: CONNECTED\n"
          );
          
        };

      

    }
    else
    {
      printf(
        "Vehicle Disconnected\n"
      );

    }
    

    return 0;
}