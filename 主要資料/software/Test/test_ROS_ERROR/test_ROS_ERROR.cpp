#include "ros/time.h"
#include <stdint.h>
#include <platform.h>

static void uart_tx(char c)//�Ω�o�e��r 
{
	while ((UART0_REG(UART_REG_TXFIFO) >> 31) & 1 == 1); //check fifo status,send new data if fifo isn't full
	
	UART0_REG(UART_REG_TXFIFO) = c ;
} 

int uart_rx(char *c)//�Ω󱵦���r 
{
	uint32_t val = UART0_REG(UART_REG_RXFIFO); // get rxfifo data
	
	if ((val >> 31)==0) // check fifo status,return data if not empty
	{
		*c = val & 0xff; // only 8bit of data is valid
		return 1;	
	}
	return 0; // fifo is empty , return 0
}

static void uart_print(const char *s)
{
	int i=0;
	
	while (*s != '\0') // the string is end of '\0'
	{
		uart_tx(*s++);
	}
}

int uart_scan(char *buf)
{
	char c=0;
	int exit=0;
	int len=0;
	
	while (exit!=1)
	{
		if(uart_rx(&c)==1)
		{
			if(c=='\r') // when input 'enter', exit
			{
				exit=1;
			}
			else
			{
				buf[len]=c; // store input data
				len++;
				uart_tx(c);
			}
		}
	}
	return len; // return data lengh
}

void demo_uart_init()
{
	//Configure UART to print
	GPIO_REG(GPIO_IOF_EN) |= IOF0_UART0_MASK;
	GPIO_REG(GPIO_IOF_SEL) &= ~IOF0_UART0_MASK;
	
	// 115200 Baud Rate
	// get _cpu_freq() / baud_rate -1, and get_cpu_freq() = 16MHz
	UART0_REG(UART_REG_DIV) = 138;
	
	UART0_REG(UART_REG_TXCTRL) |= UART_TXEN; // enable tx
	UART0_REG(UART_REG_RXCTRL) |= UART_RXEN; // enable rx
}

int main(void)
{
	//Init UART
	demo_uart_init();
	
	char buf[20]={0}; // buffer to store input data
	while (1)
	{
		uart_print(terasic_msg); // print RISC-V
		uart_print("Enter:");    // print promot message "Enter:"
		uart_scan(buf);          // wait user to input data
		uart_print("\n\rYour Input:");
		uart_print(buf);         // print user's data
	}
}


// int main() {
//   uint32_t a = 1, b = 2000000000;
//   ros::normalizeSecNSec(a, b); //  OK
//   // normalizeSecNSec(a, b);   //  會編譯錯誤
// }