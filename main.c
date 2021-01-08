 #include "bit_settings.h"
#include "config.h"

BLE_PICKIT_DEF(ble_pickit, "SPC", BLE_SECURITY_DISABLED);

static void acquisitions_spc_tasks(uint16_t *p_an15);

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
        case 0:     // NTC (°C)
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
