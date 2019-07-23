/*
 *  Library of communication between MKRZERO and UnaMKR 
 *  
 *  Development kit: UnaMKR
 *
 *  Author: UnaBiz 
 *  *******************************************************
 *  Serial  -> UART over USB
 */
#ifndef __TSL2540_SNR__
#define __TSL2540_SNR__

/* type */
typedef  void (*pficb_t)(void)    ;
typedef  enum _THRESHOLD_TYPE_
{
    THRESHOLD_LOW,
    THRESHOLD_HIGH,

} Threshold_t;

/* definitions */
#define     I2C_ADDR_TSL2540       0x39

#define     REG_ADDR_ENABLE        0x80
#define     REG_ADDR_ATIME         0x81
#define     REG_ADDR_WTIME         0x83
#define     REG_ADDR_LOW_THR_L     0x84
#define     REG_ADDR_LOW_THR_H     0x85
#define     REG_ADDR_HIGH_THR_L    0x86
#define     REG_ADDR_HIGH_THR_H    0x87
#define     REG_ADDR_PERS          0x8C
#define     REG_ADDR_CFG0          0x8D
#define     REG_ADDR_CFG1          0x90
#define     REG_ADDR_VISDATA_L     0x94
#define     REG_ADDR_VISDATA_H     0x95
#define     REG_ADDR_IRDATA_L      0x96
#define     REG_ADDR_IRDATA_H      0x97
#define     REG_ADDR_CFG3          0xAB
#define     REG_ADDR_INTERRUPT     0xDD

class TSL2540
{
public:
    /* constructor */
    TSL2540(){p_isr_cb_func = NULL;}
    
    /* destructor */
    ~TSL2540(){end();}

    /* methods */
    bool begin(int pin);
    bool begin(void);
    bool end(void);

    bool configInterrupt(Threshold_t type, int threshold, pficb_t callback);
    bool enableInterrupt(void);
    bool disableInterrupt(void);

    unsigned short readVisible(void);
    unsigned short readIR(void);

private:
    /* private methods */
    bool tsl2540_write(byte reg_addr, byte value);
    byte tsl2540_read(byte reg_addr);
    void tsl2540_enable(void);
    void tsl2540_disable(void);


    /* members */
    int     irq_pin;
    pficb_t p_isr_cb_func;
};

#endif //__TSL2540_SNR__
