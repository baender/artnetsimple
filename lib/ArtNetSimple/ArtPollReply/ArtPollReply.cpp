#include "ArtPollReply.h"

void ArtPollReplyHandler::setUniverse(uint16_t universe15Bit) {
    _artPollReply.netSwitch = (universe15Bit >> 8) & 0x7F;
    _artPollReply.subSwitch = (universe15Bit >> 4) & 0x0F;
}

void ArtPollReplyHandler::setEndpoint(uint8_t portNr, uint16_t universe15Bit, ART_PORT_TYPE_DIRECTION direction,
                                      ART_PORT_TYPE_PROTOCOL protocol, ART_GOOD_INPUT goodInput,
                                      ART_GOOD_OUTPUTA goodOutputA) {
    if (portNr >= ARTNET_ENDPOINTS) {
        return;
    }
    // _callback = callback;
    _artPollReply.portTypes[portNr] = static_cast<uint8_t>(direction) | static_cast<uint8_t>(protocol);
    _artPollReply.goodInput[portNr] = static_cast<uint8_t>(goodInput);
    _artPollReply.goodOutputA[portNr] = static_cast<uint8_t>(goodOutputA);
    uint8_t universe4Bit = (universe15Bit + portNr) & 0x0F;
    switch (direction) {
        case ART_PORT_TYPE_DIRECTION::SEND_ONLY:
            _artPollReply.swIn[portNr] = universe4Bit;
            // _artPollReply.swOut[portNr] = universe4Bit;  // DELETE AFTER DEBUGGING
            // _callbacks[portNr] = callback;
            break;
        case ART_PORT_TYPE_DIRECTION::RECEIVE_ONLY:
            // _artPollReply.swIn[portNr] = universe4Bit;  // DELETE AFTER DEBUGGING
            _artPollReply.swOut[portNr] = universe4Bit;
            // _callbacks[portNr] = callback;
            break;
        case ART_PORT_TYPE_DIRECTION::SEND_AND_RECEIVE:
            _artPollReply.swIn[portNr] = universe4Bit;
            _artPollReply.swOut[portNr] = universe4Bit;
            // _callbacks[portNr] = callback;
            break;
    }
}

bool ArtPollReplyHandler::loadPacketToBuffer(uint8_t* buffer, size_t size) {
    if (size < _size) {
        return false;
    }
    memcpy(buffer, &_artPollReply, _size);
    return true;
}

void ArtPollReplyHandler::setNodeName(const char* shortName, const char* longName) {
    strncpy(_artPollReply.portName, shortName, sizeof(_artPollReply.portName) - 1);
    _artPollReply.portName[sizeof(_artPollReply.portName) - 1] = '\0';
    strncpy(_artPollReply.longName, longName, sizeof(_artPollReply.longName) - 1);
    _artPollReply.longName[sizeof(_artPollReply.longName) - 1] = '\0';
}

// PORT TYPES
// void ArtPollReplyHandler::setDirection(uint8_t portNr,
// ART_PORT_TYPE_DIRECTION direction) {
//     if (portNr >= ARTNET_MAX_PORTS) { return; }
//     uint8_t value = _artPollReply.portTypes[portNr];
//     value = (value & 0x3F) | static_cast<uint8_t>(direction);
//     _artPollReply.portTypes[portNr] = value;
// }
// void ArtPollReplyHandler::setProtocol(uint8_t portNr, ART_PORT_TYPE_PROTOCOL
// protocol) {
//     if (portNr >= ARTNET_MAX_PORTS) { return; }
//     uint8_t value = _artPollReply.portTypes[portNr];
//     value = (value & 0xC0) | static_cast<uint8_t>(protocol);
//     _artPollReply.portTypes[portNr] = value;
// }

// void ArtPollReplyHandler::setSwIn(uint8_t portNr, uint16_t universe15Bit) {
//     if (portNr >= ARTNET_MAX_PORTS) { return; }
//     _artPollReply.swIn[portNr] = universe15Bit & 0x0F;
// }

// void ArtPollReplyHandler::setSwOut(uint8_t portNr, uint16_t universe15Bit) {
//     if (portNr >= ARTNET_MAX_PORTS) { return; }
//     _artPollReply.swOut[portNr] = universe15Bit & 0x0F;
// }