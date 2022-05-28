Optimizing Algorithms
=====================

Today I will be exploring a few ways to calculate the number of set bits in an unsigned 32-bit integer.

The number of set bits in an unsigned variable is also known as popcount or "population count".
At first glance the problem seems like a typical CS 101 algorithmic riddle and the answer is not obviously useful.
Maybe it could be used for a simple error detection mechanism capable of detecting a bit swap or two in the right circumstances, but there are better error detection and correction algorithms out there.
On the other hand, there is an [x86 instruction POPCNT](https://www.felixcloutier.com/x86/popcnt) and even a [NEON SIMD instruction VCNT](https://developer.arm.com/documentation/den0018/a/NEON-Intrinsics-Reference/Data-processing/VCNT), so it must have some other uses. Well it does and you can read about them [here](https://vaibhavsagar.com/blog/2019/09/08/popcount) for example.

Either way, let's implement it!

Popcount implementations
------------------------

Let's start with the naive approach. Let's just try counting any set bits we find in a variable while there are some bits set.

```C
uint32_t popcountA(uint32_t value)
{
    uint32_t popcount = 0;
    
    while (value)
    {
        popcount += (value & 0x1);
        value >>= 1;
    }

    return popcount;
}
```

Not a bad start, in the worst case of `0x80000000` we'll loop 32 times to get to that last set bit, but hey, it works! We can even unroll the loop.

```C
uint32_t popcountB(uint32_t value)
{
    uint32_t popcount = 
        (value >>  0) & 0x1u +
        (value >>  1) & 0x1u +
        (value >>  2) & 0x1u +
        (value >>  3) & 0x1u +
        (value >>  4) & 0x1u +
        (value >>  5) & 0x1u +
        (value >>  6) & 0x1u +
        (value >>  7) & 0x1u +
        (value >>  8) & 0x1u +
        (value >>  9) & 0x1u +
        (value >> 10) & 0x1u +
        (value >> 11) & 0x1u +
        (value >> 12) & 0x1u +
        (value >> 13) & 0x1u +
        (value >> 14) & 0x1u +
        (value >> 15) & 0x1u +
        (value >> 16) & 0x1u +
        (value >> 17) & 0x1u +
        (value >> 18) & 0x1u +
        (value >> 19) & 0x1u +
        (value >> 20) & 0x1u +
        (value >> 21) & 0x1u +
        (value >> 22) & 0x1u +
        (value >> 23) & 0x1u +
        (value >> 24) & 0x1u +
        (value >> 25) & 0x1u +
        (value >> 26) & 0x1u +
        (value >> 27) & 0x1u +
        (value >> 28) & 0x1u +
        (value >> 29) & 0x1u +
        (value >> 30) & 0x1u +
        (value >> 31) & 0x1u;
    
    return popcount;
}
```

Can we do better? Of course! Let's try some bit-twiddling shenanigans! Bring your "Hacker's Delight" books and some pop-corn. (Get it? Pop-corn?)

```C
uint32_t popcountC(uint32_t value)
{
    uint32_t popcount = 0;

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
uint32_t popcountD(uint32_t x)
{
    static const uint32_t LUT[] = {
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
    return 
        LUT[(x >>  0) & 0xFF] +
        LUT[(x >>  8) & 0xFF] +
        LUT[(x >> 16) & 0xFF] +
        LUT[(x >> 24) & 0xFF];
}
```

That's a lot of constants, but the resulting assembly will probably be quite short.

How about we try a less flash-intensive middle ground?

```C
uint32_t popcountE(uint8_t x)
{
    static const uint8_t lookup[16] = {
        0, 1, 1, 2, 1, 2, 2, 3,
        1, 2, 2, 3, 2, 3, 3, 4 
    };
   
    return 
        lookup[(x >>  0) &0xF] +
        lookup[(x >>  4) &0xF] +
        lookup[(x >>  8) &0xF] +
        lookup[(x >> 12) &0xF] +
        lookup[(x >> 16) &0xF] +
        lookup[(x >> 20) &0xF] +
        lookup[(x >> 24) &0xF] +
        lookup[(x >> 28) &0xF];
}
```

That looks like a fair compromise.

And if you're using GCC and trust the compiler people, you can just use their implementation and save yourself the hassle.

```C
uint32_t popcountF(uint32_t n)
{
      return __builtin_popcountl(n);
}
```

And that's it! You can see all the above code examples [here](https://godbolt.org/z/GPPdPercf) and experiment with other compilers and all their options. Have fun!
