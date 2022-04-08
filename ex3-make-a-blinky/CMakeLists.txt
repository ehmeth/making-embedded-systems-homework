cmake_minimum_required(VERSION 3.20.0)

set(BOARD stm32f3_disco)

find_package(Zephyr)
project(stoppable_blinky)

target_sources(app PRIVATE src/main.c src/led.c src/button.c src/events.c)
