#ifndef MES_EVENTS_H__

#include <inttypes.h>
#include <stdbool.h>

typedef enum {
   evBlinkTimeout         = 1 << 0,
   evButtonStateChanged   = 1 << 1
} eBlinkyEvents_t;

void event_generate(eBlinkyEvents_t event);

uint32_t event_wait(uint32_t event_set);

void event_periodic(eBlinkyEvents_t event, uint32_t period_ms, bool active);

#define MES_EVENTS_H__
#endif
