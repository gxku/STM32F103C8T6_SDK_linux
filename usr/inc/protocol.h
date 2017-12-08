#ifndef PROTOCOL_H
#define PROTOCOL_H

#define MAX_MSG_LEN          64
#define MSG_MAGIC1 0xff
#define MSG_MAGIC2 0xfe

#define MAX_MSG_TYPE 16
enum msg_parse_state
{
    MSG_PARSE_STATE_WAITING_MAGIC1,
    MSG_PARSE_STATE_WAITING_MAGIC2,
    MSG_PARSE_STATE_WAITING_TYPE,
    MSG_PARSE_STATE_WAITING_LEN,
    MSG_PARSE_STATE_WAITING_CHKSUM,
    MSG_PARSE_STATE_WAITING_DATA,
};

typedef struct
{
    uint8_t type;
    uint8_t data[MAX_MSG_LEN];
    uint8_t dat_index;
    uint8_t dat_len;
    uint8_t chksum;
    enum msg_parse_state  state;
} data_pkt;

int parse_char(uint8_t c, data_pkt *pkt);
void send_data(char type, void *buffer, size_t buflen);

#endif

