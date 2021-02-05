/*
 *  Library of communication between MKRZERO and UnaMKR 
 *  
 *  Development kit: UnaMKR
 *
 *  Author: UnaBiz 
 *  *******************************************************
 *  Serial  -> UART over USB
 *  Serial1 -> TX/RX between MKRZERO and UnaMKR
 */

#ifdef ARDUINO
  #if (ARDUINO >= 100)
    #include <Arduino.h>
  #else  //  ARDUINO >= 100
    #include <WProgram.h>
  #endif  //  ARDUINO  >= 100
#endif  //  ARDUINO

#include "UnaMKR.h"

/* function for all methods */
void sscanf_02X(const char *src, byte *out)
{
    *out = 0;
    if (src[0] >= '0' && src[0] <= '9')
    {
        *out = (src[0] - '0') << 4;
    }
    else
    {
        *out = (src[0] - 'A' + 10) << 4;
    }
    
    if (src[1] >= '0' && src[1] <= '9')
    {
        *out += (src[1] - '0');
    }
    else
    {
        *out += (src[1] - 'A' + 10);
    }
}

/* ***************** UnaMKR ***************** */
bool UnaMKR :: init(void)
{
    unsigned long ctime, stime = millis();
    bool status;

    // PIO serial port (TX, RX)
    Serial1.begin(115200);

    // disable log message
    logEnable(false);

    // power on init
    reset();
    getData_CheckOk(2500);

    // clear buffer queue
    resetQueue();
    return true;
}

bool UnaMKR :: configEcho(bool enable) 
{
    int resp_len;
    char cmd[16], resp_data[32];
    bool status = false;

    resetQueue();
    sprintf(cmd, "AT$ECHO=%d\r", enable ? 1 : 0);
    Serial1.flush();
    Serial1.print(cmd);
    delay(100);
    if (getData(resp_data, &resp_len, 3000) == true) 
    {
        if (resp_data[0] == 'A' && resp_data[1] == 'T')
        {
            getData_CheckOk(3000);
        }
        fEcho = enable;
    }
    return status;
}

/* ***************** Commands ***************** */
int  UnaMKR:: echo(void)
{
    int status = chk_command_seq();
    if (status) return status;
    if (printlog_en) Serial.println("AT?");

    resetQueue();
    Serial1.flush();
    Serial1.print("AT?\r");
    return NO_ERROR;
}

int  UnaMKR:: reset(void)
{
    int resp_len, status;
    char resp_data[32];

    status = chk_command_seq();
    if (status) return status;
    if (printlog_en) Serial.println("AT$RESET");

    resetQueue();
    Serial1.flush();
    Serial1.print("AT$RESET\r");
    delay(100);
    if (getData(resp_data, &resp_len, 3000) == true) 
    {
        if (resp_data[0] == 'A' && resp_data[1] == 'T')
        {
            getData_CheckOk(1000);
        }
        delay(3000);
        configEcho(fEcho);
    }

    Serial1.flush();
    Serial1.print("AT?\r");
    return NO_ERROR;
}
int  UnaMKR:: sleep(void)
{
    return sleep(0);
}
int  UnaMKR:: sleep(int sleep_time)
{
    char str[32];

    int status = chk_command_seq();
    if (status) return status;

    if (sleep_time)
        sprintf(str, "AT$SLEEP=%d\r", sleep_time);
    else
        sprintf(str, "AT$SLEEP\r");

    if (printlog_en) Serial.println((const char*) str);

    resetQueue();
    Serial1.flush();
    Serial1.print((const char*) str);

    fSleep = true;
    return NO_ERROR;
}
int  UnaMKR:: wakeup(void)
{
    //int status = chk_command_seq();
    //if (status) return status;
    if (!Serial1)
        return ERR_SERIAL_CLOSED;

    Serial1.print("\r\r");
    delay(100);
    while (Serial1.available()) Serial1.read();

    return NO_ERROR;
}
int  UnaMKR :: getVersion(void)
{
    int status = chk_command_seq();
    if (status) return status;

    if (printlog_en) Serial.println("AT$FW?");

    resetQueue();
    Serial1.flush();
    Serial1.print("AT$FW?\r");
    
    return NO_ERROR;
}
int  UnaMKR :: getInfo(void)
{
    int status = chk_command_seq();
    if (status) return status;

    if (printlog_en) Serial.println("AT$INFO?");

    resetQueue();
    Serial1.flush();
    Serial1.print("AT$INFO?\r");
    
    return NO_ERROR;
}
  
/* Sigfox commands */
int  UnaMKR:: getId(void)
{
    int status = chk_command_seq();
    if (status) return status;

    if (printlog_en) Serial.println("AT$ID?");

    resetQueue();
    Serial1.flush();
    Serial1.print("AT$ID?\r");
    
    return NO_ERROR;
}

int  UnaMKR:: getPac(void)
{
    int status = chk_command_seq();
    if (status) return status;

    if (printlog_en) Serial.println("AT$PAC?");

    resetQueue();
    Serial1.flush();
    Serial1.print("AT$PAC?\r");
    
    return NO_ERROR;
}

int  UnaMKR:: getZone(void)
{
    int status = chk_command_seq();
    if (status) return status;

    if (printlog_en) Serial.println("AT$ZONE?");
    
    resetQueue();
    Serial1.flush();
    Serial1.print("AT$ZONE?\r");
    
    return NO_ERROR;
}
int  UnaMKR:: setZone(int rcz)
{
    char str[16];

    int status = chk_command_seq();
    if (status) return status;

    if (rcz < 1 || rcz > SIGFOX_MAX_RC_ZONE)
        return ERR_INVALID_PARAMETER;

    // set private/public key
    if (!public_key_sent)
    {
        bool logprn = printlog_en;
        public_key_sent = true;

        logEnable(false);
        publicKey(public_key);
        getData_CheckOk(1000);
        logEnable(logprn);

        delay(1000);
    }

    sprintf(str, "AT$SETZONE=%d\r", rcz);
    if (printlog_en) Serial.println((const char *) str);

    resetQueue();
    Serial1.flush();
    Serial1.print((const char*) str);

    return NO_ERROR;
}
int  UnaMKR:: setZone(enum eRCZONE ercz) {
    return setZone((int)ercz);
}
int  UnaMKR:: monarch(int search_time)
{
    char str[32];

    int status = chk_command_seq();
    if (status) 
        return status;

    if (search_time < 0 || search_time > 3600)
        return ERR_INVALID_PARAMETER;

    sprintf(str, "AT$MONARCH=%d\r", search_time);
    if (printlog_en) Serial.println((const char *) str);

    resetQueue();
    Serial1.flush();
    Serial1.print((const char*) str);

    return NO_ERROR;
}
int UnaMKR :: getMonarch(void) 
{
    char str[16];

    int status = chk_command_seq();
    if (status) 
        return status;
    
    sprintf(str, "AT$MONARCH?\r");
    if (printlog_en) Serial.println((const char *) str);

    resetQueue();
    Serial1.flush();
    Serial1.print((const char*) str);

    return NO_ERROR;
}
int UnaMKR :: publicKey(bool enable)
{
    char str[16];

    int status = chk_command_seq();
    if (status) return status;

    sprintf(str, "AT$PBKEY=%d\r", (enable)? 1 : 0);
    if (printlog_en) Serial.println((const char *) str);

    resetQueue();
    Serial1.flush();
    Serial1.print((const char*) str);
    
    return NO_ERROR;
}

// send frame data (ASCII string)
int  UnaMKR:: uplink(const String data_str)
{
    char buf[12];
    int  length = data_str.length();
    if (length <= 12)
    {
        data_str.toCharArray(buf, 12);
        return sf_send_frame(false, (const char*) buf, length);
    }
    else
    {
        return ERR_INVALID_PARAMETER;
    }
}

int  UnaMKR:: downlink(const String data_str)
{
    char buf[12];
    int  length = data_str.length();
    if (length <= 12)
    {
        data_str.toCharArray(buf, 12);
        return sf_send_frame(true, (const char*) buf, length);
    }
    else
    {
        return ERR_INVALID_PARAMETER;
    }
}


// send frame data (ASCII data)
int  UnaMKR:: uplink(const char *data_str)
{
    return sf_send_frame(false, data_str);
}
int  UnaMKR:: downlink(const char *data_str)
{
    return sf_send_frame(true, data_str);
}

// send frame data (HEX)
int  UnaMKR:: uplink(const byte *hex_data, int data_len)
{
    return sf_send_frame(false, (const char*)hex_data, data_len);
}
int  UnaMKR:: downlink(const byte *hex_data, int data_len)
{
    return sf_send_frame(true, (const char*)hex_data, data_len);
}

// send frame data (MSG)
int  UnaMKR :: uplink(UnaShieldMessage* pMsg)
{
    byte hex_data[12];
    int  data_len;

    if (!pMsg->getHexData(hex_data, &data_len))
        return ERR_INVALID_PARAMETER;

    //debug
    // Serial.println("UPLINK DATA:");
    // for (int i=0; i<data_len; i++)
    // {
    //     Serial.println(hex_data[i]);
    // }

    return sf_send_frame(false, (const char*)hex_data, data_len);
}
int  UnaMKR :: downlink(UnaShieldMessage* pMsg)
{
    byte hex_data[12];
    int  data_len;
    
    if (!pMsg->getHexData(hex_data, &data_len))
        return ERR_INVALID_PARAMETER;
    
    return sf_send_frame(true, (const char*)hex_data, data_len);
}

int  UnaMKR :: uplink(UnaMkrMessage* pMsg)
{
    byte hex_data[12];
    int  i, data_len, playlod_num = pMsg->getPayloadNum();

    for (i=0; i<playlod_num; i++)
    {    
        if (pMsg->getHexData(i, hex_data, &data_len))
        {
            sf_send_frame(false, (const char*)hex_data, data_len);

            if (i < playlod_num - 1)
            {
                int to = pMsg->getTimeout();

                if (to)
                    getData_CheckOk(to);
                else
                    getData_CheckOk(10000);
            }
        }
        
    }

    //debug
    // Serial.println("UPLINK DATA:");
    // for (int i=0; i<data_len; i++)
    // {
    //     Serial.println(hex_data[i]);
    // }

    return NO_ERROR;
}


int UnaMKR :: sf_send_frame(bool f_downlink, const char* hex_data, int data_len)
{
    char *p, str[64];
    
    int status = chk_command_seq();
    if (status) return status;

    if (data_len > 12)
        return ERR_INVALID_PARAMETER;

    // set private/public key
    if (!public_key_sent)
    {
        bool logprn = printlog_en;
        public_key_sent = true;

        logEnable(false);
        publicKey(public_key);
        getData_CheckOk(1000);
        logEnable(logprn);

        delay(1000);
    }
    resetQueue();
    Serial1.flush();
    p = str;
    // if (f_downlink)
    //     p += sprintf(p, "AT+DL=");
    // else
    //     p += sprintf(p, "AT+UL=");
    p += sprintf(p, "AT$SF=");

    for(int i=0; i<data_len; i++)
        p += sprintf(p, "%02X", hex_data[i]);

    *(p ++) = ',';

    if (f_downlink)
        *(p ++) = '1';
    else
        *(p ++) = '0';

    *(p ++) = '\r';
    *(p ++) = '\0';
    
    if (printlog_en) Serial.println((const char *) str);
    Serial1.print((const char *) str);
    return NO_ERROR;
}
int UnaMKR :: sf_send_frame(bool f_downlink, const char* data_str)
{
    char str[64];
    int  data_len = strlen(data_str)/2;

    int status = chk_command_seq();
    if (status) return status;

    if (data_len > 12)
        return ERR_INVALID_PARAMETER;

    resetQueue();
    Serial1.flush();
    // if (f_downlink)
    //     sprintf(str, "AT+DL=%s\r", data_str);
    // else
    //     sprintf(str, "AT+UL=%s\r", data_str);
    sprintf(str, "AT$SF=%s,%d\r", data_str, (f_downlink) ? 1 : 0);
    
    if (printlog_en) Serial.println((const char *) str);
    Serial1.print((const char *) str);
    return NO_ERROR;
}


/* BLE commands */
int  UnaMKR:: setBtName(const char *pName)
{
    char str[32];
    int len;

    int status = chk_command_seq();
    if (status) 
        return status;
    if (!pName)
        return ERR_INVALID_PARAMETER;

    resetQueue();
    Serial1.flush();
    
    len = strlen(pName);
    if (!len || len > 16)
        return ERR_INVALID_PARAMETER;

    sprintf(str, "AT$NAME=%s\r", pName);
    if (printlog_en) Serial.println((const char *) str);
    Serial1.print((const char*) str);
    return NO_ERROR;
}
int  UnaMKR:: setBtDeviceAddress(const char *pBdAddr)
{
    char str[32];
    int len;

    int status = chk_command_seq();
    if (status) 
        return status;
    if (!pBdAddr)
        return ERR_INVALID_PARAMETER;

    resetQueue();
    Serial1.flush();
    
    len = strlen(pBdAddr);
    if (len != 12)
        return ERR_INVALID_PARAMETER;

    sprintf(str, "AT$BTADDR=%s\r", pBdAddr);
    if (printlog_en) Serial.println((const char *) str);
    Serial1.print((const char*) str);
    return NO_ERROR;
}
int  UnaMKR:: getBtDeviceAddress(void)
{
    int status = chk_command_seq();
    if (status) 
        return status;
    
    resetQueue();
    Serial1.flush();

    if (printlog_en) Serial.println("AT$BTADDR?");
    Serial1.print("AT$BTADDR?\r");
    return NO_ERROR;
}
int  UnaMKR:: btReset(void)
{
    int status = chk_command_seq();
    if (status) return status;

    resetQueue();
    Serial1.flush();
    Serial1.print("AT$BTRST\r");
    if (printlog_en) Serial.println("AT$BTRST");
    return NO_ERROR;
}

int UnaMKR :: btScan(void)
{
    return btScan(3000, 0, 0);
}
int UnaMKR :: btScan(int scan_time_ms)
{
    return btScan(scan_time_ms, 0, 0);
}

int  UnaMKR:: btScan(int scan_time_ms, int RSSI_filter, int Adv_filter)
{
    char str[32];

    int status = chk_command_seq();
    if (status) 
        return status;
    if (scan_time_ms <= 0 || RSSI_filter > 5 || Adv_filter > 2)
        return ERR_INVALID_PARAMETER;
    
    resetQueue();
    Serial1.flush();
    
    sprintf(str, "AT$BTSCAN=%d,%d,%d\r", scan_time_ms, RSSI_filter, Adv_filter);
    if (printlog_en) Serial.println((const char *) str);
    Serial1.print((const char*) str);
    return NO_ERROR;
}

int UnaMKR :: btScan(int scan_time_ms, ScanRssiFilter RSSI_filter, ScanAdvFilter Adv_filter)
{
    return btScan(scan_time_ms, (int)RSSI_filter, (int) Adv_filter);
}


int  UnaMKR:: btGetScanResult(void)
{
    int status = chk_command_seq();
    if (status) return status;
    if (printlog_en) Serial.println("AT$BTRESULT?");

    resetQueue();
    Serial1.flush();
    Serial1.print("AT$BTRESULT?\r");
    
    return NO_ERROR;
}
int  UnaMKR:: btConnect(const char *pBdAddress)
{
    char str[32];
    int len;

    int status = chk_command_seq();
    if (status) 
        return status;
    if (!pBdAddress)
        return ERR_INVALID_PARAMETER;

    resetQueue();
    Serial1.flush();
    
    len = strlen(pBdAddress);
    if (len != 12)
        return ERR_INVALID_PARAMETER;

    sprintf(str, "AT$BTCONN=%s\r", pBdAddress);
    if (printlog_en) Serial.println((const char *) str);

    Serial1.print((const char*) str);
    return NO_ERROR;
}
int  UnaMKR:: btDisconnect(void)
{
    int status = chk_command_seq();
    if (status) return status;
    if (printlog_en) Serial.println("AT$BTDISCONN");

    resetQueue();
    Serial1.flush();
    Serial1.print("AT$BTDISCONN\r");
    return NO_ERROR;
}

int UnaMKR :: btIsConnect(void) 
{
    int status = chk_command_seq();
    if (status) return status;
    if (printlog_en) Serial.println("AT$BTCONN?");

    resetQueue();
    Serial1.flush();
    Serial1.print("AT$BTCONN?\r");
    return NO_ERROR;
}

int  UnaMKR :: btWrite(const String text)
{
    char data[128];
    int data_len;
    int status = chk_command_seq();
    if (status) return status;
    if (printlog_en) Serial.print("AT$BTTX=");

    resetQueue();
    Serial1.flush();
    Serial1.print("AT$BTTX=");
    text.toCharArray(data, 127);
    data_len = text.length();
    for (int i = 0; i < data_len; ++i)
    {
        Serial1.write(data[i]);
        if (printlog_en) Serial.print(data[i]);
    }
    Serial1.write('\r');
    Serial1.flush();
    if (printlog_en) Serial.println("");
    return NO_ERROR;
}
int  UnaMKR:: btRead(int read_len)
{
    char str[32];

    int status = chk_command_seq();
    if (status) return status;

    if (!read_len || read_len > 255)
        return ERR_INVALID_PARAMETER;

    resetQueue();
    Serial1.flush();
    
    sprintf(str, "AT$BTRX=%d\r", read_len);
    if (printlog_en) Serial.println((const char *) str);

    Serial1.print((const char*) str);
    return NO_ERROR;
}

int  UnaMKR :: btAdvertising(UnaAdvertiser* adv)
{
    char str[96];
    bool ret;
    int  status = chk_command_seq();
    if (status) return status;

    resetQueue();
    Serial1.flush();

    if (adv)
    {
        int gap_adv_type;
        ScanAdvType mkr_adv_type = adv->getConnectionType();

        // decode as type of GAP_ADV 
        switch (mkr_adv_type)
        {
            case CONNECTABLE:
                gap_adv_type = 0x00;    //ADV_IND
                break;

            case NON_CONNECTABLE:
            default:
                gap_adv_type = 0x02;    //ADV_SCAN_IND
                break;
        }
        sprintf(str, "AT$BTADV=%d,%d\r", gap_adv_type, adv->getAddressType());
    }
    else
    {
        strcpy(str, "AT$BTADV\r");
    }

    if (printlog_en) Serial.println((const char *) str);
    Serial1.print(str);

    if (adv && adv->getListHead())
    {
        char *ptr;
        Adv_t *pList;
        ret = getData_CheckOk(2500);
        pList = adv->getListHead();

        while (pList)
        {
            // AT+BTSETADV={datasize},{advertising flag},{data}\r
            ptr = str;
            ptr += sprintf(ptr, "AT$BTSETADV=%d,%d,", (pList->adv_len-1)*2,pList->adv_type);
            for (int i=0; i<pList->adv_len-1; i++)
            {
                ptr += sprintf(ptr, "%02X", pList->adv_data[i]);
            }
            ptr [0] = '\r';
            ptr [1] = '\0';

            if (printlog_en) Serial.println((const char *) str);
            Serial1.print((const char*)str);
            pList = pList->pNextList;

            if (pList)
                ret = getData_CheckOk(2500);
        }
    
        if (ret == true)
            return NO_ERROR;
        else
            return ERR_INVALID_PARAMETER;
    }

    return NO_ERROR;
}


/* data is available in buffer queue */
bool UnaMKR :: checkData_Available(void)
{
    return checkOneLineReceived();
}

/* non-blocked read data from UART to buffer queue*/
bool UnaMKR :: getData_try(char *response, int *resp_len)
{
    bool data_in_queued;

    // from PIO-RX to queue
    uart_write_queue();

    /* check response data & copy to buffer 'response' */
    data_in_queued = checkOneLineReceived();
    if (response && data_in_queued)
    {
        // check the end of response
        if (checkResponseReceived())
        {
            // end of response '\r'
            if (getResponseData(response, resp_len))
            {
                clearOneLineReceived();
            }
        }
        else
        {
            // one-line
            for (int i=0; i<4; i++)
            {
                char nl = getNewLineCharacter(i);
                if (nl && readOneLine(response, resp_len, nl) == true)
                {
                    clearOneLineReceived();
                    break;
                }
            }
        }
    }
    return data_in_queued;
}

bool UnaMKR :: getData(char *response, int *resp_len, unsigned int timeout)
{
    unsigned long delta, ctime, stime, l_timeout;
    bool read_success = false;

    stime = millis();
    l_timeout = (unsigned long) timeout;
    
    do
    {
        read_success = getData_try(response, resp_len);

        // end of string
        if (read_success && response && resp_len) 
        {
            response[*resp_len] = '\0';
            break;
        }

        // timeout
        if (!l_timeout) 
            break;

        ctime = millis();
        delta = ctime - stime;

        //yield();

    } while ( (!read_success) && (delta < l_timeout) );
    
    return read_success;
}

/* Parser */
bool UnaMKR :: getData_CheckOk(unsigned int timeout)
{
    char response[128];
    int  resp_len;

    if (!getData(response, &resp_len, timeout))
    {
        if (checkPrintLog())
            Serial.println("Response: Read data failed");

        return false;
    }
    if (!strstr(response, "OK"))
    {
        if (checkPrintLog())
        {
            char str[32];
            sprintf(str, "Response: NOT OK (data = %s, %d)", response, resp_len);
            Serial.println(str);
        }
        return false;
    }
    return true;
}
bool UnaMKR :: getData_ScanResult(UnaScanResult *result, unsigned int timeout)
{
    char response[128];
    int  resp_len;
    bool rsp = false;

    if (!result)
        return false;

    /* clear elements */
    result->clear();
    
    /* return false if response data has been processed */ 
    if (checkResponseReceived())
        return false;

    /* The scan result is separated by ';' */
    addNewLineCharacter(';');

    /* read buffer queue */
    bool retry;

    do
    {
        retry = false;
        if (getData(response, &resp_len, timeout))
        {
            // check the response data is empty 
            if (!resp_len)
                break;

            // if decodes failed, ignore current response
            if (result->decodeScanResult(response) == false)
                retry = true;
            else
                rsp = true;
        }

    } while (retry);

    /* remove character due to completed */
    removeNewLineCharacter(';');
    return rsp;
}

bool UnaMKR :: getData_Monarch(UnaMonarch *result, unsigned int timeout)
{
    char response[64];
    int  resp_len;
    bool rsp = false;

    if (!result)
        return false;

    /* clear elements */
    result->clear();
    
    /* return false if response data has been processed */ 
    if (checkResponseReceived())
        return false;

    /* read buffer queue */
    bool retry;

    do
    {
        retry = false;
        if (getData(response, &resp_len, timeout))
        {
            // check the response data is empty 
            if (!resp_len)
                break;

            // if decodes failed, ignore current response
            if (result->decodeMonarchRsp(response) == false)
                retry = true;
            else
                rsp = true;
        }

    } while (retry);

    return rsp;
}

void UnaMKR :: logEnable(bool enable)
{
    printlog_en = enable;
}

bool UnaMKR :: checkPrintLog(void)
{
    return printlog_en;
}

/* ***************** private functions ***************** */
int  UnaMKR :: chk_command_seq(void)
{
    // check serial port
    if (!Serial1)
        return ERR_SERIAL_CLOSED;

    // try to wake MKR up
    if (fSleep)
    {
        fSleep = false;
        wakeup();
    }

    // clear RX buffer
    while (Serial1.available()) Serial1.read();

    return NO_ERROR;
}

/* uart 2 queue */
void  UnaMKR :: uart_write_queue(void)
{
    while (Serial1.available()) 
    {
        bool fNewLine = false;
        char c;

        if (checkBufferFull())
            break;

        c = Serial1.read();
        
        if (checkNewLineCharacter(c))
            fNewLine = true;
        
        if (( (c >= 0x20) && (c <= 0x7F) ) || fNewLine)
        {
            /* enqueue */
            write(c);

            /* write to TX (uart over usb) */
            if (checkPrintLog())
                Serial.write(c);

            if (fNewLine)
            {
                setOneLineReceived();

                /* set both of flag if we recv '\r' */
                if (c == '\r')
                    setResponseReceived();
                
                /* write to TX (uart over usb) */
                if (checkPrintLog())
                    Serial.println("");

                /* exit while-loop because the main flow parser needs to proccess the response data */
                break;
            }
        }
    }
}