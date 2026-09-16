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

typedef enum
{
    AUTH_REASON_NONE,
    AUTH_REASON_REJECTED,
    AUTH_REASON_INVALID,
    AUTH_REASON_TIMEOUT
    
} AuthFailureReason;



// event bit mask

#define VEHICLE_CONNECTED_MASK (1U << 1U)
#define AUTH_REQUESTED_MASK (1U << 2U)
#define AUTH_ACCEPTED_MASK (1U << 3U)
#define AUTH_REJECTED_MASK (1U << 4U)
#define AUTH_INVALID_MASK (1U << 5U)
#define AUTH_TIMEOUT_MASK (1U << 6U)
#define START_REQUESTED_MASK (1U << 7U)
#define CHARGING_COMPLETE_MASK (1U << 8U)
#define FAULT_DETECTED_MASK (1U << 9U)
#define RETRY_AUTH_MASK (1U << 10U)
#define VEHICLE_DISCONNECTED_MASK (1U << 11U)

/*Function prototype*/
void handle_charging_state(void);
void charging_entry(void);
void charging_exit(void);
void ready_entry(void);
void ready_exit(void);

int main(void)
{
    ChargerState state = IDLE;
    uint16_t charger_event_register =
    VEHICLE_CONNECTED_MASK |
    AUTH_REQUESTED_MASK |
    AUTH_ACCEPTED_MASK |
    START_REQUESTED_MASK;

    ChargerState previous_state = IDLE;
    

    AuthFailureReason auth_failure_reason = AUTH_REASON_NONE;
    while(1)
    {
      switch (state)
      {
          case IDLE:
              printf("Charger is idle.\n");
              if((charger_event_register & VEHICLE_CONNECTED_MASK) != 0U)
              {
                  state = VEHICLE_CONNECTED;
                  charger_event_register &= ~VEHICLE_CONNECTED_MASK;
              }
              break;
          case VEHICLE_CONNECTED:
              printf("Vehicle connected.\n");
              if((charger_event_register & AUTH_REQUESTED_MASK) != 0U)
              {
                  state = AUTHENTICATING;
                  charger_event_register &= ~AUTH_REQUESTED_MASK;
              }
              break;
          case AUTHENTICATING:
              printf("Authenticating...\n");
              if(((charger_event_register & AUTH_ACCEPTED_MASK) != 0U) && 
              ((charger_event_register & AUTH_REJECTED_MASK) != 0U))
              {
                  state = FAULTED;
                  charger_event_register &= ~(AUTH_ACCEPTED_MASK | AUTH_REJECTED_MASK);
              }
              else if((charger_event_register & AUTH_ACCEPTED_MASK) != 0U)
              {
                  state = READY;
                  auth_failure_reason = AUTH_REASON_NONE;
                  charger_event_register &= ~AUTH_ACCEPTED_MASK;
                                    
              }
              else if((charger_event_register & AUTH_REJECTED_MASK) != 0U)
              {
                  state = AUTH_FAILED;
                  auth_failure_reason = AUTH_REASON_REJECTED;
                  charger_event_register &= ~AUTH_REJECTED_MASK;
                  
              }
              else if((charger_event_register & AUTH_INVALID_MASK) != 0U)
              {
                  state = AUTH_FAILED;
                  auth_failure_reason = AUTH_REASON_INVALID;
                  charger_event_register &= ~AUTH_INVALID_MASK;
                  
              }
              else if((charger_event_register & AUTH_TIMEOUT_MASK) != 0U)
              {
                  state = AUTH_FAILED;
                   auth_failure_reason = AUTH_REASON_TIMEOUT;
                  charger_event_register &= ~AUTH_TIMEOUT_MASK;
                 
              }
              
              break;
              
          case AUTH_FAILED:
              printf("Authentication failed.\n");
              if((charger_event_register & RETRY_AUTH_MASK) != 0U)
              {
                  state = AUTHENTICATING; // Retry authentication
                  auth_failure_reason = AUTH_REASON_NONE;
                  charger_event_register &= ~RETRY_AUTH_MASK; // Clear the retry flag
              }
              else if((charger_event_register & VEHICLE_DISCONNECTED_MASK) != 0U)
              {
                  state = IDLE; // Reset to IDLE if vehicle is disconnected
                  auth_failure_reason = AUTH_REASON_NONE;
                  charger_event_register &= ~VEHICLE_DISCONNECTED_MASK;
              }              
              break;
          case READY:
              printf("Charger is ready.\n");
              if((charger_event_register & START_REQUESTED_MASK) != 0U)
              {
                  state = CHARGING;
                  charger_event_register &= ~START_REQUESTED_MASK;
              }
              break;
          case CHARGING:
              printf("Charging in progress.\n");
                            
              handle_charging_state();

              if((charger_event_register & FAULT_DETECTED_MASK) != 0U)
              {
                  state = FAULTED;
                  charger_event_register &= ~FAULT_DETECTED_MASK;
              }
              else if((charger_event_register & CHARGING_COMPLETE_MASK) != 0U)
              {
                  state = READY; // Return to READY after charging is complete;
                  charger_event_register &= ~CHARGING_COMPLETE_MASK;
              }               
              break;
          case FAULTED:
              printf("Charger is faulted.\n");
              break;
          default:
              printf("Unknown state.\n");
              break;
      }

      if(state != previous_state)
            
        {   
            if(state == READY)
            {
                ready_entry();
            }

            if(previous_state == READY)
            {
                ready_exit();
            }

            if(state == CHARGING)
            {
                charging_entry();
            }

            if(previous_state == CHARGING)
            {
                charging_exit();
            }
    }
        previous_state = state;
    
    }

    return 0;
}

void handle_charging_state(void)
{
   printf("Monitoring voltage...\n");
   printf("Monitoring Current...\n");
   printf("Monitoring Temperature...\n");
}

void charging_entry(void)
{
    printf("Charging started.\n");
}
void charging_exit(void)
{
    printf("Charging stopped.\n");
}

void ready_entry(void)
{
    printf("Charger is ready for charging.\n");    
}

void ready_exit(void)
{
    printf("Exiting READY state.\n");
}