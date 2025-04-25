#ifndef _ARTNETSIMPLE_H_
#define _ARTNETSIMPLE_H_

#include <Arduino.h>

#include "ArtDmx/ArtDmx.h"
#include "ArtPollReply/ArtPollReply.h"
#include "Common.h"

// will be replaced by interface
#include <WiFi.h>
#include <WiFiUdp.h>

enum class ArtNetStatus : uint8_t {
    OK = 0,
    NO_PACKET_RECEIVED = 1,
    ID_NOT_FOUND = 2,
    INCOMPATIBLE_PROTOCOL_VERSION = 3,
    UNSUPPORTED = 4,
    BUFFER_TOO_SMALL = 5,
    PACKET_SIZE_MISMATCH = 6,
};

class ArtNetSimple : public ArtPollReplyHandler {
   public:
    // void begin(WiFiClass* network, WiFiUDP* udp);
    // void end();
    void setNetwork(WiFiClass* network, WiFiUDP* udp);
    inline void setCallback(ArtDmxCallback artDmxCallback) { _artDmxCallback = artDmxCallback; };
    ArtNetStatus parse();

   private:
    ArtNetStatus handlePoll();
    ArtNetStatus handleDmx();
    // ArtNetStatus handleNzs();
    // ArtNetStatus handleTrigger();
    // ArtNetStatus handleSync();
    void sendPacket(uint8_t* packet, size_t size);

    uint32_t convertIpAddress(IPAddress ipAddress);
    // bool isNetworkReady();
    // const uint8_t* getArtDmxData() const;
    // uint16_t getArtDmxUniverse15bit() const;

    uint8_t _buffer[BUFFER_SIZE] = {0};
    size_t _packetSize;
    ArtDmxCallback _artDmxCallback = nullptr;

    WiFiClass* _network = nullptr;
    WiFiUDP* _udp = nullptr;
};

#endif
