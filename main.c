#include "bit_settings.h"
#include "config.h"

ACQUISITIONS_DEF(acquisitions);
BLE_PICKIT_DEF(ble_pickit, "Home box", BLE_SECURITY_DISABLED);

SOFTWARE_PWM_DEF(spwm, TIMER5, SOFTWARE_PWM_FREQ_200HZ, SOFTWARE_PWM_RESO_1, __PB12);
LED_DEF(stairs_led, &spwm.pwm[0], OFF, 100, TICK_10MS, TICK_5MS);
AVERAGE_DEF(stairs_sensor_top, AN14, 30, TICK_1MS);
AVERAGE_DEF(stairs_sensor_bottom, AN13, 30, TICK_1MS);
SWITCH_DEF(sw_rolling_shutters_open, SWITCH1, ACTIVE_LOW);
SWITCH_DEF(sw_rolling_shutters_close, SWITCH2, ACTIVE_LOW);
static uint64_t tick_led = (TICK_1S * 60);

static uint8_t rolling_shutters_previous_states = 0;
static uint8_t rolling_shutter_kitchen_state = 0;
static uint8_t rolling_shutter_dining_room_state = 0;
static uint8_t rolling_shutter_living_room_state = 0;

uint32_t secure_validation(uint8_t * public_key);

int main(void)
{        
    cfg_pic32();
    cfg_ports(SWITCH1_PULLUP_ENABLE | SWITCH2_PULLUP_ENABLE | SWITCH3_PULLUP_ENABLE, 0, NULL);
    cfg_timers();    
    cfg_adc10(ADC_MUX | AN14 | AN13);
    
    // Others initializations
    pwm_init(PWM_NONE, 40000, 250000);
    ble_init(UART4, UART_BAUDRATE_1M, &ble_pickit);
    log_init(UART1, UART_BAUDRATE_2M);
    m_init_hardware_picadapter();
    mUpdateLedStatusD2(OFF);
    mUpdateLedStatusD3(BLINK);          
    
    software_pwm_init(&spwm);
    
    mInitIOAsOutput(__PB10);
    mInitIOAsOutput(__PB11);
    
    mInitIOAsOutput(__PB8);
    mInitIOAsOutput(__PB9);
    
    mInitIOAsOutput(__PB4);
    mInitIOAsOutput(__PB5);
    
    while(1)
    {
        static uint64_t tick_timeout = 0;
        
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
        
        mLatIO(__PB10) = GET_BIT(rolling_shutter_kitchen_state, 0);
        mLatIO(__PB11) = GET_BIT(rolling_shutter_kitchen_state, 1);
        
        mLatIO(__PB8) = GET_BIT(rolling_shutter_dining_room_state, 0);
        mLatIO(__PB9) = GET_BIT(rolling_shutter_dining_room_state, 1);
        
        mLatIO(__PB4) = GET_BIT(rolling_shutter_living_room_state, 0);
        mLatIO(__PB5) = GET_BIT(rolling_shutter_living_room_state, 1);
        
        if (ble_pickit.app_buffer.in_is_updated)
        {
            ble_pickit.app_buffer.in_is_updated = false;
            
            if (ble_pickit.app_buffer.in_length == 3)
            {
                mUpdateTick(tick_timeout);
                if ((ble_pickit.app_buffer.in_data[0] >= 0) && (ble_pickit.app_buffer.in_data[0] <= 2))
                {
                    rolling_shutter_kitchen_state = ble_pickit.app_buffer.in_data[0];
                }
                if ((ble_pickit.app_buffer.in_data[1] >= 0) && (ble_pickit.app_buffer.in_data[1] <= 2))
                {
                    rolling_shutter_dining_room_state = ble_pickit.app_buffer.in_data[1];
                }
                if ((ble_pickit.app_buffer.in_data[2] >= 0) && (ble_pickit.app_buffer.in_data[2] <= 2))
                {
                    rolling_shutter_living_room_state = ble_pickit.app_buffer.in_data[2];
                }
            }     
        }  
        else if (mTickCompare(tick_timeout) >= (TICK_1S * 30))
        {
            rolling_shutter_kitchen_state = 0;
            rolling_shutter_dining_room_state = 0;
            rolling_shutter_living_room_state = 0;
        }
                
        if (rolling_shutters_previous_states != ((rolling_shutter_kitchen_state << 0) | (rolling_shutter_dining_room_state << 2) | (rolling_shutter_living_room_state << 4)))
        {
            rolling_shutters_previous_states = (rolling_shutter_kitchen_state << 0) | (rolling_shutter_dining_room_state << 2) | (rolling_shutter_living_room_state << 4);
            ble_pickit.app_buffer.out_length = 3;
            ble_pickit.app_buffer.out_data[0] = rolling_shutter_kitchen_state;
            ble_pickit.app_buffer.out_data[1] = rolling_shutter_dining_room_state;
            ble_pickit.app_buffer.out_data[2] = rolling_shutter_living_room_state;
            ble_pickit.flags.notif_app_buffer = 1;
            
            LOG("rolling shutters states: %x", rolling_shutters_previous_states);
        }
        
        ble_stack_tasks();
               
        background_tasks(&acquisitions);
        
    }
}
