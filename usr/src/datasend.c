#include <datasend.h>
#include <uart.h>

void real_send(int fd,char *str,int len){
    uart2_send(str,len);
    
}

void send_blk_data(int fd,int type,char *buff,int buf_len)
{
    char header[HEAD_LEN] = {0};
    unsigned char sum = 0;

    header[MSG_MGC1] = MESSAGE_MAGIC1;
    header[MSG_MGC2] = MESSAGE_MAGIC2;
    header[MSG_TYPE] = type;

    header[MSG_LEN] = buf_len;
    int i;
    for(i=0;i<buf_len;i++){
        sum = sum + (unsigned char)buff[i];
    }
    header[MSG_SUM] = sum;

    real_send(fd,header, sizeof(header));
    real_send(fd,buff, buf_len);
}
