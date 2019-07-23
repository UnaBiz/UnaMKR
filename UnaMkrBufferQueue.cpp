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

#include "UnaMkrBufferQueue.h"

/* flag set, clear & check */
bool UnaMkrBufferQueue :: checkBufferFull(void)
{
    return buff_full;
}
void UnaMkrBufferQueue :: setResponseReceived  (void) 
{
    buff_response_completed = true; 
}
void UnaMkrBufferQueue :: clearResponseReceived(void) 
{
    buff_response_completed = false;
}
bool UnaMkrBufferQueue :: checkResponseReceived(void) 
{
    return buff_response_completed; 
}

void UnaMkrBufferQueue :: setOneLineReceived  (void)  
{
    buff_one_line_in_buff = true; 
}
void UnaMkrBufferQueue :: clearOneLineReceived(void)  
{
    buff_one_line_in_buff = false;
}
bool UnaMkrBufferQueue :: checkOneLineReceived(void)  
{
    return buff_one_line_in_buff; 
}

/*  */
void UnaMkrBufferQueue :: addNewLineCharacter(char c)
{
    for (int i = 0; i < 4; ++i)
    {
        if (!c || buff_change_line_character[i] == c)
        {
            // exist already or invalid character
            break;
        }

        if (buff_change_line_character[i] == 0)
        {
            //Serial.print("Add success\r\n");
            buff_change_line_character[i] = c;
            break;
        }
    }
}
void UnaMkrBufferQueue :: removeNewLineCharacter(char c)
{
    // invalid character
    if (!c) return;

    for (int i = 0; i < 4; ++i)
    {
        if (buff_change_line_character[i] == c)
        {
            buff_change_line_character[i] = 0;
            break;
        }
    }
}
bool UnaMkrBufferQueue :: checkNewLineCharacter(char c)
{
    bool rsp = false;
    
    for (int i = 0; i < 4; ++i)
    {
        if ( (c == '\r') || (buff_change_line_character[i] && (c == buff_change_line_character[i]) ) )
        {
            rsp = true;
            break;
        }
    }

    return rsp;
};
char UnaMkrBufferQueue :: getNewLineCharacter (int index)
{
    if (index >= 0 && index < 4)
        return buff_change_line_character[index];
    else
        return 0;
}


/* read one line from queue (until '\r')*/
bool UnaMkrBufferQueue :: getResponseData(char *pStream, int *pLen)
{
    return readOneLine(pStream, pLen, '\r');
}

/* read one line from queue */
bool UnaMkrBufferQueue :: readOneLine(char *pStream, int *pLen, char chr)
{
    char c;
    if (!pStream) return false;
    if (pLen) *pLen = 0;
    
    while (1)
    {
        c = read();
        
        // check end of line
        if (c != chr)
        {
            if (c == '\r' || c == chr)
                break;
        }
        else if (c == '\r')
        {
            break;
        }
        else if (c == '\n')
        {
            continue;
        }
        
        // check EOF
        if (c == 0xFF)
            break;

        *(pStream++) = c;
        
        if (pLen)
        {
          (*pLen)++;
        }
    }

    return (*pLen > 0) ? true : false;
}

/* write N-bytes data to queue */
bool UnaMkrBufferQueue :: writeQueue(char *c, int len)
{
    while (len && !buff_full)
    {
        buff_stack[buff_inx++] = *c;
        len--;
        c++;
        buff_length++;

        if (buff_inx >= buff_max)
            buff_inx = 0;

        if (buff_inx == buff_outx)
            buff_full = true;
    }

    return (len > 0) ? false : true;
}

/* read data from queue */
int UnaMkrBufferQueue :: readQueue(char *c, int len)
{
    int read_num = 0;
    
    while (len > 0 && buff_length > 0)
    {
        *c = buff_stack[buff_outx++];
        buff_length--;
        len--;
        c++;
        read_num++;

        if (buff_outx >= buff_max)
            buff_outx = 0;

        buff_full = false;
    }

    return read_num;
}

/* write one byte to queue */
bool UnaMkrBufferQueue :: write(char c) 
{
    if (!buff_full)
    {
        buff_stack[buff_inx++] = c;
        buff_length++;
        if (buff_inx >= buff_max)
            buff_inx = 0;

        if (buff_inx == buff_outx)
            buff_full = true;
    }
    return buff_full;
}
/* read one byte */
char UnaMkrBufferQueue :: read() 
{
    char c = 0xFF;

    if (buff_length > 0)
    {
        c = buff_stack[buff_outx++];
        buff_length--;
        
        if (buff_outx >= buff_max)
            buff_outx = 0;

        buff_full = false;
    }

    return c;
}
