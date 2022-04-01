Development Board Investigations
================================

Or why, oh why do we need all the silicone to go to crypto miners?

32F429IDISCOVERY
----------------

### Board hardware

![Hardware Component Diagram](assets/board-x-hardware-component-diagram.png)

|                                             | 32F429IDISCOVERY              |
|---------------------------------------------|-------------------------------|
|Processor                                    | STM32F429ZI                   |
|Flash                                        | 2Mb internal flash            |
|RAM                                          | Internal 256 kb + 4 kb SRAM, External 8 Mb SDRAM       |
|ADC Features                                 | 3×12-bit, 2.4 MSPS ADC        |
|Interesting peripherals                      | TFT-LCD 2.4" QVGA display, MEMS motion sensor 3-axis digital output gyroscope, USB OTG port, push buttons for reset and custom user input, LEDs for power and USB status and for custom signalling, on-board ST-LINK/V2-B |
|Board price (Digikey, Mouser, Octopart)      | 32-51 USD                     |
|Processor price                              | 21-35 USD per piece, usually in stock   |

### Possible applications

The board has a rather powerful 180 MHz MCU with a lot of Flash and RAM, so it should be the perfect candidate for prototyping and trying out ideas before selecting any target hardware.

The set of peripherals is not particularly impressive, not hinting to any particular application, but the amount of I/O pins available through headers makes the system easy to expand.

The vendor application notes for the board aren't very interesting, but the ones for the MCU itself provide a lot of information on e.g. audio DSP or the chips' TFT LCD screen controller.

STM32F7508-DK
-------------

### Board hardware

![Hardware Component Diagram](assets/board-x-hardware-component-diagram.png)

|                                             | STM32F7508-DK                 |
|---------------------------------------------|-------------------------------|
|Processor                                    | STM32F750N8H6                 |
|Flash                                        | 64 kb internal, 16 Mb external QSPI |
|RAM                                          | Internal 340 kb SRAM, External 8 Mb SDRAM       |
|ADC Features                                 | 3×12-bit, 2.4 MSPS ADC        |
|Interesting peripherals                      | 4.3-inch 480x272 color LCD-TFT with capacitive touch screen, Ethernet connector, USB OTG port, SAI audio codec, 2 ST-MEMS digital microphones, microphone and speaker jack connectors, push buttons for reset and custom user input, on-board ST-LINK/V2-B |
|Board price (Digikey, Mouser, Octopart)      | 58-79 USD                     |
|Processor price                              | ~15 USD for a single chip, half of that in bulk, but none are available currently |

### Possible applications

The microcontroller itself is quite speedy, rocking at 216 MHz and being able to perform almost twice as much integer operations as the previous MCU. The internal memory is not impressive and must be augmented with external chips.

The amount of built-in peripherals on this board is much larger, which makes the board a lot more expensive for obvious reasons. The capacitive touch screen display, the built in microphones and Ethernet connectivity make this a good candidate for prototyping handheld devices or devices with speech-related features.

An application note for this board related to microphone array beam forming also suggest that one of the expected uses for this is device is registering voice input.
