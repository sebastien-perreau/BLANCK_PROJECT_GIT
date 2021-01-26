#include "bit_settings.h"
#include "config.h"

BLE_PICKIT_DEF(ble_pickit, "SPC", BLE_SECURITY_DISABLED);

static void acquisitions_spc_tasks(uint16_t *p_an15);

#define CS_PIN      __PB8
#define RESX_PIN    __PB9

#define NUMBER_OF_RGB_LEDS  42
#define NUMBER_OF_LINES     63
#define LED_RESOLUTION      0       // 0: 8 bits / 1: 16 bits

#define SCAN1               0
#define SCAN2               (NUMBER_OF_RGB_LEDS * 3)
#define SCAN3               (NUMBER_OF_RGB_LEDS * 6)

#define DATA1               2   // 3 - 1 = 1 x 3 - 1
#define DATA2               1   // 3 - 2 = 1 x 3 - 2
#define DATA3               0   // 3 - 3 = 1 x 3 - 3
#define DATA4               5   // 6 - 1 = 2 x 3 - 1
#define DATA5               4   // 6 - 2 = 2 x 3 - 2
#define DATA6               3   // 6 - 3 = 2 x 3 - 3

#define DATA(n)             ((n-1)/3)*3 + (2 - ((n-1)%3))
#define SCAN(n)             (NUMBER_OF_RGB_LEDS * 3 * (n-1))
#define DATA_SCAN(x, y)     (DATA(x) + SCAN(y))

static dma_module_type_t dma_id;
static uint8_t dummy_byte;

static _io_t cs_pin = {2, 8};
static _io_t resx_pin = {2, 9};

static uint8_t buffer[2 + NUMBER_OF_RGB_LEDS*3*NUMBER_OF_LINES] = {0};
static dma_channel_transfer_t dma_ch_transfer_tx = {buffer, NULL, sizeof(buffer), 1, 1, 0x0000};

static uint16_t test[] = {DATA_SCAN(1, 1), DATA_SCAN(2, 1), DATA_SCAN(3, 3), DATA_SCAN(4, 3)};
static uint8_t i, j;

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
    
    
    buffer[0] = 0x01;
    buffer[1] = 0x2c;
    // CS pin
    ports_reset_pin_output(cs_pin);
    ports_set_bit(cs_pin);
    
    // RESX pin
    ports_reset_pin_output(resx_pin);
    ports_set_bit(resx_pin);

    spi_init(SPI1, NULL, IRQ_NONE, 20000000, SPI_STD_MASTER_CONFIG);
    dma_id = dma_init(NULL, DMA_CONT_PRIO_3, DMA_INT_BLOCK_TRANSFER_DONE, DMA_EVT_START_TRANSFER_ON_IRQ, spi_get_tx_irq(SPI1), 0xff);
        
    // XM11921 hardware reset
    ports_set_bit(resx_pin);
    Delay_ms(10);
    ports_clr_bit(resx_pin);
    Delay_ms(10);
    ports_set_bit(resx_pin);
    Delay_ms(200);
    
    ports_clr_bit(cs_pin);
    while(spi_write_and_read_8(SPI1, 0x00, &dummy_byte));
    while(spi_write_and_read_8(SPI1, 0x00, &dummy_byte));
    while(spi_write_and_read_8(SPI1, 0x00, &dummy_byte));
    ports_set_bit(cs_pin);
    ports_clr_bit(cs_pin);
    while(spi_write_and_read_8(SPI1, 0x00, &dummy_byte));
    while(spi_write_and_read_8(SPI1, 0xff, &dummy_byte));
    while(spi_write_and_read_8(SPI1, 0x70, &dummy_byte));
    while(spi_write_and_read_8(SPI1, 0x66, &dummy_byte));
    while(spi_write_and_read_8(SPI1, 0x01, &dummy_byte));
    ports_set_bit(cs_pin);
    ports_clr_bit(cs_pin);
    while(spi_write_and_read_8(SPI1, 0x00, &dummy_byte));
    while(spi_write_and_read_8(SPI1, 0x00, &dummy_byte));
    while(spi_write_and_read_8(SPI1, 0x80, &dummy_byte));
    ports_set_bit(cs_pin);
    ports_clr_bit(cs_pin);
    while(spi_write_and_read_8(SPI1, 0x00, &dummy_byte));
    while(spi_write_and_read_8(SPI1, 0xff, &dummy_byte));
    while(spi_write_and_read_8(SPI1, 0x70, &dummy_byte));
    while(spi_write_and_read_8(SPI1, 0x66, &dummy_byte));
    ports_set_bit(cs_pin);
    ports_clr_bit(cs_pin);
    while(spi_write_and_read_8(SPI1, 0x00, &dummy_byte));
    while(spi_write_and_read_8(SPI1, 0x00, &dummy_byte));
    while(spi_write_and_read_8(SPI1, 0xa1, &dummy_byte));
    ports_set_bit(cs_pin);
    ports_clr_bit(cs_pin);
    while(spi_write_and_read_8(SPI1, 0x00, &dummy_byte));
    while(spi_write_and_read_8(SPI1, 0xc0, &dummy_byte));
    while(spi_write_and_read_8(SPI1, NUMBER_OF_RGB_LEDS, &dummy_byte));   // Resolution 60 x 60
    while(spi_write_and_read_8(SPI1, NUMBER_OF_LINES, &dummy_byte));
    while(spi_write_and_read_8(SPI1, 0x10, &dummy_byte));
    ports_set_bit(cs_pin);
    ports_clr_bit(cs_pin);
    while(spi_write_and_read_8(SPI1, 0x00, &dummy_byte));
    while(spi_write_and_read_8(SPI1, 0x3a, &dummy_byte));
    while(spi_write_and_read_8(SPI1, LED_RESOLUTION, &dummy_byte));   // 1: 16 bits PWM / 0: 8 bits PWM
    ports_set_bit(cs_pin);
    
    Delay_ms(20);
    ports_clr_bit(cs_pin);
    while(spi_write_and_read_8(SPI1, 0x00, &dummy_byte));
    while(spi_write_and_read_8(SPI1, 0x11, &dummy_byte));
    ports_set_bit(cs_pin);
    Delay_ms(200);
    ports_clr_bit(cs_pin);
    while(spi_write_and_read_8(SPI1, 0x00, &dummy_byte));
    while(spi_write_and_read_8(SPI1, 0x29, &dummy_byte));
    ports_set_bit(cs_pin);
    Delay_ms(100);
    
    
    
    dma_ch_transfer_tx.dst_start_addr = (void *) spi_get_tx_reg(SPI1);   
    
    ports_clr_bit(cs_pin);
    dma_set_transfer_params(dma_id, &dma_ch_transfer_tx);   
    dma_channel_enable(dma_id, ON, false);
    
    while(1)
    {     
        static uint64_t tick = 0;
        static uint8_t v = 255, k = 0;
        
        if ((dma_get_flags(dma_id) & DMA_FLAG_BLOCK_TRANSFER_DONE) > 0)
        {
            dma_clear_flags(dma_id, DMA_FLAG_BLOCK_TRANSFER_DONE); 
            ports_set_bit(cs_pin);
            
            ports_clr_bit(cs_pin);
            dma_set_transfer_params(dma_id, &dma_ch_transfer_tx);   
            dma_channel_enable(dma_id, ON, false);
        }
        
        if (mTickCompare(tick) >= TICK_100MS)
        {
            mUpdateTick(tick);
            
            buffer[test[k]+2] = v;
            if (++k >= (sizeof(test)/2))
            {
                k = 0;
                v = (v == 0) ? 255 : 0;
            }
            
//            ports_clr_bit(cs_pin);
//            while(spi_write_and_read_8(SPI1, 0x01, &dummy_byte));
//            while(spi_write_and_read_8(SPI1, 0x2c, &dummy_byte));
//            for (i = 0; i < NUMBER_OF_LINES; i++) // y lines
//            {
//                for (j = 0; j < NUMBER_OF_RGB_LEDS; j++) // x pixel
//                {  
//                    if (LED_RESOLUTION)
//                    {
//                        while(spi_write_and_read_8(SPI1, GET_BIT(inc, 0) ? 255 : 0, &dummy_byte));
//                        while(spi_write_and_read_8(SPI1, GET_BIT(inc, 0) ? 255 : 0, &dummy_byte));
//                        while(spi_write_and_read_8(SPI1, GET_BIT(inc, 1) ? 255 : 0, &dummy_byte));
//                        while(spi_write_and_read_8(SPI1, GET_BIT(inc, 1) ? 255 : 0, &dummy_byte));
//                        while(spi_write_and_read_8(SPI1, GET_BIT(inc, 2) ? 255 : 0, &dummy_byte));
//                        while(spi_write_and_read_8(SPI1, GET_BIT(inc, 2) ? 255 : 0, &dummy_byte));
//                    }
//                    else
//                    {
//                        while(spi_write_and_read_8(SPI1, GET_BIT(inc, 0) ? 255 : 0, &dummy_byte));
//                        while(spi_write_and_read_8(SPI1, GET_BIT(inc, 1) ? 255 : 0, &dummy_byte));
//                        while(spi_write_and_read_8(SPI1, GET_BIT(inc, 2) ? 255 : 0, &dummy_byte));
//                    }
//                }
//            }
//            ports_set_bit(cs_pin);
//            
//            inc++;
//            inc &= 7;
        }
                       
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
