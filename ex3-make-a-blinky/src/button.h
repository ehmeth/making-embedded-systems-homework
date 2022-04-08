#ifndef MES_BUTTON_H__

#include <stdbool.h>
#include <inttypes.h>
#include "events.h"

bool button_is_pressed();

uint32_t button_init_on_change(eBlinkyEvents_t onChangeEvent);

uint32_t button_init_press_release(eBlinkyEvents_t onPressedEvent, eBlinkyEvents_t onReleasedEvent);

#define MES_BUTTON_H__
#endif
