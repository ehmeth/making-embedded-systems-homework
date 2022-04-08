#include "led.h"
#include "errors.h"

#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <sys/util.h>
#include <sys/printk.h>
#include <inttypes.h>

#define LED0_NODE DT_ALIAS(led0)
#if !DT_NODE_HAS_STATUS(LED0_NODE, okay)
#error "Unsupported board: led0 devicetree alias is not defined"
#endif
static struct gpio_dt_spec led = GPIO_DT_SPEC_GET_OR(LED0_NODE, gpios, {0});

uint32_t led_init() 
{
   uint32_t init_error = errOk;

	if (!device_is_ready(led.port)) {
		printk("Error: LED device %s is not ready; ignoring it\n",
		       led.port->name);
      init_error = errLEDNotReady;
	}

	if (!init_error) {
		init_error = gpio_pin_configure_dt(&led, GPIO_OUTPUT);
		if (init_error) 
      {
			printk("Error %d: failed to configure LED device %s pin %d\n", init_error, led.port->name, led.pin);
			init_error = errLEDNotConfigured;
		} else {
			printk("Set up LED at %s pin %d\n", led.port->name, led.pin);
		}
	}

   return init_error;
}

void led_set(bool on)
{
   gpio_pin_set_dt(&led, (uint32_t)on);
}

