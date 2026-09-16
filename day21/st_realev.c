#include <stdio.h>
#include <stdint.h>

typedef enum
{
    IDLE,
    VEHICLE_CONNECTED,
    AUTHENTICATING,
    AUTH_FAILED,
    READY,
    CHARGING,
    FAULTED
} ChargerState;

int main(void)
{
    ChargerState state = IDLE;
    uint8_t Charger_state = 1U;
   

  
    while(1)
    {
      switch (state)
      {
          case IDLE:
              printf("Charger is idle.\n");
              if((Charger_state & (1U << 1U)) == 1U)
              {
                  state = VEHICLE_CONNECTED;
              }
              break;
          case VEHICLE_CONNECTED:
              printf("Vehicle connected.\n");
              if((Charger_state & (1U << 2U)) == 1U)
              {
                  state = AUTHENTICATING;
              }
              break;
          case AUTHENTICATING:
              printf("Authenticating...\n");
              if((Charger_state & (1U << 3U)) == 1U)
              {
                  state = READY;                  
              }
              else 
              {
                  state = AUTH_FAILED;
              }
              break;
              
          case AUTH_FAILED:
              printf("Authentication failed.\n");
              state = IDLE; // Reset to IDLE after failure
              break;
          case READY:
              printf("Charger is ready.\n");
              if((Charger_state & (1U << 4U)) == 1U)
              {
                  state = CHARGING;
              }
              break;
          case CHARGING:
              printf("Charging in progress.\n");
              if((Charger_state & (1U << 5U)) == 1U)
              {
                  state = FAULTED;
              }
              else if((Charger_state & (1U << 6U)) == 1U)
              {
                  state = READY; // Return to READY after charging is complete;
              }
              break;
          case FAULTED:
              printf("Charger is faulted.\n");
              break;
          default:
              printf("Unknown state.\n");
              break;
      }
    }

    return 0;
}


