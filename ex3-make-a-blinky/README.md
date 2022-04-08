Make a blinky!
==============

This is a blinky program for the STM32F3-DISCO board running on Zephyr OS.

On startup the program will start blinking one of the available user LEDs.
If the user button is pressed, the LED will be on for the duration of the press and then the blinking will be toggled off. Another press will toggle the blinking back on.
The button press/release is debounced by a 10 ms timeoutm so the signal must stabilize for at least 10 ms to trigger the software reaction.

Zephyr provides a pretty high level of abstraction by itself, but the logic of the blinky loop is further separated from the OS by a group of minimal interfaces for the led, the button and an event system.

The OS provides ISRs, which then can be used to add custom callback functions.
Below an example of a timer/clock ISR:
```
#0  blink_handler
#1  z_timer_expiration_handler
#2  sys_clock_announce
#3  sys_clock_isr
#4  <signal handler called>
```

And here is an example stack of a GPIO ISR:
```
#0  button_handler (dev=0x8003b54 <__device_dts_ord_17>, cb=0x200002a0 <button_cb_data>, pins=1) 
#1  gpio_fire_callbacks 
#2  gpio_stm32_isr
#3  __stm32_exti_isr
#4  _isr_wrapper ()
#5  <signal handler called>
```

Although the OS keeps the developer far away from the actual hardware, it's possible to get down to the nitty-gritty using e.g. gdb to inspect the relevant addresses:

GPIOE_ODR when the output LED is on and off:
```
bitmask for the LED pin is 0x8000

(gdb) x/xw 0x48001014
0x48001014:     0x00008000
...
(gdb) x/xw 0x48001014
0x48001014:     0x00000000
```

GPIOA_IDR when the input button is pressed and released:
```
bitmask for button pin is 0x0001
(gdb) x/xw 0x48000010
0x48000010:     0x0000dfe4
...
(gdb) x/xw 0x48000010
0x48000010:     0x0000dfe7
```
 
