#include "bit_settings.h"
#include "config.h"

ACQUISITIONS_DEF(acquisitions);
BLE_DEF(ble_params, "Prototypes");

int main(void)
{        
    cfg_pic32();
    cfg_ports(SWITCH1_PULLUP_ENABLE | SWITCH2_PULLUP_ENABLE | SWITCH3_PULLUP_ENABLE, 0, NULL);
    cfg_timers();    
    cfg_adc10(ADC_MUX);
    
    // Others initializations
    pwm_init(PWM_NONE, 40000, 250000);
    ble_init(UART4, UART_BAUDRATE_1M, &ble_params);
    log_init(UART1, UART_BAUDRATE_2M);
    m_init_hardware_picadapter();
    mUpdateLedStatusD2(OFF);
    mUpdateLedStatusD3(BLINK);          
    
    while(1)
    {      
        // test
        ble_stack_tasks();
               
        background_tasks(&acquisitions);
        
    }
}
