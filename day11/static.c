#include <stdio.h>
#include <stdint.h>

void update_session_count(void);

int main(void)
{
  printf("EV Charger Session Counter\n");
  printf("\n");
  

  update_session_count();
  update_session_count();
  update_session_count();
  update_session_count();
  update_session_count();

  return 0;

}

void update_session_count(void)
{
  static uint32_t session_count = 0U;
  session_count++;
  printf(
    "Total Charging Session: %u\n",
    session_count
  );
}