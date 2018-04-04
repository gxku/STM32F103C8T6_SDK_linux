#define MSG_MAX_LEN 128

#define MSG_MGC1 0
#define MSG_MGC2 1
#define MSG_TYPE 2
#define MSG_SUM  3
#define MSG_LEN  4

#define HEAD_LEN 5

#define MESSAGE_MAGIC1 0xff
#define MESSAGE_MAGIC2 0xfe
enum rcv_msg_state
{
    PARSE_STATE_IDLE,
    PARSE_STATE_GOT_MSG_STX,
    PARSE_STATE_GOT_MSG_STX2,
    PARSE_STATE_GOT_SENSOR_TYPE,
    PARSE_STATE_GOT_CHECK_SUM,
    PARSE_STATE_GOT_MSG_LEN,
};

typedef struct
{
    char header[HEAD_LEN];     
    //header[0]:magic  header[1]:sensor type  header[2]:priority  header[3]:msglen
    char msg[MSG_MAX_LEN];
    int  hdr_index;
    int  msg_index;
}rcvd_msg;

void send_blk_data(int fd,int type,char *buff,int buf_len);
