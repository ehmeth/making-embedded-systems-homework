Optimizing Algorithms
=====================

Today I will be exploring a few ways to calculate the number of set bits in an unsigned integer variable.
The following implementations focus on 8-bit variables, but can fairly easily be extended to 16- and 32-bits.

The number of set bits in an unsigned variable is also known as popcount or "population count".
At first glance the problem seems like a typical CS 101 algorithmic riddle and the answer is not obviously useful.
Maybe it could be used for a simple error detection mechanism capable of detecting a bit swap or two in the right circumstances, but there are better error detection and correction algorithms out there.
On the other hand, there is an [x86 instruction POPCNT](https://www.felixcloutier.com/x86/popcnt) and even a [NEON SIMD instruction VCNT](https://developer.arm.com/documentation/den0018/a/NEON-Intrinsics-Reference/Data-processing/VCNT), so it must have some other uses. Well it does and you can read about them [here](https://vaibhavsagar.com/blog/2019/09/08/popcount) for example.

Either way, let's implement it!

Popcount implementations
------------------------

Let's start with the naive approach. Let's just try counting any set bits we find in a variable while there are some bits set.

```C
uint8_t popcountA(uint8_t value)
{
    uint8_t popcount = 0;
    
    while (value)
    {
        popcount += (value & 0x1);
        value >>= 1;
    }

    return popcount;
}
```

Not a bad start, in the worst case of `0x80` we'll loop 8 times to get to that last set bit, but hey, it works! We can even unroll the loop.

```C
uint8_t popcountB(uint8_t value)
{
    uint8_t popcount = 
        (value >> 0) & 0x1u +
        (value >> 1) & 0x1u +
        (value >> 2) & 0x1u +
        (value >> 3) & 0x1u +
        (value >> 4) & 0x1u +
        (value >> 5) & 0x1u +
        (value >> 6) & 0x1u +
        (value >> 7) & 0x1u;
    
    return popcount;
}
```

Can we do better? Of course! Let's try some bit-twiddling shenanigans! Bring your "Hacker's Delight" books and some pop-corn. (Get it? Pop-corn?)

```C
uint8_t popcountC(uint8_t value)
{
    uint8_t popcount = 0;

    while (value) {
        // clear least significant set bit 
        value &= value - 1;
        popcount++;
    }
    
    return popcount;
}
```

Huh? Don't fret if `value &= value - 1` doesn't seem obvious, it's not. 

On one hand, we'll reduce the number of loop iterations, on the other... loop unrolling is no longer an obvious improvement.

Next we can move to look-up tables.

```C
uint8_t popcountD(uint8_t x)
{
    static const uint8_t LUT[] = {
        0, 1, 1, 2, 1, 2, 2, 3, 
        1, 2, 2, 3, 2, 3, 3, 4, 
        1, 2, 2, 3, 2, 3, 3, 4, 
        2, 3, 3, 4, 3, 4, 4, 5, 
        1, 2, 2, 3, 2, 3, 3, 4, 
        2, 3, 3, 4, 3, 4, 4, 5, 
        2, 3, 3, 4, 3, 4, 4, 5, 
        3, 4, 4, 5, 4, 5, 5, 6, 
        1, 2, 2, 3, 2, 3, 3, 4, 
        2, 3, 3, 4, 3, 4, 4, 5, 
        2, 3, 3, 4, 3, 4, 4, 5, 
        3, 4, 4, 5, 4, 5, 5, 6, 
        2, 3, 3, 4, 3, 4, 4, 5, 
        3, 4, 4, 5, 4, 5, 5, 6, 
        3, 4, 4, 5, 4, 5, 5, 6, 
        4, 5, 5, 6, 5, 6, 6, 7, 
        1, 2, 2, 3, 2, 3, 3, 4, 
        2, 3, 3, 4, 3, 4, 4, 5, 
        2, 3, 3, 4, 3, 4, 4, 5, 
        3, 4, 4, 5, 4, 5, 5, 6, 
        2, 3, 3, 4, 3, 4, 4, 5, 
        3, 4, 4, 5, 4, 5, 5, 6, 
        3, 4, 4, 5, 4, 5, 5, 6, 
        4, 5, 5, 6, 5, 6, 6, 7, 
        2, 3, 3, 4, 3, 4, 4, 5, 
        3, 4, 4, 5, 4, 5, 5, 6, 
        3, 4, 4, 5, 4, 5, 5, 6, 
        4, 5, 5, 6, 5, 6, 6, 7, 
        3, 4, 4, 5, 4, 5, 5, 6, 
        4, 5, 5, 6, 5, 6, 6, 7, 
        4, 5, 5, 6, 5, 6, 6, 7, 
        5, 6, 6, 7, 6, 7, 7, 8, 
    };
   return LUT[x];
}
```

That's a lot of constants, but the resulting assembly will probably be quite short.

How about we try a less flash-intensive middle ground?

```C
uint8_t popcountE(uint8_t n)
{
    static const uint8_t lookup[16] = {
        0, 1, 1, 2, 1, 2, 2, 3,
        1, 2, 2, 3, 2, 3, 3, 4 
    };
   
    return (lookup[n&0xF] << 4) + lookup[n>>4];
}
```

That looks like a fair compromise.

And if you're using GCC and trust the compiler people, you can just use their implementation and save yourself the hassle.

```C
uint8_t popcountF(uint8_t n)
{
      return __builtin_popcount(n);
}
```

And that's it! You can see all the above code examples [here](https://godbolt.org/z/ooez9ffhr) and experiment with other compilers and all their options. Have fun!
