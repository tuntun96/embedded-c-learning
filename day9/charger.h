#ifndef CHARGER_H
#define CHARGER_H

#include <stdint.h>

/* Charger function declarations */

void charger_init(void);

uint16_t read_voltage(void);

uint32_t calculate_power(
  uint16_t voltage,
  uint16_t current
);

uint8_t check_over_voltage(
  uint16_t voltage
);

void update_voltage(
  uint16_t *voltage,
  uint16_t new_voltage
);

#endif