Resource Constraints
====================

In order to explore the memory organization in Zephyr OS let's first experiment with variables global variables, both initialized and uninitialized.

We will be working with the following program

```C
#include <zephyr.h>
#include <stdlib.h>

uint32_t foo;
uint32_t bar = 42;

static uint32_t foo_local;
static uint32_t bar_local = 0x42;

void main(void)
{
   extern int __bss_start;
   extern int __data_start;

   static uint32_t foo_main_only;
   uint32_t bar_stack = 451;

   uint32_t *baz_heap = malloc(sizeof(uint32_t));


	printk("Address of uninitialised global with external linkage: %p\n", &foo);
	printk("Address of   initialised global with external linkage: %p\n", &bar);
	printk("Address of uninitialised global with internal linkage: %p\n", &foo_local);
	printk("Address of   initialised global with internal linkage: %p\n", &bar_local);
	printk("Address of            function scoped static variable: %p\n", &foo_main_only);
	printk("Address of                       local stack variable: %p\n", &bar_stack);
	printk("Address of                    heap allocated variable: %p\n", baz_heap);
	printk("Address of                      start of .bss section: %p\n", &__bss_start);
	printk("Address of                     start of .data section: %p\n", &__data_start);

   free(baz_heap);
   baz_heap = 0;
}
```

Declaring the functions as internally linked, i.e. `static` doesn't affect their layout.
Judging by the addresses of initialized and uninitialized variables, in the default configuration in Zephyr the `.data` section precedes the `.bss` section.
The first iteration of the program didn't print the section `*_start` addresses, so this was confirmed later.

```
*** Booting Zephyr OS build zephyr-v3.0.0-1382-g1165bd667f39  ***
Address of uninitialised global with external linkage: 0x20000180
Address of   initialised global with external linkage: 0x20000000
Address of uninitialised global with internal linkage: 0x20000184
Address of   initialised global with internal linkage: 0x20000004
Address of            function scoped static variable: 0x20000188
Address of                       local stack variable: 0x20000764
Address of                    heap allocated variable: (nil)
```

At first, when the heap was not configured for the project, the call to `malloc` returned a null pointer. After adding appropriate configuration variables to `prj.conf` file, the heap allocation succeeded and seems to be placed inside the `.bss` section.

```
*** Booting Zephyr OS build zephyr-v3.0.0-1382-g1165bd667f39  ***
Address of uninitialised global with external linkage: 0x200005a0
Address of   initialised global with external linkage: 0x20000000
Address of uninitialised global with internal linkage: 0x200005a4
Address of   initialised global with internal linkage: 0x20000004
Address of            function scoped static variable: 0x200005a8
Address of                       local stack variable: 0x20000b84
Address of                    heap allocated variable: 0x200000b0
```

Adding printing of addresses of starts of sections confirmed my previous assumptions.

```
*** Booting Zephyr OS build zephyr-v3.0.0-1382-g1165bd667f39  ***
...
Address of                      start of .bss section: 0x20000078
Address of                     start of .data section: 0x20000000
```

For stm32 chips, there is no specific linker file, it simply includes an architecture-specific linker file which defines the overall memory layout. It can be found in the Zephyr OS workspace under the path `zephyr/include/arch/arm/aarch32/cortex_m/scripts/linker.ld`. [See here](https://github.com/zephyrproject-rtos/zephyr/blob/main/include/zephyr/arch/arm/aarch32/cortex_m/scripts/linker.ld)

Swapping the sections was somewhat daunting since the linker script is very generic and far from minimal, but it was in the end possible:

```
Address of uninitialised global with external linkage: 0x20000528
Address of   initialised global with external linkage: 0x200006f4
Address of uninitialised global with internal linkage: 0x2000052c
Address of   initialised global with internal linkage: 0x200006f8
Address of            function scoped static variable: 0x20000530
Address of                       local stack variable: 0x20000b84
Address of                    heap allocated variable: 0x20000038
Address of                      start of .bss section: 0x20000000
Address of                     start of .data section: 0x200006f4
```
