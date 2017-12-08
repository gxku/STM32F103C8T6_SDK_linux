#include "main.h" 
#include "protocol.h"
#include "uart.h" 

int parse_char(uint8_t c, data_pkt *pkt)
{
    int ret = 0;
    
    while (1)
    {    
        switch(pkt->state)
        {
        case MSG_PARSE_STATE_WAITING_MAGIC1:
            {
                if(c == MSG_MAGIC1)
                {
                    //print("\r\n >>> MAGIC1 matched\n");
                    pkt->state = MSG_PARSE_STATE_WAITING_MAGIC2;
                }
            }
            return ret;
            
        case MSG_PARSE_STATE_WAITING_MAGIC2:
            {
                if(c == MSG_MAGIC2)
                {
                    //print("\r\n >>> MAGIC2 matched\n");
                    pkt->state = MSG_PARSE_STATE_WAITING_TYPE;
                }
                else
                {
                    print("\r\n >>> T BACK to SEARCH MAGIC1\n");
                    pkt->state = MSG_PARSE_STATE_WAITING_MAGIC1;
                    continue;
                }
            }
            return ret;
            
        case MSG_PARSE_STATE_WAITING_TYPE:
            {
                
                if (c > MAX_MSG_TYPE)
                {
                    print("\r\n >>> T BACK to SEARCH MAGIC1\n");
                    pkt->state = MSG_PARSE_STATE_WAITING_MAGIC1;
                    continue;
                }
                
                pkt-> type= c;
                //print("\r\n >>> TYPE matched %02x\n", c);
                pkt->state = MSG_PARSE_STATE_WAITING_LEN;
            }
            return ret;
        case MSG_PARSE_STATE_WAITING_LEN:
            {
                if (c > MAX_MSG_LEN)
                {
                    print("\r\n >>> T BACK to SEARCH MAGIC1\n");
                    pkt->state = MSG_PARSE_STATE_WAITING_MAGIC1;
                    continue;
                }
                pkt->dat_len = c;
                pkt->state = MSG_PARSE_STATE_WAITING_DATA;
            }
            return ret;
            
        case MSG_PARSE_STATE_WAITING_DATA:
            {
                pkt->data[pkt->dat_index++] = c;
                pkt->chksum += c;
                if(pkt->dat_index == pkt->dat_len)
                {	
                    //print("\r\n >>> D to CHKSUM\n");
                    pkt->state = MSG_PARSE_STATE_WAITING_CHKSUM;
                }
                
            }
            return ret;
            
        case MSG_PARSE_STATE_WAITING_CHKSUM:
            {
                pkt->state = MSG_PARSE_STATE_WAITING_MAGIC1;
                pkt->dat_index = 0;
                
                if (pkt->chksum != c)
                {
                    
                    print("\r\n >>> BAD CHKSUM=%d\n", pkt->chksum);
                    pkt->chksum = 0;
                    continue;
                }
                
                pkt->chksum = 0;
                ret = 1;
                //print("\r\n >>> GOOD PKT\n");
            }
            return ret;
            
        default:
            {
                print("ERR PARSE STATE!");
            }
            break;
        }
    }
    
    return ret;
}



void send_data(char type, void *buffer, size_t buflen)
{
    uint8_t header[4] = {0};
    uint8_t sum=0;
    header[0] = MSG_MAGIC1;
    header[1] = MSG_MAGIC2;
    header[2] = type;
    // header[2] = msub.priority[i];
    header[3] = buflen;
    
    uart2_send(header, sizeof(header));
    uart2_send(buffer, buflen);
    for(int i=0;i<buflen;i++){
	sum+=((uint8_t*)buffer)[i];
    }
    uart2_send(&sum,1);

}

