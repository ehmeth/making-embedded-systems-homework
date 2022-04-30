

//only necessary for non WIN32 systems
#ifndef _WIN32

/*
Sources:
https://stackoverflow.com/a/448982/910094
https://stackoverflow.com/a/58454101/910094
*/

#include <zephyr.h>
#include <drivers/uart.h>
#include <stdio.h>

/* change this to any other UART peripheral if desired */
#define UART_DEVICE_NODE DT_CHOSEN(zephyr_shell_uart)

static const struct device *uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE);

static int last_received_char = EOF;
static int input_ready = 0;

int _kbhit()
{
    if (!input_ready) {
       int result = uart_poll_in(uart_dev, &last_received_char);
       input_ready = (0 == result);
    }
    return input_ready;
}

int _getch()
{
    if (input_ready) {
        input_ready = false;
        return last_received_char;
    } else {
        return EOF;
    }
}


#endif
