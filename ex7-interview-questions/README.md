Interview Questions
===================

These are solutions to 2 chosen interview questions from the Making Embedded Systems book.

Question R.9
------------

Given an integer variable a, write two code fragments. The first should set bit 3 of a.
The second should clear bit 3 of a. In both cases, the other bits should be unmodified.

### Solution:

```C
/* Set bit 3 in a */
a = a | (1u << 3u);

/* Clear bit 3 in a */
a = a & ~(1u << 3u);
```

Question R.10
-------------

On a certain project it is required to set an integer variable at the absolute address 0x67a9 to the value 0xaa55. Write code to accomplish this task.

### Solution:

```
int *variable = (int *)(0x67a9);
*variable = 0xaa55;
```

