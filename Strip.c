#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define GetBit(var,pos) ((var) & (1<<(pos)))

#define numLEDs 72
#define numBytes 216
#define LEDPin 4
#define InputPin 3

//GRB
uint8_t pixels[numBytes] =
{
	0
};

uint8_t pin = LEDPin;
uint8_t pinMask = _BV(LEDPin);

void SetPixel(uint8_t i, uint8_t r, uint8_t g, uint8_t b){
	uint8_t ix = i * 3;
	pixels[ix] = g;
	pixels[ix + 1] = r;
	pixels[ix + 2] = b;
}

void Begin(){
	DDRD |= pinMask;
	PORTD &= ~pinMask;

	//Input
	DDRD &= ~_BV(InputPin);
	PORTD |= _BV(InputPin);
}

inline void Show(){
	PORTD &= ~pinMask;
	//Reset strip signal
	_delay_us(50);

	volatile uint16_t
		i = numBytes;	// Loop counter
	volatile uint8_t
		*ptr = pixels,  // Pointer to next byte
		b = *ptr++,		// Current byte value
		hi,             // PORT w/output bit set high
		lo;             // PORT w/output bit set low

	volatile uint8_t n1, n2 = 0;

	hi = PORTD | pinMask;
	lo = PORTD & ~pinMask;
	n1 = lo;
	if (b & 0x80) n1 = hi;

	asm volatile(
		"headD:"                   "\n\t" // Clk  Pseudocode
		// Bit 7:
		"out  %[port] , %[hi]"    "\n\t" // 1    PORT = hi
		"mov  %[n2]   , %[lo]"    "\n\t" // 1    n2   = lo
		"out  %[port] , %[n1]"    "\n\t" // 1    PORT = n1
		"rjmp .+0"                "\n\t" // 2    nop nop
		"sbrc %[byte] , 6"        "\n\t" // 1-2  if(b & 0x40)
		"mov %[n2]   , %[hi]"    "\n\t" // 0-1   n2 = hi
		"out  %[port] , %[lo]"    "\n\t" // 1    PORT = lo
		"rjmp .+0"                "\n\t" // 2    nop nop
		// Bit 6:
		"out  %[port] , %[hi]"    "\n\t" // 1    PORT = hi
		"mov  %[n1]   , %[lo]"    "\n\t" // 1    n1   = lo
		"out  %[port] , %[n2]"    "\n\t" // 1    PORT = n2
		"rjmp .+0"                "\n\t" // 2    nop nop
		"sbrc %[byte] , 5"        "\n\t" // 1-2  if(b & 0x20)
		"mov %[n1]   , %[hi]"    "\n\t" // 0-1   n1 = hi
		"out  %[port] , %[lo]"    "\n\t" // 1    PORT = lo
		"rjmp .+0"                "\n\t" // 2    nop nop
		// Bit 5:
		"out  %[port] , %[hi]"    "\n\t" // 1    PORT = hi
		"mov  %[n2]   , %[lo]"    "\n\t" // 1    n2   = lo
		"out  %[port] , %[n1]"    "\n\t" // 1    PORT = n1
		"rjmp .+0"                "\n\t" // 2    nop nop
		"sbrc %[byte] , 4"        "\n\t" // 1-2  if(b & 0x10)
		"mov %[n2]   , %[hi]"    "\n\t" // 0-1   n2 = hi
		"out  %[port] , %[lo]"    "\n\t" // 1    PORT = lo
		"rjmp .+0"                "\n\t" // 2    nop nop
		// Bit 4:
		"out  %[port] , %[hi]"    "\n\t" // 1    PORT = hi
		"mov  %[n1]   , %[lo]"    "\n\t" // 1    n1   = lo
		"out  %[port] , %[n2]"    "\n\t" // 1    PORT = n2
		"rjmp .+0"                "\n\t" // 2    nop nop
		"sbrc %[byte] , 3"        "\n\t" // 1-2  if(b & 0x08)
		"mov %[n1]   , %[hi]"    "\n\t" // 0-1   n1 = hi
		"out  %[port] , %[lo]"    "\n\t" // 1    PORT = lo
		"rjmp .+0"                "\n\t" // 2    nop nop
		// Bit 3:
		"out  %[port] , %[hi]"    "\n\t" // 1    PORT = hi
		"mov  %[n2]   , %[lo]"    "\n\t" // 1    n2   = lo
		"out  %[port] , %[n1]"    "\n\t" // 1    PORT = n1
		"rjmp .+0"                "\n\t" // 2    nop nop
		"sbrc %[byte] , 2"        "\n\t" // 1-2  if(b & 0x04)
		"mov %[n2]   , %[hi]"    "\n\t" // 0-1   n2 = hi
		"out  %[port] , %[lo]"    "\n\t" // 1    PORT = lo
		"rjmp .+0"                "\n\t" // 2    nop nop
		// Bit 2:
		"out  %[port] , %[hi]"    "\n\t" // 1    PORT = hi
		"mov  %[n1]   , %[lo]"    "\n\t" // 1    n1   = lo
		"out  %[port] , %[n2]"    "\n\t" // 1    PORT = n2
		"rjmp .+0"                "\n\t" // 2    nop nop
		"sbrc %[byte] , 1"        "\n\t" // 1-2  if(b & 0x02)
		"mov %[n1]   , %[hi]"    "\n\t" // 0-1   n1 = hi
		"out  %[port] , %[lo]"    "\n\t" // 1    PORT = lo
		"rjmp .+0"                "\n\t" // 2    nop nop
		// Bit 1:
		"out  %[port] , %[hi]"    "\n\t" // 1    PORT = hi
		"mov  %[n2]   , %[lo]"    "\n\t" // 1    n2   = lo
		"out  %[port] , %[n1]"    "\n\t" // 1    PORT = n1
		"rjmp .+0"                "\n\t" // 2    nop nop
		"sbrc %[byte] , 0"        "\n\t" // 1-2  if(b & 0x01)
		"mov %[n2]   , %[hi]"    "\n\t" // 0-1   n2 = hi
		"out  %[port] , %[lo]"    "\n\t" // 1    PORT = lo
		"sbiw %[count], 1"        "\n\t" // 2    i-- (don't act on Z flag yet)
		// Bit 0:
		"out  %[port] , %[hi]"    "\n\t" // 1    PORT = hi
		"mov  %[n1]   , %[lo]"    "\n\t" // 1    n1   = lo
		"out  %[port] , %[n2]"    "\n\t" // 1    PORT = n2
		"ld   %[byte] , %a[ptr]+" "\n\t" // 2    b = *ptr++
		"sbrc %[byte] , 7"        "\n\t" // 1-2  if(b & 0x80)
		"mov %[n1]   , %[hi]"    "\n\t" // 0-1   n1 = hi
		"out  %[port] , %[lo]"    "\n\t" // 1    PORT = lo
		"brne headD"              "\n"   // 2    while(i) (Z flag set above)
		:[byte]  "+r" (b),
		[n1]    "+r" (n1),
		[n2]    "+r" (n2),
		[count] "+w" (i)
		: [port]   "I" (_SFR_IO_ADDR(PORTD)),
		[ptr]    "e" (ptr),
		[hi]     "r" (hi),
		[lo]     "r" (lo));
}

int intloop(int index)
{
	if (index < numLEDs)
		return index;
	else
		return index - numLEDs;
}

void DrawRainbow(int shift){
	uint8_t i = 0;

	for (i = 0; i < numLEDs; i++)
	{
		int a = i % 24 * 21;
		uint8_t x = 255 - abs(a - 255);
		uint8_t ii = abs((i + shift) % numLEDs);
		if (i < 12)
			SetPixel(ii, 255, x, 0);
		else if (i < 24)
			SetPixel(ii, x, 255, 0);
		else if (i < 36)
			SetPixel(ii, 0, 255, x);
		else if (i < 48)
			SetPixel(ii, 0, x, 255);
		else if (i < 60)
			SetPixel(ii, x, 0, 255);
		else
			SetPixel(ii, 255, 0, x);
	}
}

void DrawColors(int shift){
	uint8_t i = 0;

	for (i = 0; i < numLEDs; i++)
	{
		int a = i % 24 * 21;
		uint8_t x = 255 - abs(a - 255);

		if (shift < 12)
			SetPixel(i, 255, x, 0);
		else if (shift < 24)
			SetPixel(i, x, 255, 0);
		else if (shift < 36)
			SetPixel(i, 0, 255, x);
		else if (shift < 48)
			SetPixel(i, 0, x, 255);
		else if (shift < 60)
			SetPixel(i, x, 0, 255);
		else
			SetPixel(i, 255, 0, x);
	}
}

void DrawRed(){
	uint8_t i = 0;

	for (i = 0; i < numLEDs; i++)
	{
		SetPixel(i, 255, 0, 0);
	}

	_delay_ms(1);
}

void DrawFlag(uint8_t shift){
	uint8_t i = 0;

	for (i = 0; i < numLEDs; i++)
	{
		if (i < (numLEDs / 2))
			SetPixel(i, 0, 0, 255);
		else
			SetPixel(i, 255, 0, 0);
	}

	_delay_ms(1);
}

void DrawFlag2(uint8_t shift){
	uint8_t i = 0;

	for (i = 0; i < numLEDs; i++)
	{
		uint8_t ii = (i + shift) % numLEDs;
		if (i < (numLEDs / 2))
			SetPixel(ii, 0, 0, 255);
		else
			SetPixel(ii, 255, 0, 0);
	}

	_delay_ms(1);
}

void DrawDots(uint8_t shift){

	SetPixel((shift) % numLEDs, 255, 0, 0);
	SetPixel((shift + 10) % numLEDs, 255, 0, 0);
	SetPixel((shift + 20) % numLEDs, 255, 0, 0);
	SetPixel((shift + 30) % numLEDs, 255, 0, 0);
	SetPixel((shift + 40) % numLEDs, 255, 0, 0);
	SetPixel((shift + 50) % numLEDs, 255, 0, 0);
	SetPixel((shift + 60) % numLEDs, 255, 0, 0);

	Show();

	_delay_ms(1);

	SetPixel((shift) % numLEDs, 0, 0, 0);
	SetPixel((shift + 10) % numLEDs, 0, 0, 0);
	SetPixel((shift + 20) % numLEDs, 0, 0, 0);
	SetPixel((shift + 30) % numLEDs, 0, 0, 0);
	SetPixel((shift + 40) % numLEDs, 0, 0, 0);
	SetPixel((shift + 50) % numLEDs, 0, 0, 0);
	SetPixel((shift + 60) % numLEDs, 0, 0, 0);
}

int main(void){
	Begin();
	uint8_t shift = 0;
	uint8_t inputState = 0;
	uint8_t mode = 0;

	while (1){

		switch (mode){
			case 0: DrawRed(); break;
			case 1: DrawFlag(shift); break;
			case 2: DrawFlag2(shift); break;
			case 3: DrawDots(shift); break;
			case 4: DrawRainbow(0); break;
			case 5: DrawRainbow(shift); _delay_ms(20); break;
			case 6: DrawRainbow(shift); _delay_ms(5); break;
			case 7: DrawColors(shift); _delay_ms(50); break;
			case 8: DrawColors(shift); _delay_ms(5); break;
			default: mode = 0; break;
		}

		Show();

		shift++;

		if (shift >= numLEDs)
			shift = 0;

		uint8_t input = GetBit(PIND, InputPin);
		if (input != inputState && input > 0)
		{
			mode++;
		}

		inputState = input;
	}
	return 0;
}