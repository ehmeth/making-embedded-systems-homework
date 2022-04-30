Porting a Commandline interface onto Zephyr OS
==============================================

The goal was to port an existing implementation of a commandline user interface onto our board and chosen software platform.
Since I am going with Zephyr OS for my final project, I decided to port the code to that platform.

To be fair, Zephyr already offers a similar interface called [Shell](https://docs.zephyrproject.org/latest/services/shell/index.html) which offers more transports and is already well integrated into the system, and I will probably use that in my project. On the other hand, it's always a good idea to have a backup plan in case I'll need every last byte of the codespace and having a simpler commandline interface might just save me a few bytes while not sacrificing the whole functionality.

## Porting

The porting process was relatively straightforward, only editing the conioCompat.c file and trying to mimic the behaviour of the Windows ConIO interface. The mapping is not obvious since ConIO operates on concepts of a keyboard and keys being pressed, which doesn't translate into any of the UART interfaces offered by Zephyr, but with a few state variables I was able to get the module going.

One bug that threw a wrench into my implementation was caused by a seemingly innocuous line of code. Can you spot it?

```C
static int getch_noblock() {
    if (_kbhit())
        return _getch();
    else
        return EOF;
}

eConsoleError ConsoleIoReceive(uint8_t *buffer, const uint32_t bufferLength, uint32_t *readLength)
{
	uint32_t i = 0;
	char ch;
	
	ch = getch_noblock();
	while ( ( EOF != ch ) && ( i < bufferLength ) )
	{
		buffer[i] = (uint8_t) ch;
		i++;
		ch = getch_noblock();
	}
	*readLength = i;
	return CONSOLE_SUCCESS;
}
```

The `(EOF != ch)` part. What type is `ch`? What type is `EOF`? What type is returned by `getch_noblock()`? Yeah, integer types in C are weird and they have all sorts of promotion rules when you try to compare not exactly compatible types. In case of my system and compiler, this condition was always true because for whatever reason `( EOF != (char)EOF )` is always true. Changing the type of `ch` to `int` fixed the issue.

To verify this I added the following print statement in my code and the result was:
```C
printf("Result of ( EOF != (char)EOF ): %d\n", ( EOF != (char)EOF ));
// Result of ( EOF != (char)EOF ): 1
```

In the end, the port was a success and I even was able to add a few simple commands. See it in action below.

![Port in action](assets/commandline_port_demo.gif)

