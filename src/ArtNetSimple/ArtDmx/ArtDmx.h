#ifndef _ART_DMX_H_
#define _ART_DMX_H_

#include <Arduino.h>

#include "Common.h"

struct ArtDmx {
    ArtHeader header;
    uint8_t sequence;
    uint8_t physical;
    uint8_t net;
    uint8_t subUni;
    uint8_t lengthHi;
    uint8_t lengthLo;
    uint8_t data[DMX_CHANNEL_SIZE];
};

#endif
