#ifndef __DEF_CONFIG
#define __DEF_CONFIG

/** For degguging set the optimization to 0 in order to have all breakpoint **/

#include "../PLIB2_GIT/PLIB2.h"

#if (PERIPHERAL_FREQ != 80000000) || (PERIPHERAL_FREQ != 48000000)
//#error "PLIB is only compatible with PERIPHERAL_FREQ equal to 80 MHz or 48 MHz"
//#error "SYS_FREQ should be equal to 8000000L: FPLLMUL = MUL_20, FPLLODIV = 1
//#error "SYS_FREQ should be equal to 4800000L: FPLLMUL = MUL_24, FPLLODIV = 2
//#error "Do not forget to update bit_settings and comment these errors if you want to change the frequence"
#endif

void cfg_pic32(void);
void cfg_ports(uint32_t cn_pull_up, uint32_t cn_pins_enable, ports_event_handler_t evt_handler);
void cfg_timers(void);
void cfg_adc10(uint32_t analog_inputs);

// -----------------------------------------------
// IRQ priority 0 (disable) 1..7 / sub priority 0..3

#define CHANGE_NOTICE_INTERRUPT_PRIORITY    3, 1

#define ADC10_INTERRUPT_PRIORITY            3, 1

#define TIMER1_INTERRUPT_PRIORITY           7, 3
#define TIMER2_INTERRUPT_PRIORITY           3, 1
#define TIMER3_INTERRUPT_PRIORITY           4, 1
#define TIMER4_INTERRUPT_PRIORITY           3, 1
#define TIMER5_INTERRUPT_PRIORITY           6, 3

#define DMA0_INTERRUPT_PRIORITY             5, 3
#define DMA1_INTERRUPT_PRIORITY             5, 3
#define DMA2_INTERRUPT_PRIORITY             5, 3
#define DMA3_INTERRUPT_PRIORITY             5, 3
#define DMA4_INTERRUPT_PRIORITY             3, 1
#define DMA5_INTERRUPT_PRIORITY             3, 1
#define DMA6_INTERRUPT_PRIORITY             3, 1
#define DMA7_INTERRUPT_PRIORITY             3, 1

#define UART1_I2C3_SPI3_INTERRUPT_PRIORITY  3, 1
#define UART3_I2C4_SPI2_INTERRUPT_PRIORITY  3, 1
#define UART2_I2C5_SPI4_INTERRUPT_PRIORITY  3, 1
#define UART1_INTERRUPT_PRIORITY            UART1_I2C3_SPI3_INTERRUPT_PRIORITY
#define UART2_INTERRUPT_PRIORITY            UART2_I2C5_SPI4_INTERRUPT_PRIORITY
#define UART3_INTERRUPT_PRIORITY            UART3_I2C4_SPI2_INTERRUPT_PRIORITY
#define UART4_INTERRUPT_PRIORITY            5, 1
#define UART5_INTERRUPT_PRIORITY            3, 1
#define UART6_INTERRUPT_PRIORITY            3, 1
#define SPI1_INTERRUPT_PRIORITY             3, 1
#define SPI2_INTERRUPT_PRIORITY             UART3_I2C4_SPI2_INTERRUPT_PRIORITY
#define SPI3_INTERRUPT_PRIORITY             UART1_I2C3_SPI3_INTERRUPT_PRIORITY
#define SPI4_INTERRUPT_PRIORITY             UART2_I2C5_SPI4_INTERRUPT_PRIORITY
#define I2C1_INTERRUPT_PRIORITY             3, 1
#define I2C2_INTERRUPT_PRIORITY             3, 1
#define I2C3_INTERRUPT_PRIORITY             UART1_I2C3_SPI3_INTERRUPT_PRIORITY
#define I2C4_INTERRUPT_PRIORITY             UART3_I2C4_SPI2_INTERRUPT_PRIORITY
#define I2C5_INTERRUPT_PRIORITY             UART2_I2C5_SPI4_INTERRUPT_PRIORITY

#define CAN1_INTERRUPT_PRIORITY             4, 0
#define CAN2_INTERRUPT_PRIORITY             4, 0

#define _IPLxAUTO(a)                        _IPLxAUTO2(a)
#define _IPLxAUTO2(a, b)                     IPL ## a ## AUTO

#endif
