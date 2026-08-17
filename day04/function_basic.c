#include<stdio.h>
#include<stdint.h>

/* Function declarations */
void power_on_charger(void);
void perform_self_test(void);
void display_charger_ready(void);

int main(void)
{
  /* Function cals*/
  power_on_charger();
  perform_self_test();
  display_charger_ready();

  return 0;
}

/*Function Definitions*/
void power_on_charger(void)
{
  printf("Charger Power ON\n");
}

void perform_self_test(void)
{
  printf("Performing Self Test\n");
}

void display_charger_ready(void)
{
  printf("Charger is READY\n");
}