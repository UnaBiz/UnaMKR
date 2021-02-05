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

#ifndef __UNAMKR_MONARCH__
#define __UNAMKR_MONARCH__

#define SIGFOX_MAX_RC_ZONE  7

enum eRCZONE
{
  MONARCH_FAIL = 0,
  RCZ_1 = 1,
  RCZ_2 = 2,
  RCZ_3 = 3,
  RCZ_4 = 4,
  RCZ_5 = 5,
  RCZ_6 = 6,
  RCZ_7 = 7,
};

class UnaMonarch
{
public:
  UnaMonarch() 
  {
    clear();
  }
  ~UnaMonarch() 
  {

  }
  /* method */
  void clear(void) 
  {
    m_rc = 0;
    m_rssi = 0;
    m_valid = false;
    m_success = false;
  }
  bool checkAvailable(void) {return m_valid;}
  bool checkSuccessful(void) {return m_success;}
  int getRssi(void) {return m_rssi;}
  int getRc(void) {return m_rc;}
  bool decodeMonarchRsp(char *response) 
  {
    int num, zone, rssi;

    clear();
    if (strlen((const char*)response) < 3)
        return false;

    zone = rssi = 0;
    num = sscanf((const char*) response, "%d,%d", &zone, &rssi);

    if (num == 2) 
    {
        if (zone <= SIGFOX_MAX_RC_ZONE)
        {
            m_rssi = rssi;
            m_rc = zone;
            m_valid = true;

            if (zone > 0) 
                m_success = true;
        }
        return true;
    }
    else
    {
        return false;
    }
  }
  
private:
  bool m_valid;
  bool m_success;
  int m_rssi;
  int m_rc;
};

#endif /*__UNAMKR_MONARCH__*/