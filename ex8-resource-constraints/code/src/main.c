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
