#include "hal/soc/soc.h"
#include "board.h"
#include "uart_port.h"
#include <project.h>

#include <stdio.h>
#include <stdbool.h>
uart_dev_t uart_0;

extern uint32_t HW_Get_MFT_Baud(void);

#define UART_1_UART_BITS_TO_WAIT    (2u)
int default_UART_Init(void)
{
    uart_0.port                = STDIO_UART;
    uart_0.config.baud_rate    = HW_Get_MFT_Baud();
    uart_0.config.data_width   = DATA_WIDTH_8BIT;
    uart_0.config.parity       = NO_PARITY;
    uart_0.config.stop_bits    = STOP_BITS_1;
    uart_0.config.flow_control = FLOW_CONTROL_DISABLED;

    return hal_uart_init(&uart_0);
}

extern void wakeup_from_uart(char ch);
extern void linkwan_serial_input(uint8_t cmd);
void UART_API(_customISR)(void)
{
    while(0u != (UART_API(_GetRxInterruptSourceMasked()) & UART_FLAG(_INTR_RX_NOT_EMPTY)))
    {
        char ch=0;
        ch=(char)UART_API(_UartGetChar());
        UART_API(_ClearRxInterruptSource)(UART_FLAG(_INTR_RX_NOT_EMPTY));
        
//#ifndef LOW_POWER_DISABLE        
        //wakeup_from_uart(ch);
//#endif  

        if(isprint(ch) || ch == '\r' || ch == '\n') {            
            //linkwan_serial_input(ch);   
#ifndef CONFIG_PRINT_ECHO_DISABLE            
            UART_API(_UartPutChar(ch));
#endif            
        }
    }
    /* UART done */
    if(0u != (UART_API(_GetTxInterruptSourceMasked)() & UART_FLAG(_INTR_TX_UART_DONE)))
    {
        UART_API(_ClearTxInterruptSource)(UART_FLAG(_INTR_TX_UART_DONE));
    }
}

int32_t hal_uart_init(uart_dev_t *uart)
{
    switch(uart->port)
    {
        case UART0:
        UART_API(_SetCustomInterruptHandler)(&UART_API(_customISR));
        UART_API(_Start)();
        break;
        
        default:
        break;
    }
    return 0;
}

int32_t hal_uart_send(uart_dev_t *uart, const void *data, uint32_t size, uint32_t timeout)
{
    uint8 *pdata = (uint8 *)data;
    
    (void) timeout;
    
    if((uart == NULL) || (data == NULL))
    {
        return -1;
    }
    
    switch(uart->port)
    {
        case UART0:
        UART_API(_SpiUartPutArray)(pdata, size);
        break;
        
        default:
        break;
    }    return 0;
}

int32_t hal_uart_recv(uart_dev_t *uart, void *data, uint32_t expect_size, uint32_t timeout)
{
    uint32 *pdata = (uint32 *)data;
    uint32 i = 0;
    
    (void)timeout;
    
    switch(uart->port)
    {
        case UART0:
        for(i = 0; i < expect_size; i++)
        {
            *pdata = UART_API(_UartGetChar());
        }
        break;
        
        default:
        break;
    }

    return 0;
}

int32_t hal_uart_finalize(uart_dev_t *uart)
{
    (void)uart;
    
    switch(uart->port)
    {
        case UART0:
        UART_API(_Stop());
        break;
        
        default:
        break;
    }
    
    return 0;
}

void uart_put_char(int c)
{
    hal_uart_send(&uart_0,&c,1,3000);
}
uint8 uart_pre_deepsleep(void)
{
    uint32_t div = (UART_1_SCBCLK_DIV_REG >> 8) + 1;
    uint32_t bitTime = div*UART_1_UART_OVS_FACTOR/(CYDEV_BCLK__HFCLK__HZ/1000000) + 1;
    uint8 bitsToWait = UART_1_UART_BITS_TO_WAIT;
    uint8 enterDeepSleep = 1u;

    UART_1_skipStart = bitTime<25?0:1;
    
    while (0 != (UART_1_SpiUartGetTxBufferSize() + UART_1_GET_TX_FIFO_SR_VALID)) {
        ;
    }
    if (0 == UART_1_SpiUartGetRxBufferSize()) {
        UART_1_Sleep();        
        while (0 != bitsToWait) {
            CyDelayUs(bitTime);
            --bitsToWait;
            if (0 != UART_1_GET_RX_FIFO_SR_VALID) {
                enterDeepSleep = 0;
                break;
            }
        }        
        if (0 != enterDeepSleep) {
            return 1;
        } else {
            return 0;
        }
    }

    return 0;
}
