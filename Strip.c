#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define numLEDs 72
#define numBytes 216
#define LEDPin 4

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
	PORTD &= ~pinMask;
	DDRD |= pinMask;


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

int main(void){
	Begin();
	uint8_t index = 0;

	while (1){
		SetPixel(index, 0, 0, 0);

		Show();

		if (index >= numLEDs)
			index = 0;

		SetPixel(index++, 255, 0, 0);

		Show();
	}
	return 0;
}