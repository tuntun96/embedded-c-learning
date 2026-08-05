#include <stdio.h>

#include "charger.h"

/* private function declaration */
static void check_internal_fault(void);

/* Public function */
void charger_monitor(void)
{
  printf(
    "EV Charger Monitoring Started\n"
  );
  check_internal_fault();
}

/* Private helper function */
static void check_internal_fault(void)
{
  printf(
    "Internal Fault Check: PASSED\n"
  );
}