#include "events.h"
#include <zephyr.h>
#include <sys/util.h>
#include <sys/printk.h>
#include <inttypes.h>

K_EVENT_DEFINE(blink_events);

static void blink_handler(struct k_timer *dummy);
K_TIMER_DEFINE(blink_timer, blink_handler, NULL);

static void blink_handler(struct k_timer *dummy)
{
   k_event_post(&blink_events, evBlinkTimeout);
}

void event_generate(eBlinkyEvents_t event)
{
   k_event_post(&blink_events, event);
}

uint32_t event_wait(uint32_t event_set)
{
   return k_event_wait(&blink_events, event_set , true, K_FOREVER);
}

void event_periodic(eBlinkyEvents_t event, uint32_t period_ms, bool active)
{
   if (active) 
   {
      k_timer_start(&blink_timer, K_MSEC(period_ms), K_MSEC(period_ms));
   } 
   else
   {
      k_timer_stop(&blink_timer);
   }
}

