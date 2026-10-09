#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#define ENABLE_GLOBAL_INTERRUPT()  __asm volatile ("CPSIE i" : : : "memory")
#define DISABLE_GLOBAL_INTERRUPT() __asm volatile ("CPSID i" : : : "memory")

#define PERIPH_BASE                 (0x40000000UL)

// RCC
#define AHB1_OFFSET                 (0x20000UL)
#define AHB1_BASE                    (PERIPH_BASE + AHB1_OFFSET)

#define RCC                         (0x40023800UL)

#define AHB1_ENROFFSET              (0x30UL)
#define AHB1_RCC                    (*(volatile unsigned int *)(AHB1_ENROFFSET + RCC))

//button

#define GPIOC_OFFSET                (0x0800UL)
#define GPIOC_BASE                  (AHB1_BASE + GPIOC_OFFSET)

#define MODER_OFFSET                (0x00UL)
#define GPIOC_MODER                 (*(volatile unsigned *)(GPIOC_BASE + MODER_OFFSET ))

//uart


#define APB1_OFFSET                 (0x0000UL)
#define APB1_BASE                   (PERIPH_BASE + APB1_OFFSET)

#define APB1ENR_OFFSET              (0x40UL)
#define RCC_APB1                    (*(volatile unsigned int *)(RCC + APB1ENR_OFFSET))

#define UART_OFFSET                 (0x4400UL)
#define UART_BASE                   (APB1_BASE + UART_OFFSET)

#define GPIOA_OFFSET                (0x0000UL)
#define GPIOA_BASE                  (AHB1_BASE + GPIOA_OFFSET )

#define AFRL_OFFSET                 (0x20UL)
#define GPIOA_AFRL                  (*(volatile unsigned int *)(GPIOA_BASE + AFRL_OFFSET))


#define BRR_OFFSET                  (0x08UL)
#define UART_BRR                    (*(volatile unsigned int *)(BRR_OFFSET + UART_BASE))



#define GPIOA_MODER                 (*(volatile unsigned int *)(GPIOA_BASE + MODER_OFFSET))


#define CR1_OFFSET                  (0x0CUL)
#define UART_CR1                    (*(volatile unsigned int *)(UART_BASE + CR1_OFFSET))

#define DR_OFFSET                   (0x04UL)
#define UART_DR                    (*(volatile unsigned int *)(DR_OFFSET + UART_BASE))

#define SR_OFFSET                   (0x00UL)
#define UART_SR                    (*(volatile unsigned int *)(UART_BASE + SR_OFFSET))

//interupt
#define APB2_OFFSET                 (0x10000UL)
#define APB2_BASE                   (PERIPH_BASE + APB2_OFFSET)

#define APB2ENR_OFFSET              (0x44UL)
#define RCC_APB2                    (*(volatile unsigned int *)(RCC + APB2ENR_OFFSET))


#define SYSCFG_BASE                (0x40013800UL)
#define SYSCFG_EXTICR4OFFSET       (0x14UL)
#define SYSCFG_EXTICR4             (*(volatile unsigned int *)(SYSCFG_BASE + SYSCFG_EXTICR4OFFSET))

#define EXTI_BAE                   (0x40013C00UL)
#define EXTI_IMROFFSET             (0x00UL)
#define EXTI_IMR                   (*(volatile unsigned int *)(EXTI_BAE + EXTI_IMROFFSET))

#define EXTI_FTSROFFSET            (0x0CUL)
#define EXIT_FTSR                  (*(volatile unsigned int *)(EXTI_BAE + EXTI_FTSROFFSET))
#define NVIC_ISER1                 (*(volatile unsigned int *)(0xE000E104))

#define EXTIPR_OFFSET              (0x14UL)
#define EXTIPR_BASE                (*(volatile unsigned int *)(EXTI_BAE + EXTIPR_OFFSET))

#define  GPIOCENR                   (1U<<2)
#define  GPIOAENR                   (1U<<0)
#define  UART2ENR                   (1U<<17)
#define SYS_FERQ                    16000000
#define UART_BAUDRATE               115200
#define CR1_TE                      (1U<<3)
#define UASRT_EN                    (1U<<13)
#define SYSGENR                     (1U<<14)
#define LINE13                      (1U<<13)
#define SR_TXE                      (1<<7)

void button_in_it();
void interupt_init();
void uart2_tx_init();
void uart2_write(int ch);
void uart2_write_string(char *str);
static void uart_set_baudrate(uint32_t periphclk, uint32_t BaudRate);
static uint16_t compute_uart_div(uint32_t periphclk, uint32_t BaudRate);
void EXTI15_10_IRQHandler(void);

int main(void){

uart2_tx_init();
button_in_it();
interupt_init();
while(1){


        uart2_write_string("......NORMAL MESSAGE\r\n");
        for(volatile int i=0;i<5000000;i++){

        }
}

}

void button_in_it(void){

    AHB1_RCC |=GPIOCENR;

    GPIOC_MODER &=~(1U<<26);
    GPIOC_MODER &=~(1U<<27);

}

void interupt_init(void){

	//__disable_irq();

  AHB1_RCC |=GPIOCENR;

  RCC_APB2 |=SYSGENR;


    GPIOC_MODER &=~(1U<<26);
    GPIOC_MODER &=~(1U<<27);

    //SYSCFG_EXTICR4 |=(1U<<5);
    SYSCFG_EXTICR4 &=~(0xF<<4);
	SYSCFG_EXTICR4 |=(0x2<<4);


    EXTI_IMR|= (1U<<13);

    EXIT_FTSR|=(1U<<13);

    	NVIC_ISER1 |=(1<<(40-32));

    	//__enable_irq();
}

void uart2_tx_init(void){

    AHB1_RCC |=GPIOAENR;

    GPIOA_MODER &=~(1U<<4);
    GPIOA_MODER |=(1U<<5);

    GPIOA_AFRL |=(1U<<8) ;
    GPIOA_AFRL |=(1U<<9);
    GPIOA_AFRL |=(1U<<10);
    GPIOA_AFRL &=~(1U<<11);

    RCC_APB1 |=UART2ENR;


    uart_set_baudrate(SYS_FERQ, UART_BAUDRATE);

    UART_CR1 = CR1_TE;
    UART_CR1 |= UASRT_EN;

}

void uart2_write(int ch){
    while(!(UART_SR & SR_TXE));
    UART_DR = (ch & 0XFF);
}

void uart2_write_string(char *str) {
    while (*str) {
        uart2_write(*str);
        str++;
    }
}


void EXTI15_10_IRQHandler(void){

   if((EXTIPR_BASE & LINE13)!=0){

    EXTIPR_BASE |=LINE13;
    uart2_write_string("....INTERUPT ENABLED.....\r\n");


   }

}



static uint16_t compute_uart_div(uint32_t periphclk, uint32_t BaudRate)
{
    return ((periphclk + (BaudRate / 2U)) / BaudRate);
}

static void uart_set_baudrate(uint32_t periphclk, uint32_t BaudRate)
{
    UART_BRR = compute_uart_div(periphclk, BaudRate);
}
