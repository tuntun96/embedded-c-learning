#include <stdio.h>
#include <stdint.h>

/* EV Charger Configuration */
#define MAX_VOLTAGE 900U
#define MAX_CURRENT 200U
#define MAX_TEMPERATURE 80U

int main(void)
{
  uint16_t voltage = 950U;
  uint16_t current = 250U;
  uint8_t temperature = 90U;

  printf(
      "EV Charger Configuration\n");
  printf("\n");

  printf(
      "Maximum Voltage: %u V\n",
      (unsigned int)MAX_VOLTAGE);

  printf(
      "Maximum Current: %u A\n",
      (unsigned int)MAX_CURRENT);

  printf(
      "Maximum Temperature: %u C\n",
      (unsigned int)MAX_TEMPERATURE);

  printf("\n");

  printf(
      "Measured Voltage: %u V\n",
      (unsigned int)voltage);

  printf(
      "Measured Current: %u A\n",
      (unsigned int)current);

  printf(
      "Measured Temperature: %u C\n",
      (unsigned int)temperature);

  if (voltage > MAX_VOLTAGE)
  {
    printf(
        "Voltage Status: OVER-VOLTAGE FAULT\n");
  }
  else
  {
    printf(
        "Voltage Status: NORMAL\n");
  } 

  /* Current check starts independently */
  if (current > MAX_CURRENT)
  {
    printf(
        "Current Status: OVER-CURRENT FAULT\n");
  }
  else
  {
    printf(
        "Current Status: NORMAL\n");
  }

  /* Temperature check starts independently */
  if (temperature > MAX_TEMPERATURE)
  {
    printf(
        "Temperature Status: OVER-TEMPERATURE FAULT\n");
  }
  else
  {
    printf(
        "Temperature Status: NORMAL\n");
  }

  return 0;
}
