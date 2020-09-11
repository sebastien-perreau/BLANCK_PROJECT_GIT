/* **************************************************** */
/* 		Interrupts routines for all peripherals			*/
/* **************************************************** */

#include "config.h"

void __ISR(_CHANGE_NOTICE_VECTOR, _IPLxAUTO(CHANGE_NOTICE_INTERRUPT_PRIORITY)) ChangeNoticeInterruptHandler(void)
{
    ports_interrupt_handler();
    irq_clr_flag(IRQ_CN);
}

void __ISR(_ADC_VECTOR, _IPLxAUTO(ADC10_INTERRUPT_PRIORITY)) Adc10InterruptHandler(void)
{
    adc10_interrupt_handler();
    irq_clr_flag(IRQ_AD1);
}

void __ISR(_TIMER_1_VECTOR, _IPLxAUTO(TIMER1_INTERRUPT_PRIORITY)) Timer1InterruptHandler(void)
{
    timer_interrupt_handler(TIMER1);
    TIMER1_CLR_FLAG();
}

void __ISR(_TIMER_2_VECTOR, _IPLxAUTO(TIMER2_INTERRUPT_PRIORITY)) Timer2InterruptHandler(void)
{
    timer_interrupt_handler(TIMER2);
    TIMER2_CLR_FLAG();
}

void __ISR(_TIMER_3_VECTOR, _IPLxAUTO(TIMER3_INTERRUPT_PRIORITY)) Timer3InterruptHandler(void)
{
    timer_interrupt_handler(TIMER3);
    TIMER3_CLR_FLAG();
}

void __ISR(_TIMER_4_VECTOR, _IPLxAUTO(TIMER4_INTERRUPT_PRIORITY)) Timer4InterruptHandler(void)
{
    timer_interrupt_handler(TIMER4);
    TIMER4_CLR_FLAG();
}

void __ISR(_TIMER_5_VECTOR, _IPLxAUTO(TIMER5_INTERRUPT_PRIORITY)) Timer5InterruptHandler(void)
{
    timer_interrupt_handler(TIMER5);
    TIMER5_CLR_FLAG();
}

void __ISR(_DMA_0_VECTOR, _IPLxAUTO(DMA0_INTERRUPT_PRIORITY)) Dma0Handler(void)
{
    dma_interrupt_handler(DMA0);
    irq_clr_flag(IRQ_DMA0);
}

void __ISR(_DMA_1_VECTOR, _IPLxAUTO(DMA1_INTERRUPT_PRIORITY)) Dma1Handler(void)
{
    dma_interrupt_handler(DMA1);
    irq_clr_flag(IRQ_DMA1);
}

void __ISR(_DMA_2_VECTOR, _IPLxAUTO(DMA2_INTERRUPT_PRIORITY)) Dma2Handler(void)
{
    dma_interrupt_handler(DMA2);
    irq_clr_flag(IRQ_DMA2);
}

void __ISR(_DMA_3_VECTOR, _IPLxAUTO(DMA3_INTERRUPT_PRIORITY)) Dma3Handler(void)
{
    dma_interrupt_handler(DMA3);
    irq_clr_flag(IRQ_DMA3);
}

void __ISR(_DMA_4_VECTOR, _IPLxAUTO(DMA4_INTERRUPT_PRIORITY)) Dma4Handler(void)
{
    dma_interrupt_handler(DMA4);
    irq_clr_flag(IRQ_DMA4);
}

void __ISR(_DMA_5_VECTOR, _IPLxAUTO(DMA5_INTERRUPT_PRIORITY)) Dma5Handler(void)
{
    dma_interrupt_handler(DMA5);
    irq_clr_flag(IRQ_DMA5);
}

void __ISR(_DMA_6_VECTOR, _IPLxAUTO(DMA6_INTERRUPT_PRIORITY)) Dma6Handler(void)
{
    dma_interrupt_handler(DMA6);
    irq_clr_flag(IRQ_DMA6);
}

void __ISR(_DMA_7_VECTOR, _IPLxAUTO(DMA7_INTERRUPT_PRIORITY)) Dma7Handler(void)
{
    dma_interrupt_handler(DMA7);
    irq_clr_flag(IRQ_DMA7);
}


void __ISR(_SPI_1_VECTOR, _IPLxAUTO(SPI1_INTERRUPT_PRIORITY)) SPI1Handler(void)
{
    if (irq_get_flag(IRQ_SPI1RX))
    {
        uint32_t data = SPI1BUF;
        spi_interrupt_handler(SPI1, IRQ_SPI_RX, data);
        irq_clr_flag(IRQ_SPI1RX);
    }
    else if (irq_get_flag(IRQ_SPI1E))
    {
        spi_interrupt_handler(SPI1, IRQ_SPI_FAULT, 0);
        irq_clr_flag(IRQ_SPI1E);
    }
    else if (irq_get_flag(IRQ_SPI1TX))
    {
        spi_interrupt_handler(SPI1, IRQ_SPI_TX, 0);
        irq_clr_flag(IRQ_SPI1TX);
    }
}

void __ISR(_I2C_1_VECTOR, _IPLxAUTO(I2C1_INTERRUPT_PRIORITY)) I2C1Handler(void)
{
    if (irq_get_flag(IRQ_I2C1M) > 0)
    {
        uint8_t data = 0;
        i2c_get_byte(I2C1, &data);
        i2c_interrupt_handler(I2C1, IRQ_I2C_MASTER, data);
        irq_clr_flag(IRQ_I2C1M);
    }
    else if (irq_get_flag(IRQ_I2C1B) > 0)
    {
        i2c_interrupt_handler(I2C1, IRQ_I2C_BUS_COLISION, 0);
        irq_clr_flag(IRQ_I2C1B);
    }
    else if (irq_get_flag(IRQ_I2C1S) > 0)
    {
        uint8_t data = 0;
        i2c_get_byte(I2C1, &data);
        i2c_interrupt_handler(I2C1, IRQ_I2C_SLAVE, data);
        irq_clr_flag(IRQ_I2C1S);
    }
}

void __ISR(_I2C_2_VECTOR, _IPLxAUTO(I2C2_INTERRUPT_PRIORITY)) I2C2Handler(void)
{
    if (irq_get_flag(IRQ_I2C2M) > 0)
    {
        uint8_t data = 0;
        i2c_get_byte(I2C2, &data);
        i2c_interrupt_handler(I2C2, IRQ_I2C_MASTER, data);
        irq_clr_flag(IRQ_I2C2M);
    }
    else if (irq_get_flag(IRQ_I2C2B) > 0)
    {
        i2c_interrupt_handler(I2C2, IRQ_I2C_BUS_COLISION, 0);
        irq_clr_flag(IRQ_I2C2B);
    }
    else if (irq_get_flag(IRQ_I2C2S) > 0)
    {
        uint8_t data = 0;
        i2c_get_byte(I2C2, &data);
        i2c_interrupt_handler(I2C2, IRQ_I2C_SLAVE, data);
        irq_clr_flag(IRQ_I2C2S);
    }
}

void __ISR(_I2C_3_VECTOR, _IPLxAUTO(UART1_I2C3_SPI3_INTERRUPT_PRIORITY)) I2C3_SPI3_UART1Handler(void)
{
    if (irq_get_flag(IRQ_U1) > 0)
    {
        if (irq_get_flag(IRQ_U1RX) > 0)
        {
            uint32_t data = U1RXREG;
            uart_interrupt_handler(UART1, IRQ_UART_RX, data);
            irq_clr_flag(IRQ_U1RX);
        }
        else if (irq_get_flag(IRQ_U1E) > 0)
        {
            uart_interrupt_handler(UART1, IRQ_UART_ERROR, 0);
            irq_clr_flag(IRQ_U1E);
        }
        else if (irq_get_flag(IRQ_U1TX) > 0)
        {
            uart_interrupt_handler(UART1, IRQ_UART_TX, 0);
            irq_clr_flag(IRQ_U1TX);
        }
    }
    else if (irq_get_flag(IRQ_SPI3) > 0)
    {
        if (irq_get_flag(IRQ_SPI3RX))
        {
            uint32_t data = SPI3BUF;
            spi_interrupt_handler(SPI3, IRQ_SPI_RX, data);
            irq_clr_flag(IRQ_SPI3RX);
        }
        else if (irq_get_flag(IRQ_SPI3E))
        {
            spi_interrupt_handler(SPI3, IRQ_SPI_FAULT, 0);
            irq_clr_flag(IRQ_SPI3E);
        }
        else if (irq_get_flag(IRQ_SPI3TX))
        {
            spi_interrupt_handler(SPI3, IRQ_SPI_TX, 0);
            irq_clr_flag(IRQ_SPI3TX);
        }
    }
    else if (irq_get_flag(IRQ_I2C3) > 0)
    {
        if (irq_get_flag(IRQ_I2C3M) > 0)
        {
            uint8_t data = 0;
            i2c_get_byte(I2C3, &data);
            i2c_interrupt_handler(I2C3, IRQ_I2C_MASTER, data);
            irq_clr_flag(IRQ_I2C3M);
        }
        else if (irq_get_flag(IRQ_I2C3B) > 0)
        {
            i2c_interrupt_handler(I2C3, IRQ_I2C_BUS_COLISION, 0);
            irq_clr_flag(IRQ_I2C3B);
        }
        else if (irq_get_flag(IRQ_I2C3S) > 0)
        {
            uint8_t data = 0;
            i2c_get_byte(I2C3, &data);
            i2c_interrupt_handler(I2C3, IRQ_I2C_SLAVE, data);
            irq_clr_flag(IRQ_I2C3S);
        }
    }
}

void __ISR(_I2C_4_VECTOR, _IPLxAUTO(UART3_I2C4_SPI2_INTERRUPT_PRIORITY)) I2C4_SPI2_UART3Handler(void)
{
    if (irq_get_flag(IRQ_U3) > 0)
    {
        if (irq_get_flag(IRQ_U3RX) > 0)
        {
            uint32_t data = U3RXREG;
            uart_interrupt_handler(UART3, IRQ_UART_RX, data);
            irq_clr_flag(IRQ_U3RX);
        }
        else if (irq_get_flag(IRQ_U3E) > 0)
        {
            uart_interrupt_handler(UART3, IRQ_UART_ERROR, 0);
            irq_clr_flag(IRQ_U3E);
        }
        else if (irq_get_flag(IRQ_U3TX) > 0)
        {
            uart_interrupt_handler(UART3, IRQ_UART_TX, 0);
            irq_clr_flag(IRQ_U3TX);
        }
    }
    else if (irq_get_flag(IRQ_SPI2) > 0)
    {
        if (irq_get_flag(IRQ_SPI2RX))
        {
            uint32_t data = SPI2BUF;
            spi_interrupt_handler(SPI2, IRQ_SPI_RX, data);
            irq_clr_flag(IRQ_SPI2RX);
        }
        else if (irq_get_flag(IRQ_SPI2E))
        {
            spi_interrupt_handler(SPI2, IRQ_SPI_FAULT, 0);
            irq_clr_flag(IRQ_SPI2E);
        }
        else if (irq_get_flag(IRQ_SPI2TX))
        {
            spi_interrupt_handler(SPI2, IRQ_SPI_TX, 0);
            irq_clr_flag(IRQ_SPI2TX);
        }
    }
    else if (irq_get_flag(IRQ_I2C4) > 0)
    {
        if (irq_get_flag(IRQ_I2C4M) > 0)
        {
            uint8_t data = 0;
            i2c_get_byte(I2C4, &data);
            i2c_interrupt_handler(I2C4, IRQ_I2C_MASTER, data);
            irq_clr_flag(IRQ_I2C4M);
        }
        else if (irq_get_flag(IRQ_I2C4B) > 0)
        {
            i2c_interrupt_handler(I2C4, IRQ_I2C_BUS_COLISION, 0);
            irq_clr_flag(IRQ_I2C4B);
        }
        else if (irq_get_flag(IRQ_I2C4S) > 0)
        {
            uint8_t data = 0;
            i2c_get_byte(I2C4, &data);
            i2c_interrupt_handler(I2C4, IRQ_I2C_SLAVE, data);
            irq_clr_flag(IRQ_I2C4S);
        }
    }
}

void __ISR(_I2C_5_VECTOR, _IPLxAUTO(UART2_I2C5_SPI4_INTERRUPT_PRIORITY)) I2C5_SPI4_UART2Handler(void)
{
    if (irq_get_flag(IRQ_U2) > 0)
    {
        if (irq_get_flag(IRQ_U2RX) > 0)
        {
            uint32_t data = U2RXREG;
            uart_interrupt_handler(UART2, IRQ_UART_RX, data);
            irq_clr_flag(IRQ_U2RX);
        }
        else if (irq_get_flag(IRQ_U2E) > 0)
        {
            uart_interrupt_handler(UART2, IRQ_UART_ERROR, 0);
            irq_clr_flag(IRQ_U2E);
        }
        else if (irq_get_flag(IRQ_U2TX) > 0)
        {
            uart_interrupt_handler(UART2, IRQ_UART_TX, 0);
            irq_clr_flag(IRQ_U2TX);
        }
    }
    else if (irq_get_flag(IRQ_SPI4) > 0)
    {
        if (irq_get_flag(IRQ_SPI4RX))
        {
            uint32_t data = SPI4BUF;
            spi_interrupt_handler(SPI4, IRQ_SPI_RX, data);
            irq_clr_flag(IRQ_SPI4RX);
        }
        else if (irq_get_flag(IRQ_SPI4E))
        {
            spi_interrupt_handler(SPI4, IRQ_SPI_FAULT, 0);
            irq_clr_flag(IRQ_SPI4E);
        }
        else if (irq_get_flag(IRQ_SPI4TX))
        {
            spi_interrupt_handler(SPI4, IRQ_SPI_TX, 0);
            irq_clr_flag(IRQ_SPI4TX);
        }
    }
    else if (irq_get_flag(IRQ_I2C5) > 0)
    {
        if (irq_get_flag(IRQ_I2C5M) > 0)
        {
            uint8_t data = 0;
            i2c_get_byte(I2C5, &data);
            i2c_interrupt_handler(I2C5, IRQ_I2C_MASTER, data);
            irq_clr_flag(IRQ_I2C5M);
        }
        else if (irq_get_flag(IRQ_I2C5B) > 0)
        {
            i2c_interrupt_handler(I2C5, IRQ_I2C_BUS_COLISION, 0);
            irq_clr_flag(IRQ_I2C5B);
        }
        else if (irq_get_flag(IRQ_I2C5S) > 0)
        {
            uint8_t data = 0;
            i2c_get_byte(I2C5, &data);
            i2c_interrupt_handler(I2C5, IRQ_I2C_SLAVE, data);
            irq_clr_flag(IRQ_I2C5S);
        }
    }
}

void __ISR(_UART_4_VECTOR, _IPLxAUTO(UART4_INTERRUPT_PRIORITY)) UART4Handler(void)
{
    if (irq_get_flag(IRQ_U4RX) > 0)
    {
        uint32_t v_read = U4RXREG;
        uart_interrupt_handler(UART4, IRQ_UART_RX, v_read);
        irq_clr_flag(IRQ_U4RX);
    }
    else if (irq_get_flag(IRQ_U4E) > 0)
    {
        uart_interrupt_handler(UART4, IRQ_UART_ERROR, 0);
        irq_clr_flag(IRQ_U4E);
    }
    else if (irq_get_flag(IRQ_U4TX) > 0)
    {
        uart_interrupt_handler(UART4, IRQ_UART_TX, 0);
        irq_clr_flag(IRQ_U4TX);
    }
}

void __ISR(_UART_5_VECTOR, _IPLxAUTO(UART5_INTERRUPT_PRIORITY)) UART5Handler(void)
{
    if (irq_get_flag(IRQ_U5RX) > 0)
    {
        uint32_t v_read = U5RXREG;
        uart_interrupt_handler(UART5, IRQ_UART_RX, v_read);
        irq_clr_flag(IRQ_U5RX);
    }
    else if (irq_get_flag(IRQ_U5E) > 0)
    {
        uart_interrupt_handler(UART5, IRQ_UART_ERROR, 0);
        irq_clr_flag(IRQ_U5E);
    }
    else if (irq_get_flag(IRQ_U5TX) > 0)
    {
        uart_interrupt_handler(UART5, IRQ_UART_TX, 0);
        irq_clr_flag(IRQ_U5TX);
    }
}

void __ISR(_UART_6_VECTOR, _IPLxAUTO(UART6_INTERRUPT_PRIORITY)) UART6Handler(void)
{
    if (irq_get_flag(IRQ_U6RX) > 0)
    {
        uint32_t v_read = U6RXREG;
        uart_interrupt_handler(UART6, IRQ_UART_RX, v_read);
        irq_clr_flag(IRQ_U6RX);
    }
    else if (irq_get_flag(IRQ_U6E) > 0)
    {
        uart_interrupt_handler(UART6, IRQ_UART_ERROR, 0);
        irq_clr_flag(IRQ_U6E);
    }
    else if (irq_get_flag(IRQ_U6TX) > 0)
    {
        uart_interrupt_handler(UART6, IRQ_UART_TX, 0);
        irq_clr_flag(IRQ_U6TX);
    }
}

/*
//	CAN1 handler
void __ISR(_CAN_1_VECTOR, _IPLxAUTO(CAN1_INTERRUPT_PRIORITY)) CAN1InterruptHandler(void)
{
    CANTaskRx(CAN1);
    irq_clr_flag(IRQ_CAN1);
}

//	CAN2 handler
void __ISR(_CAN_2_VECTOR, _IPLxAUTO(CAN2_INTERRUPT_PRIORITY)) CAN2InterruptHandler(void)
{
    CANTaskRx(CAN2);
    irq_clr_flag(IRQ_CAN2);
}
*/
