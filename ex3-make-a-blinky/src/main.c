#include "led.h"
#include "button.h"
#include "events.h"
#include "errors.h"

#define BLINK_TIME_MS 100

void main(void)
{
   bool blinking_active = true;
   uint32_t events = 0;
   bool button_pressed = 0;
   bool led_on = 0;
   uint32_t init_error = button_init_press_release(evButtonPressed, evButtonReleased);

   if (!init_error)
   {
      init_error = led_init();
   }
   
	if (!init_error) {
      button_pressed = button_is_pressed();
      event_periodic(evBlinkTimeout, BLINK_TIME_MS, blinking_active);

		while (1) {
         events = event_wait(evBlinkTimeout | evButtonPressed | evButtonReleased);

         if (events & evBlinkTimeout) { 
            led_on = !led_on;
         }

         if (events & evButtonPressed) { 
            button_pressed = true;
         }

         if (events & evButtonReleased) { 
            button_pressed = false;
            blinking_active = !blinking_active;
            event_periodic(evBlinkTimeout, BLINK_TIME_MS, blinking_active);
            led_on = 0;
         }

         /* override led state when button is pressed down */
         led_set(led_on || button_pressed);
		}
	}
}
