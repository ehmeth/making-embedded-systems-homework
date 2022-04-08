#ifndef MES_LED_H__

#include <inttypes.h>
#include <stdbool.h>

void led_set(bool on);

uint32_t led_init();

#define MES_LED_H__
#endif
