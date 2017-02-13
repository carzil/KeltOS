#include "bluetooth/bt.h"

enum STATUS { DISCONNECTED = 0x0, CONNECTED = 0x1 };

int bt_init() {
    return CONNECTED;
}
