/*
 *  Library of communication between MKRZERO and UnaMKR 
 *  
 *  Development kit: UnaMKR
 *
 *  Author: UnaBiz 
 *  *******************************************************
 *  Serial  -> UART over USB
 */

#ifdef ARDUINO
  #if (ARDUINO >= 100)
    #include <Arduino.h>
  #else  //  ARDUINO >= 100
    #include <WProgram.h>
  #endif  //  ARDUINO  >= 100
#endif  //  ARDUINO

#include "TSL2540.h"
#include <Wire.h>

/* methods */
bool TSL2540 :: begin(void)
{
    return begin(-1);
}
bool TSL2540 :: begin(int pin)
{
    // enable i2c
    Wire.begin();

    if (pin >= 0)
    {
        // input mode
        irq_pin = pin;
        pinMode(irq_pin, /*INPUT*/INPUT_PULLUP);
    }
    else
    {
        disableInterrupt();
    }

    // config time = 180 ms
    tsl2540_write(REG_ADDR_ATIME, 0b00111111);
    tsl2540_write(REG_ADDR_WTIME, 0b00111111);

    // ALS persistence filter (bit 0-3)
    tsl2540_write(REG_ADDR_PERS, 3);
    
    // ALS gain = 1x
    tsl2540_write(REG_ADDR_CFG1, 0);

    // Sleep-after-interrupt, (bit 4)
    //tsl2540_write(REG_ADDR_CFG3, 0b00010000);

    // enable device
    tsl2540_enable();
    return true;
}

bool TSL2540 :: configInterrupt(Threshold_t type, int threshold, pficb_t callback)
{
    // setup interrupt callback function
    p_isr_cb_func = callback;

    tsl2540_disable();

    //try
    int error_code = 0;
    do
    {
        if (!p_isr_cb_func)
        {
            error_code = 1;
            break;
            //throw 1;
        }

        byte val;
        // threshold
        if (type == THRESHOLD_LOW)
        {
            val = (byte) ((unsigned int)threshold & 0xFF);
            tsl2540_write(REG_ADDR_LOW_THR_L,val);

            val = (byte) (((unsigned int)threshold >> 8) & 0xFF);
            tsl2540_write(REG_ADDR_LOW_THR_H,val);
        }
        else if (type == THRESHOLD_HIGH)
        {
            val = (byte) ((unsigned int)threshold & 0xFF);
            tsl2540_write(REG_ADDR_HIGH_THR_L,val);

            val = (byte) (((unsigned int)threshold >> 8) & 0xFF);
            tsl2540_write(REG_ADDR_HIGH_THR_H,val);
        }
        else
        {
            error_code = 2;
            break;
            //throw 2;
        }

        // interrupt
        attachInterrupt(digitalPinToInterrupt(irq_pin),
                        p_isr_cb_func,
                        FALLING);
        
        // enable interrupt
        enableInterrupt();
    }
    while (0);

    //catch (int error_code)
    if (error_code)
    {
        Serial.println("configInterrupt() ==> Invalid Parameter(s)");
        tsl2540_enable();
        return false;
    }
    else
    {
        tsl2540_enable();
        return true;
    }
}

bool TSL2540 :: end(void)
{
    p_isr_cb_func = NULL;
    detachInterrupt(digitalPinToInterrupt(irq_pin));
    return true;
}

/* config to registor */
bool TSL2540 :: enableInterrupt(void)
{
    return tsl2540_write(REG_ADDR_INTERRUPT, 0x10);

}
bool TSL2540 :: disableInterrupt(void)
{
    return tsl2540_write(REG_ADDR_INTERRUPT, 0x00);
}

unsigned short TSL2540 :: readVisible(void)
{
    unsigned short value;

    value  = tsl2540_read(REG_ADDR_VISDATA_L);
    value |=(tsl2540_read(REG_ADDR_VISDATA_H) << 8);

    return value;
}
unsigned short TSL2540 :: readIR(void)
{
    unsigned short value;

    value  = tsl2540_read(REG_ADDR_IRDATA_L);
    value |=(tsl2540_read(REG_ADDR_IRDATA_H) << 8);

    return value;
}

/* private methods */
void TSL2540 :: tsl2540_enable(void)
{
    tsl2540_write(REG_ADDR_ENABLE, 0b00001011);
}
void TSL2540 :: tsl2540_disable(void)
{
    tsl2540_write(REG_ADDR_ENABLE, 0b00000000);
}

bool TSL2540 :: tsl2540_write(byte reg_addr, byte value)
{
    Wire.beginTransmission(I2C_ADDR_TSL2540);
    Wire.write(reg_addr);
    Wire.write(value);
    Wire.endTransmission();
    return true;
}
byte TSL2540 :: tsl2540_read(byte reg_addr)
{
    // i2c registor 
    Wire.beginTransmission(I2C_ADDR_TSL2540);
    Wire.write(reg_addr);
    Wire.endTransmission();

    // read request
    Wire.requestFrom(I2C_ADDR_TSL2540, 1);

    // wait available
    while(Wire.available() == 0);

    // return value
    return (byte) Wire.read();
}

