 #include "bit_settings.h"
#include "config.h"

BLE_PICKIT_DEF(ble_pickit, "SPC", BLE_SECURITY_DISABLED);

GROVE_MOTOR_DEF(grove_motor, I2C2, 0x33);
BUS_MANAGEMENT_DEF(bm_i2c2, &grove_motor.i2c_params.bus_management_params);
SWITCH_DEF(sw1, SWITCH2, ACTIVE_LOW);

static void acquisitions_spc_tasks(uint16_t *p_an15);

bool grove_motor_task(uint16_t next_position)
{
    // 1 tour = 200 full steps
    static uint8_t rpm = 120;   // 120 tours par minute -> 2 tours par second -> 400 pas par second -> 1 pas = 2,5ms
    static const uint64_t tick_2_5ms = 200000;
    
    static state_machine_t sm = {0};
    static uint64_t time_stepper_drive = 0;
    static uint16_t curr_position = 2000;
    
    bool ret = 0;
    
    switch (sm.index)
    {
        case 0:
            mUpdateTick(sm.tick);
            sm.index++;
            break;
            
        case 1:
            if (mTickCompare(sm.tick) >= TICK_100MS)
            {
                e_grove_motor_stepper_run(&grove_motor, GROVE_MOTOR_FULL_STEP, rpm, -200*20);
                sm.index++;
                mUpdateTick(sm.tick);
            }
            break;
            
        case 2:
            
            if (mTickCompare(sm.tick) > (TICK_1S*10))
            {
                e_grove_motor_stepper_run(&grove_motor, GROVE_MOTOR_FULL_STEP, rpm, 2000);
                sm.index++;
                mUpdateTick(sm.tick);
            }
            break;
            
        case 3:
            
            if (mTickCompare(sm.tick) > (TICK_1S*5))
            {
                sm.index++;
                mUpdateTick(sm.tick);
            }
            break;
            
        case 4:
            
            if (next_position != curr_position)
            {
                e_grove_motor_stepper_run(&grove_motor, GROVE_MOTOR_FULL_STEP, rpm, (next_position - curr_position));
                time_stepper_drive = abs((next_position - curr_position)) * tick_2_5ms;
                curr_position = next_position;
                sm.index++;
                mUpdateTick(sm.tick);
            }
            else
            {
                ret = 1;
            }
            break;
            
        case 5:
            
            if (mTickCompare(sm.tick) > time_stepper_drive)
            {
                sm.index = 4;
            }
            break;
    }
    
    return ret;
}

int main(void)
{        
    cfg_pic32();
    cfg_ports(SWITCH1_PULLUP_ENABLE | SWITCH2_PULLUP_ENABLE | SWITCH3_PULLUP_ENABLE, 0, NULL);
    cfg_timers();    
    cfg_adc10(ADC_MUX);
    
    // Others initializations
    pwm_init(PWM_NONE, 40000, 250000);
    ble_init(UART4, UART_BAUDRATE_1M, &ble_pickit);
    log_init(UART1, UART_BAUDRATE_2M);
    m_init_hardware_picadapter();
    mUpdateLedStatusD2(OFF);
    mUpdateLedStatusD3(BLINK);          
                        
    while(1)
    {     
        static uint16_t position = 2000;    // [0 .. 4000]
        
        fu_switch(&sw1);
        
        switch (sw1.indice)
        {
            case 0:
                position = 2000;
                break;
                
            case 1:
                position = 3000;
                break;

            case 2:
                position = 500;
                break;

            default:
                sw1.indice = 0;
                break;
        }
        
        grove_motor_task(position);
        
        fu_bus_management_task(&bm_i2c2);
        e_grove_motor_tb6612fng_deamon(&grove_motor);
                
        ble_stack_tasks();
               
        acquisitions_spc_tasks(NULL);
        
    }
}

/*******************************************************************************
 * Function:
 *      static void acquisitions_spc_tasks(uint16_t *p_an15)
 * 
 * Overview:
 *      This function must always be placed in the while main loop. It manages 
 *      all the acquisitions (ntc, currents, voltages, an15 & CPU load). 
 *      These features are only available for the PICAdapter development board.
 * 
 * Input:
 *      *p_an15:    A pointer which returns the AN15 value (if not NULL).
 * 
 * Output:
 *      none
 * 
 ******************************************************************************/
static void acquisitions_spc_tasks(uint16_t *p_an15)
{    
    NTC_DEF(ntc, AN15, 25, 10000, 3380, 10000); // .temperature : -40.0 .. 200.0
    AVERAGE_DEF(current, AN15, 10, TICK_1MS);   // .average: (e.i) 17.48
    AVERAGE_DEF(voltage, AN15, 10, TICK_1MS);   // .average: (e.i) 12.56
    AVERAGE_DEF(an15, AN15, 1, TICK_1MS);       // .average: 0 .. 1023
    
    static uint8_t index_tab_speed = 0;
    static uint64_t tab_speed[20] = {0};
    static uint64_t sum_tab_speed = 0;
    static uint64_t tick_speed = 0;
    
    static uint8_t mux_sel = 0;
    static uint64_t tick_leds_status = 0;
        
    /*
     * LED status
     */
    if (mTickCompare(tick_leds_status) >= TICK_200MS) 
    { 
        tick_leds_status = mGetTick(); 
        INV_BIT(__led_status, 7);
    }
    mLatIO(LED2) = (((__led_status >> 2) & 0x01) & ((~__led_status >> 3) & 0x01)) | (((~__led_status >> 2) & 0x01) & ((__led_status >> 3) & 0x01) & ((~__led_status >> 7) & 0x01)) | (((__led_status >> 2) & 0x01) & ((__led_status >> 3) & 0x01) & ((__led_status >> 7) & 0x01));
    mLatIO(LED3) = (((__led_status >> 0) & 0x01) & ((~__led_status >> 1) & 0x01)) | (((~__led_status >> 0) & 0x01) & ((__led_status >> 1) & 0x01) & ((~__led_status >> 7) & 0x01)) | (((__led_status >> 0) & 0x01) & ((__led_status >> 1) & 0x01) & ((__led_status >> 7) & 0x01));
                                        
    /*
     * PICAdapter acquisitions
     */
    switch(mux_sel)
    {
        case 0:     // NTC (�C)
            if (fu_adc_ntc(&ntc) == NTC_SUCCESS)
            {
                __temperature = fu_get_integer_value(ntc.temperature * 10.0);
                current.tick = mGetTick();
                mux_sel = 1;
            }
            break;
        case 1:     // CURRENT (A)
            if (fu_adc_average(&current))
            {
                current.average = (float) (current.average * 5.04 / 10000.0);    // ((((tabSumCurrent / 10) * 3.3) / 1023.0) / 6.4);
                __current = fu_get_integer_value(current.average * 1000.0);
                voltage.tick = mGetTick();
                mux_sel = 2;
            }
            break;
        case 2:     // VOLTAGE (V)
            if (fu_adc_average(&voltage))
            {
                voltage.average = (float) (voltage.average * 18.391 / 1000.0);    // ((((tabSumVoltage / 10) * 3.3) / 1023.0) / 0.1754);
                __voltage = fu_get_integer_value(voltage.average * 10.0);
                an15.tick = mGetTick();
                mux_sel = 3;
            }
            break;
        case 3:     // AN15
            if (fu_adc_average(&an15))
            {
                if (p_an15 != NULL)
                {
                    *p_an15 = (uint16_t) (an15.average);
                }
                ntc.average.tick = mGetTick();
                mux_sel = 0;
            }
            break;
    }

    mLatIO(MUX0) = (mux_sel >> 0);
    mLatIO(MUX1) = (mux_sel >> 1);
    
    sum_tab_speed -= tab_speed[index_tab_speed];
    tab_speed[index_tab_speed] = mTickCompare(tick_speed);
    tick_speed = mGetTick();
    sum_tab_speed += tab_speed[index_tab_speed];           
    __speed = (uint32_t) (sum_tab_speed / 20 / TICK_1US);
    if(++index_tab_speed >= 20)
    {
        index_tab_speed = 0;
    }
}
