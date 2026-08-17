#include<stdio.h>

typedef enum
{
    CHARGER_IDLE,
    CHARGER_CONNECTED,
    CHARGER_CHARGING,
    CHARGER_COMPLETE,
    CHARGER_FAULT,
} Charger_State;

int main(void)
{

    Charger_State charger_state = CHARGER_CHARGING;

    switch (charger_state)
    {
    case CHARGER_IDLE:
      printf(
        "EV Charger State: IDLE\n"
      );
      break;

    case CHARGER_CONNECTED:
      printf(
        "EV Charger State: CONNECTED\n"
      );
      break;

    case CHARGER_CHARGING:
      printf(
        "EV Charger State: CHARGING\n"
      );
      break;

    case CHARGER_COMPLETE:
      printf(
        "EV Charger State: COMPLETE\n"
      );
      break;

    case CHARGER_FAULT:
      printf(
        "EV Charger State: FAULT\n"
      );
      break;
    
    default:
      printf(
        "EV Charger State: UNKNOWN\n"
      );
      break;
    }

    return 0;

}
