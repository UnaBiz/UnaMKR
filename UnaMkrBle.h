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

#ifndef __UNAMKR_BLE__
#define __UNAMKR_BLE__

void sscanf_02X(const char *src, byte *out);

// bluetopoth low energy structure
/* Scan filter */
typedef enum __ScanRssiFilter__
{
    NO_RSSI_FILTER = 0,
    RSSI_LARGER_THAN_m70DB = 1,
    RSSI_LARGER_THAN_m60DB = 2,
    RSSI_LARGER_THAN_m50DB = 3,
    RSSI_LARGER_THAN_m40DB = 4,
    RSSI_LARGER_THAN_m30DB = 5,

} ScanRssiFilter;

typedef enum __ScanAdvFilter__
{
    NO_ADV_FILTER = 0,
    ADV_IBEACON   = 1,
    ADV_IPS       = 2,
    
} ScanAdvFilter;


/* Scan result */
typedef enum __ScanAdvertisingType__
{
    NON_CONNECTABLE = 0,
    CONNECTABLE     = 1,
    
} ScanAdvType;

typedef enum __ScanAddressType__
{
    PUBLIC_ADDRESS = 0,
    RANDOM_ADDRESS = 1,
    PUBLIC_IDENTITY_ADDRESS = 2,
    RANDOM_IDENTITY_ADDRESS = 3,
    
} ScanAddressType;

//typedef struct __UnaScanResult__
class UnaScanResult 
{
public:
    // constructor 
    UnaScanResult()
    {
        clear();
    };
    UnaScanResult(char *response)
    {
        if (response)
            decodeScanResult(response);
    };

    // destructor
    ~UnaScanResult(){};

    // method
    void clear (void)
    {
        AdvType     = NON_CONNECTABLE;
        AddressType = PUBLIC_ADDRESS;
        HexDataLength = 0;

        memset(Address, 0, sizeof(Address));
        memset(Data, 0, sizeof(Data));
        memset(HexAddress, 0, sizeof(HexAddress));
        memset(HexData, 0, sizeof(HexData));
    }

    /* transfer response data to UnaScanResult */
    bool decodeScanResult(char *response)
    {
        int  i, num, conn, addr_type, rssi, adv_len;

        if (strlen((const char*)response) < 16)
            return false;

        num = sscanf((const char*) response,
                    "%d,%d,%12s,%d,%d,%s",
                    &conn,
                    &addr_type,
                    &Address[0],
                    &rssi,
                    &adv_len,
                    &Data[0]);

        // set value of element
        AdvType     = (ScanAdvType) conn;
        AddressType = (ScanAddressType) addr_type;
        RSSI        = (signed char) rssi;
        HexDataLength = (byte) adv_len;

        // check adv structure
        if (num < 5 || adv_len >= 32)
            return false;

        // string to hex
        for (i=0; i<6; i++)
        {
            sscanf_02X((const char*)&Address[2*i], &HexAddress[i]);
        }
        for (i=0; i<adv_len; i++)
        {
            sscanf_02X((const char*)&Data[2*i], &HexData[i]);
        }

        // end of string
        Data[adv_len*2] = 0;

        return true;
    };

    // elements
    ScanAdvType     AdvType;
    ScanAddressType AddressType;

    // ascii type
    char  Address[13];
    char  Data[65];

    // hex type
    byte  HexAddress[6];
    byte  HexDataLength;
    byte  HexData[32];

    signed char  RSSI;
};


#define __FUNCTION_ADVERTISING_ENABLE__
#ifdef __FUNCTION_ADVERTISING_ENABLE__
/* Advertisement structure */
typedef enum  __Adv_type__
{
    ADV_TYPE_BT_FLAG    = 0x01,
    ADV_TYPE_UUID16     = 0x02,
    ADV_TYPE_UUID16_CMP = 0x03,
    ADV_TYPE_UUID32     = 0x04,
    ADV_TYPE_UUID32_CMP = 0x05,
    ADV_TYPE_UUID128    = 0x06,
    ADV_TYPE_UUID128_CMP= 0x07,
    ADV_TYPE_NAME_SHORT = 0x08,
    ADV_TYPE_NAME_CMP   = 0x09,
    ADV_TYPE_POWER_LEVEL= 0x0A, 

    /*  */
    ADV_TYPE_IPS        = 0x25,
    ADV_TYPE_3D_INFO    = 0x3D,
    ADV_TYPE_MANUFACTURER = 0xFF,

} AdvType_t;

typedef struct __BleAdvertisement__
{
    byte  adv_len;
    byte  adv_type;
    byte  adv_data[32];

    struct __BleAdvertisement__ *pNextList;

} Adv_t;

class UnaAdvertiser
{
public:
    /* constructor */
    UnaAdvertiser() 
    {
        pAdvList = NULL;
        clear();
    }
    UnaAdvertiser(ScanAdvType connectable, ScanAddressType address_type) 
    {
        pAdvList = NULL;
        clear();
        config(connectable, address_type);
    }
    /* destructor */
    ~UnaAdvertiser()
    {
        clear();
    }

    /* clear the data stack of advertisement */
    void clear(void)
    {
        Adv_t *next, *pL = pAdvList;

        lenData = 0;
        memset(advData, 0, 32*sizeof(byte));
    
        while (pL)
        {
            next = pL->pNextList;
            delete pL;
            pL = next;
        }
        pAdvList = NULL;
        config(NON_CONNECTABLE, RANDOM_ADDRESS);
    }
    /* config GAP advertising type: advertising type, address type*/
    bool config(ScanAdvType connectable, ScanAddressType address_type)
    {
        adv_connType = connectable;
        adv_addrType = address_type;
    }

    /* transfer string name to type of Adv_t */
    Adv_t  transfer_localName(const char* name)
    {
        return transfer_raw(ADV_TYPE_NAME_SHORT, (byte*)name, strlen(name));
    }
    /* transfer UUID-16bits to type of Adv_t */
    Adv_t  transfer_UUID16(const uint16_t *uuid16, int list_number)
    {
        Adv_t ad = {0};
        byte len = 1 + 2*list_number;
        byte *pdata = ad.adv_data;
        
        ad.adv_len  = len;
        ad.adv_type = (byte) ADV_TYPE_UUID16_CMP;

        for (int i=0; i<list_number; i++)
        {
            uint16_t uuid = *(uuid16 + i);
            pdata[0] = (byte)((uuid >> 0) & 0xFF);
            pdata[1] = (byte)((uuid >> 8) & 0xFF);
            len += 2;
        }
        return ad;
    }
    /* transfer a specifi raw data to type of Adv_t */
    Adv_t  transfer_raw(AdvType_t type, const byte *data, byte length)
    {
        Adv_t ad = {0};
        byte len = 1 + length;
        byte *pdata = ad.adv_data;
        
        ad.adv_len  = len;
        ad.adv_type = (byte) type;
        memcpy(ad.adv_data, data, length);

        return ad;
    }

    /* add specific adv type to data stack */
    bool add(Adv_t advertisement)
    {
        byte *ptr = advData + lenData;
        byte len  = advertisement.adv_len;

        if (len > 1)
        {
            Adv_t *pL, *pAdv = new Adv_t;
            memcpy(pAdv, &advertisement, sizeof(Adv_t));

            *(ptr + 0) = len;
            *(ptr + 1) = pAdv->adv_type;
            memcpy(ptr + 2, pAdv->adv_data, len - 1);

            lenData += len + 1;

            // find the end of link list
            pL = pAdvList;

            if (pL)
            {
                while (pL->pNextList)
                {
                    pL = pL->pNextList;
                }

                pL->pNextList = pAdv;
            }
            else
            {
                pAdvList = pAdv;
            }
            return true;
        }

        return false;
    }

    /* remove */
    bool remove(AdvType_t remove_type)
    {
        int  res_len = lenData;
        byte type, size, *ptr, *end;

        ptr = advData;
        end = ptr + lenData;

        while (res_len > 0)
        {
            size = ptr[0];
            type = ptr[1];

            if (type == remove_type)
            {
                for (int i=0; ptr + i + size + 1 <end; i++)
                {
                    ptr[i] = ptr[i+size+1];
                }

                while (ptr < end)
                    *(ptr++) = 0;

                lenData -= size + 1;

                // remove frome link list
                Adv_t *pre, *next, *curr;
                pre  = pAdvList;
                curr = pAdvList;

                if (curr)
                {
                    while ( curr && curr->adv_type != remove_type)
                    {
                        pre = curr;
                        curr = curr->pNextList;
                    }

                    if (curr)
                    {
                        next = curr->pNextList;
                        pre->pNextList = next;
                        delete curr;
                    }
                }

                break;
            }

            res_len -= size + 1;
            ptr     += size + 1;
        }

        return true;
    }

    /* Adv type */
    ScanAdvType getConnectionType(void) {return adv_connType;}
    ScanAddressType getAddressType(void){return adv_addrType;}

    /* Link list of adv */
    Adv_t *getListHead(void) {return pAdvList;}
    Adv_t *getNextList(Adv_t *pAdv) 
    {
        if (pAdv)   return pAdv->pNextList;
        else        return NULL;
    }
    

private:
    ScanAdvType         adv_connType;
    ScanAddressType     adv_addrType;

    // head of advertising list
    Adv_t *pAdvList;

    // raw data type
    byte lenData;
    byte advData[32];

};
#endif

#endif//__UNAMKR_BLE__
