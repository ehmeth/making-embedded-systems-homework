#include "button.h"
#include "errors.h"
#include "events.h"

#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <sys/util.h>
#include <sys/printk.h>
#include <inttypes.h>

/*
 * Get button configuration from the devicetree sw0 alias. This is mandatory.
 */
#define SW0_NODE DT_ALIAS(sw0)
#if !DT_NODE_HAS_STATUS(SW0_NODE, okay)
#error "Unsupported board: sw0 devicetree alias is not defined"
#endif
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0});
static struct gpio_callback button_cb_data;

static eBlinkyEvents_t g_onChangeEvent = evNone,
                       g_onPressedEvent = evNone,
                       g_onReleasedEvent = evNone;

static void onchange_handler(struct k_timer *dummy);
static void press_release_handler(struct k_timer *dummy);
static void button_handler(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
static uint32_t button_init_base();

struct k_timer debounce_timer;

static void onchange_handler(struct k_timer *dummy)
{
   event_generate(g_onChangeEvent);
}

static void press_release_handler(struct k_timer *dummy)
{
   event_generate(button_is_pressed() ? g_onPressedEvent : g_onReleasedEvent);
}

static void button_handler(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
   /* wait for level to stabilize, starting a running timer will reset it */
   k_timer_start(&debounce_timer, K_MSEC(10), K_NO_WAIT);
}

uint32_t button_init_press_release(eBlinkyEvents_t onPressedEvent, eBlinkyEvents_t onReleasedEvent)
{
   uint32_t init_error = button_init_base();

   if (!init_error)
   {
      g_onPressedEvent = onPressedEvent;
      g_onReleasedEvent = onReleasedEvent;
      k_timer_init(&debounce_timer, press_release_handler, NULL);
   }

   return init_error;
}

uint32_t button_init_on_change(eBlinkyEvents_t onChangeEvent)
{
   uint32_t init_error = button_init_base();

   if (!init_error)
   {
      g_onChangeEvent = onChangeEvent;
      k_timer_init(&debounce_timer, onchange_handler, NULL);
   }

   return init_error;
}

static uint32_t button_init_base()
{
	int init_error = errOk;

	if (!device_is_ready(button.port)) {
		printk("Error: button device %s is not ready\n", button.port->name);
	   init_error = errButtonNotReady;
	}

   if (!init_error)
   {
      init_error = gpio_pin_configure_dt(&button, GPIO_INPUT);
      if (init_error) {
         printk("Error %d: failed to configure %s pin %d\n", init_error, button.port->name, button.pin);
         init_error = errButtonNotConfigured;
      }
   }

   if (!init_error)
   {
      init_error = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_BOTH);
      if (init_error) {
         printk("Error %d: failed to configure interrupt on %s pin %d\n", init_error, button.port->name, button.pin);
         init_error = errButtonInterruptNotConfigured;
      }
   }

   if (!init_error)
   {
      gpio_init_callback(&button_cb_data, button_handler, BIT(button.pin));
      gpio_add_callback(button.port, &button_cb_data);
      printk("Set up button at %s pin %d\n", button.port->name, button.pin);
   }

   return init_error;
}

bool button_is_pressed()
{
   int state = gpio_pin_get_dt(&button);

   if (state < 0) {
      printk("Error %d: reading state of %s pin %d\n", state, button.port->name, button.pin);
      return false;
   } 

   return state == 0;
}
