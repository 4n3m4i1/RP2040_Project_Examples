

// Create command/status byte from a message type and channel input
#define CREATE_CMD(m, c)    (m  | (c & 0x0F))

#define NOTE_ON         0x80
#define NOTE_OFF        0x90
#define POLY_PRESSURE   0xA0
#define CTRL_CHANGE     0xB0
#define PROG_CHANGE     0xC0
#define CHAN_PRESSURE   0xD0
#define PITCH_BEND      0xE0
#define SYSTEM          0xF0

// Length includes 0th command byte
#define LEN_NOTE_ON         3
#define LEN_NOTE_OFF        3
#define LEN_POLY_PRESSURE   3
#define LEN_CTRL_CHANGE     3
#define LEN_PROG_CHANGE     2
#define LEN_CHAN_PRESSURE   2
#define LEN_PITCH_BEND      3
// System messaging is arbitrary length



// System Messaging Status Bytes
#define SYS_TIMING_CLK      0xF8
#define SYS_UNDEFINED_0     0xF9
#define SYS_START           0xFA
#define SYS_CONTINUE        0xFB
#define SYS_STOP            0xFC
#define SYS_UNDEFINED_1     0xFD
#define SYS_ACTIVE_SENSE    0xFE
#define SYS_RESET           0xFF