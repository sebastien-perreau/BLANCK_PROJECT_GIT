 #include "bit_settings.h"
#include "config.h"

ACQUISITIONS_DEF(acquisitions);
BLE_PICKIT_DEF(ble_pickit, "SPC", BLE_SECURITY_DISABLED);


GROVE_MOTOR_DEF(grove_motor, I2C2);
BUS_MANAGEMENT_DEF(bm_i2c2, &grove_motor.i2c_params.bus_management_params);
SWITCH_DEF(sw1, SWITCH1, ACTIVE_LOW);

//static uint32_t crc1 = 0, crc2 = 0;
//uint32_t buffer1 = 0x12345678;
//
//
//#define crc_16(p_data, length)              crc_16_modbus(p_data, length)
//#define crc_16_modbus(p_data, length)       fu_crc16_fast_poly_0x8005(0xffff, true, true, 0x0000, p_data, length)
//#define crc_16_usb(p_data, length)          fu_crc16_fast_poly_0x8005(0xffff, true, true, 0xffff, p_data, length)
//#define crc_16_maxim(p_data, length)        fu_crc16_fast_poly_0x8005(0x0000, true, true, 0xffff, p_data, length)
//#define crc_16_arc(p_data, length)          fu_crc16_fast_poly_0x8005(0x0000, true, true, 0x0000, p_data, length)
//#define crc_16_buypass(p_data, length)      fu_crc16_fast_poly_0x8005(0x0000, false, false, 0x0000, p_data, length)
//#define crc_16_dds_110(p_data, length)      fu_crc16_fast_poly_0x8005(0x800d, false, false, 0x0000, p_data, length)
//
//static const uint16_t __crc16_tab_reflected_poly_0x8005[256] = 
//{
//    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241, 0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
//    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40, 0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
//    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40, 0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
//    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641, 0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
//    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240, 0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
//    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41, 0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
//    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41, 0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
//    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640, 0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
//    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240, 0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
//    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41, 0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
//    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41, 0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
//    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640, 0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
//    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241, 0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
//    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40, 0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
//    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40, 0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
//    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641, 0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
//};
//
//
//
//static uint32_t fu_crc_reflect_data(uint32_t data, uint8_t data_bit_length)
//{
//    uint32_t reflection = 0;
//    uint8_t current_bit;
//    for (current_bit = 0; current_bit < data_bit_length; ++current_bit)
//    {
//        if (data & 0x01)
//        {
//            reflection |= (1 << ((data_bit_length - 1) - current_bit));
//        }
//        data >>= 1;
//    }
//    return reflection;
//}
//
//static uint16_t fu_crc16_fast_poly_0x8005(uint16_t seed, bool reflected_input, bool reflected_output, uint16_t xorout, void * p_data, uint16_t length)
//{
//    uint8_t *p = p_data;
//    uint8_t data;
//    uint16_t crc = seed;
//    
//    while (length-- > 0)
//    {
//        data = *p++;
//        if (!reflected_input)
//        {
//            // Because crc16_tab is already reflected, if "reflected_input" is "false" we need to reflected it again.
//            data = fu_crc_reflect_data(data, 8);
//        }
//        crc = (crc >> 8) ^ __crc16_tab_reflected_poly_0x8005[ (crc ^ (uint16_t) data) & 0x00ff ];
//    }
//    
//    // Above, the CRC is calculated as "reflected".
//    if (!reflected_output)
//    {
//        // Reflected it again if "reflected_output" is "false".
//        crc = fu_crc_reflect_data(crc, 16);
//    }
//    
//    return (crc ^ xorout);
//}


int main(void)
{        
    cfg_pic32();
    cfg_ports(SWITCH1_PULLUP_ENABLE | SWITCH2_PULLUP_ENABLE | SWITCH3_PULLUP_ENABLE, 0, NULL);
    cfg_timers();    
    cfg_adc10(ADC_MUX);
    
    // Others initializations
//    dma_crc_16();
    pwm_init(PWM_NONE, 40000, 250000);
    ble_init(UART4, UART_BAUDRATE_1M, &ble_pickit, &acquisitions);
    log_init(UART1, UART_BAUDRATE_2M);
    m_init_hardware_picadapter();
    mUpdateLedStatusD2(OFF);
    mUpdateLedStatusD3(BLINK);          
        
//    static uint32_t time1, time2;
//    mResetTime();
//    dma_crc_execute(&buffer1, 4);               
//    while (!dma_crc_is_calculated(&crc1));
//    time1 = (mGetTick() - getTime);
//    mResetTime();
//    crc2 = crc_16(&buffer1, 4);
//    time2 = (mGetTick() - getTime);
//    LOG("crc1: %4x (%d)/ crc2: %4x (%d)", crc1, time1, crc2, time2);
    
        
    while(1)
    {     
        switch (sw1.indice)
        {
            case 0:
                break;

            case 1:
                e_grove_motor_stepper_stop(&grove_motor);
                sw1.indice++;
                break;

            case 2:
                break;

            case 3:
//                e_grove_motor_stepper_keep_run(&grove_motor, GROVE_MOTOR_FULL_STEP, 200, false);
                e_grove_motor_stepper_run(&grove_motor, GROVE_MOTOR_MICRO_STEPPING, 50, 192);
                sw1.indice++;
                break;

            default:
                sw1.indice = 0;
                break;
        }
        
        fu_switch(&sw1);
        fu_bus_management_task(&bm_i2c2);
        e_grove_motor_tb6612fng_deamon(&grove_motor);
                
        ble_stack_tasks();
               
        background_tasks(&acquisitions);
        
    }
}
