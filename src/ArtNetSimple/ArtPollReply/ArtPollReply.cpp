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
            break;
        case ART_PORT_TYPE_DIRECTION::RECEIVE_ONLY:
            _artPollReply.swOut[portNr] = universe4Bit;
            break;
        case ART_PORT_TYPE_DIRECTION::SEND_AND_RECEIVE:
            _artPollReply.swIn[portNr] = universe4Bit;
            _artPollReply.swOut[portNr] = universe4Bit;
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
