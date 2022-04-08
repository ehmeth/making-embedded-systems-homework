#include "led.h"
#include "button.h"
#include "events.h"
#include "errors.h"

#define BLINK_TIME_MS 100

void main(void)
{
   bool blinking_active = true;
   uint32_t init_error = errOk;
   uint32_t events = 0;
   bool button_pressed = 0;
   bool led_on = 0;
   init_error = button_init();

   if (!init_error)
   {
      init_error = led_init();
   }
   
	if (!init_error) {
      button_pressed = button_is_pressed();
      event_periodic(evBlinkTimeout, BLINK_TIME_MS, blinking_active);

		while (1) {
         events = event_wait(evBlinkTimeout | evButtonStateChanged);

         if (events & evBlinkTimeout) { 
            led_on = !led_on;
         }

         if (events & evButtonStateChanged) { 
            button_pressed = button_is_pressed();

            if (!button_pressed)
            {
               blinking_active = !blinking_active;
               event_periodic(evBlinkTimeout, BLINK_TIME_MS, blinking_active);
               led_on = 0;
            } 
         }

         /* override led state when button is pressed down */
         led_set(led_on || button_pressed);
		}
	}
}
