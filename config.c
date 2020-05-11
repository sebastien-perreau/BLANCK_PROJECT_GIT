#include "config.h"

const IRQ_DATA_PRIORITY IrqDataPriorityTab[] =
{
    { CHANGE_NOTICE_INTERRUPT_PRIORITY },
    
    { ADC10_INTERRUPT_PRIORITY },
    
    { TIMER1_INTERRUPT_PRIORITY },
    { TIMER2_INTERRUPT_PRIORITY },
    { TIMER3_INTERRUPT_PRIORITY },
    { TIMER4_INTERRUPT_PRIORITY },
    { TIMER5_INTERRUPT_PRIORITY },
    
    { DMA0_INTERRUPT_PRIORITY },
    { DMA1_INTERRUPT_PRIORITY },
    { DMA2_INTERRUPT_PRIORITY },
    { DMA3_INTERRUPT_PRIORITY },
    { DMA4_INTERRUPT_PRIORITY },
    { DMA5_INTERRUPT_PRIORITY },
    { DMA6_INTERRUPT_PRIORITY },
    { DMA7_INTERRUPT_PRIORITY },
    
    { UART1_INTERRUPT_PRIORITY },
    { UART2_INTERRUPT_PRIORITY },
    { UART3_INTERRUPT_PRIORITY },
    { UART4_INTERRUPT_PRIORITY },
    { UART5_INTERRUPT_PRIORITY },
    { UART6_INTERRUPT_PRIORITY },
    
    { SPI1_INTERRUPT_PRIORITY },
    { SPI2_INTERRUPT_PRIORITY },
    { SPI3_INTERRUPT_PRIORITY },
    { SPI4_INTERRUPT_PRIORITY },
    
    { I2C1_INTERRUPT_PRIORITY },
    { I2C2_INTERRUPT_PRIORITY },
    { I2C3_INTERRUPT_PRIORITY },
    { I2C4_INTERRUPT_PRIORITY },
    { I2C5_INTERRUPT_PRIORITY },
    
    { CAN1_INTERRUPT_PRIORITY },
    { CAN2_INTERRUPT_PRIORITY }
};

void cfg_pic32(void)
{
    system_config_performance(PERIPHERAL_FREQ);
    JTAGPortEnable(DISABLE);    
    irq_enable_system_multi_vectore_interrupts();   
    irq_link_data_priority(IrqDataPriorityTab);
}

void cfg_ports(uint32_t cn_pull_up, uint32_t cn_pins_enable, ports_event_handler_t evt_handler)
{
    ports_reset_all_pins_input();
    ports_change_notice_init(cn_pull_up, cn_pins_enable, evt_handler);    
}

void cfg_timers(void)
{
    // Configure interrupt for timer
    irq_init(IRQ_T1, IRQ_DISABLED, irq_timer_priority(TIMER1));
    // Initialization TIMER 1 - DISABLE INTERRUPTION (ONLY USED FOR GET_TICK).
    TIMER_REGISTERS * timerRegister = (TIMER_REGISTERS *)_TMR1_BASE_ADDRESS;
    timerRegister->TCON    = 0x0000;
    timerRegister->TMR     = 0x0000;
    timerRegister->PR      = 0xFFFF;
    timerRegister->TCON    = TMR_ON;
    // Timer 2 & 3 are used for output compare module (see 'cfg_pwm()') - Disable interrupt and enable timer module
    // Timer 4 & 5 are free to use    
}

void cfg_adc10(uint32_t analog_inputs)
{
    // All ports B as digital (AN0 to AN15 on port B)
    AD1PCFGSET = ALL;
    if (analog_inputs > 0)
    {
        adc10_init(analog_inputs, ADC10_VDD_VSS, NULL);
    }
}
