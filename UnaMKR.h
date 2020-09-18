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

#ifndef __UNAMKR__
#define __UNAMKR__

#include "UnaMkrMonarch.h"
#include "UnaMkrBle.h"
#include "UnaMkrMessage.h"
#include "UnaMkrBufferQueue.h"

class UnaMKR : public UnaMkrBufferQueue
{
public:
  /* Constructor */
  UnaMKR()
  {
    public_key  = false;    // applied private key
    printlog_en = false;    // disable log output
  };

  /* Destructor */
  ~UnaMKR(){};
  
  /* Initialize */
  bool  begin()
  {
      return init();
  }
  bool  begin(bool public_key_usage)
  {
      public_key = public_key_usage;
      return init();
  }

  /* System commands */
  int   echo(void);
  int   reset(void);
  int   sleep(void);
  int   sleep(int sleep_time_ms);
  int   wakeup(void);
  int   getVersion(void);
  int   getInfo(void);
  
  /* Sigfox commands */
  int   getId(void);
  int   getPac(void);
  int   getZone(void);
  int   setZone(int rcz);
  int   setZone(enum eRCZONE ercz);
  int   monarch(int search_time_ms);
  int   getMonarch(void);
  int   publicKey(bool enable);

  /* Uplink ASCII string data */
  int   uplink(const String data_str);

  /* Uplink hexadecimal ASCII data */
  int   uplink(const char *data_str);

  /* Uplink hexadecimal raw data */
  int   uplink(const byte *hex_data, int data_len);

  /* Uplink message */
  int   uplink(UnaShieldMessage* pMsg);
  int   uplink(UnaMkrMessage* pMsg);

  /* Upink ASCII string data for downlink */
  int   downlink(const String data_str);

  /* Upink hexadecimal ASCII data for downlink */
  int   downlink(const char *data_str);

  /* Upink hexadecimal raw data for downlink */
  int   downlink(const byte *hex_data, int data_len);

  /* Upink message for downlink */
  int   downlink(UnaShieldMessage* pMsg);

  /* BLE commands */
  int   setBtName(const char *pName);
  int   setBtDeviceAddress(const char *pBdAddr);
  int   getBtDeviceAddress(void);
  int   btReset(void);
  int   btScan(void);
  int   btScan(int scan_time_ms);
  int   btScan(int scan_time_ms, int RSSI_filter, int Adv_filter);
  int   btScan(int scan_time_ms, ScanRssiFilter RSSI_filter, ScanAdvFilter Adv_filter);
  int   btGetScanResult(void);
  int   btAdvertising(UnaAdvertiser* adv);
  int   btConnect(const char *pBdAddress);
  int   btDisconnect(void);
  int   btIsConnect(void);

  /* Peripheral TX & RX */
  int   btWrite(const String text);
  int   btRead(int read_len);
  
  /* Data is available in buffer queue */
  bool  checkData_Available(void);

  /* Receive function without blocking */
  bool  getData_try(char *response, int *resp_len);

  /* Receive function with timeout. If timeout is zero, return immediately (non-blocking) */
  bool  getData(char *response, int *resp_len, unsigned int timeout);

  /* Receive and check response is "OK" */
  bool  getData_CheckOk(unsigned int timeout);

  /* Receive and decode as UnaScanResult */
  bool  getData_ScanResult(UnaScanResult *result, unsigned int timeout);

  /* Receive and decode monarch RC zone & RSSI */
  bool  getData_Monarch(UnaMonarch *result, unsigned int timeout);

  /* Configuration of log print-out on Serial (over USB) port */
  void  logEnable(bool enable);

private:
  /* Initialize */
  bool  init(void);
  bool  configEcho(bool enable);

  /* UART to queue */
  void  uart_write_queue(void);

  /* Check command sequence */
  int   chk_command_seq(void);

  /* Send frame */
  int   sf_send_frame(bool f_downlink, const char* hex_data, int data_len);
  int   sf_send_frame(bool f_downlink, const char* data_str);

  /* Public or private key */
  bool public_key = false;
  bool public_key_sent = false;

  /* Flag */
  bool fSleep = true;
  bool fEcho = false;

  /* Debug log */
  bool printlog_en;
  bool checkPrintLog(void);

};

/* Digital I/O */
#define   PIN_BUTTON                (1)
#define   PIN_REED_INT              (6)
#define   PIN_LIGHT_INT             (2)

/* status */
#define   NO_ERROR                  (0)
#define   ERR_SERIAL_CLOSED         (1)
#define   ERR_INVALID_PARAMETER     (2)



#endif //__UNAMKR__
