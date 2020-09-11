#include "bit_settings.h"
#include "config.h"

ACQUISITIONS_DEF(acquisitions);
BLE_PICKIT_DEF(ble_params, "Home box");


SOFTWARE_PWM_DEF(spwm, TIMER5, SOFTWARE_PWM_FREQ_200HZ, SOFTWARE_PWM_RESO_1, __PB12);
LED_DEF(stairs_led, &spwm.pwm[0], OFF, 100, TICK_10MS, TICK_5MS);
AVERAGE_DEF(stairs_sensor_top, AN14, 30, TICK_1MS);
AVERAGE_DEF(stairs_sensor_bottom, AN13, 30, TICK_1MS);
SWITCH_DEF(sw_rolling_shutters_open, SWITCH1, ACTIVE_LOW);
SWITCH_DEF(sw_rolling_shutters_close, SWITCH2, ACTIVE_LOW);
static uint64_t tick_led = (TICK_1S * 60);

int main(void)
{        
    cfg_pic32();
    cfg_ports(SWITCH1_PULLUP_ENABLE | SWITCH2_PULLUP_ENABLE | SWITCH3_PULLUP_ENABLE, 0, NULL);
    cfg_timers();    
    cfg_adc10(ADC_MUX | AN14 | AN13);
    
    // Others initializations
    pwm_init(PWM_NONE, 40000, 250000);
    ble_init(UART4, UART_BAUDRATE_1M, &ble_params);
    log_init(UART1, UART_BAUDRATE_2M);
    m_init_hardware_picadapter();
    mUpdateLedStatusD2(OFF);
    mUpdateLedStatusD3(BLINK);          
    
    software_pwm_init(&spwm);    
    
    while(1)
    {
        
        if (((uint16_t) stairs_sensor_bottom.average < 100) && ((uint16_t) stairs_sensor_top.average < 100))
        {
            mUpdateLedStatusD2(OFF);
        }
        else if (((uint16_t) stairs_sensor_bottom.average > 500) || ((uint16_t) stairs_sensor_top.average > 500))
        {
            mUpdateLedStatusD2(ON);
            mUpdateTick(tick_led);
        }
        
        if (mTickCompare(tick_led) < (TICK_1S * 17))
        {
            stairs_led.enable = ON;
        }
        else
        {
            stairs_led.enable = OFF;
        }               
        
        fu_adc_average(&stairs_sensor_top);
        fu_adc_average(&stairs_sensor_bottom);
        fu_led(&stairs_led);
        fu_switch(&sw_rolling_shutters_open);
        fu_switch(&sw_rolling_shutters_close);        
        
        ble_stack_tasks();
               
        background_tasks(&acquisitions);
        
    }
}
