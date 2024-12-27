#ifndef _ARTNET_COMMON_H_
#define _ARTNET_COMMON_H_

#include <Arduino.h>

using ArtCallback = void (*)(const uint8_t* data, uint16_t size);

constexpr char ARTNET_ID[] = "Art-Net";
constexpr uint16_t ARTNET_DEFAULT_PORT = 6454;  // 0x1936
constexpr uint16_t PROTOCOL_VERSION = 14;       // 0x000E
constexpr uint8_t ARTNET_ENDPOINTS = 4;
constexpr uint16_t DMX_CHANNEL_SIZE = 512;
constexpr uint16_t BUFFER_SIZE = 530;

struct ArtHeader {
    char id[sizeof(ARTNET_ID)];
    uint16_t opCode;
    uint8_t protocolVersionHi;
    uint8_t protocolVersionLo;
};

enum class OpCode : uint16_t {
    // Device Discovery
    Poll = 0x2000,
    PollReply = 0x2100,
    // Device Configuration
    Address = 0x6000,
    Input = 0x7000,
    IpProg = 0xF800,
    IpProgReply = 0xF900,
    Command = 0x2400,
    // Streaming Control
    Dmx = 0x5000,
    Nzs = 0x5100,
    Sync = 0x5200,
    // RDM
    TodRequest = 0x8000,
    TodData = 0x8100,
    TodControl = 0x8200,
    Rdm = 0x8300,
    RdmSub = 0x8400,
    // Time-Keeping
    TimeCode = 0x9700,
    TimeSync = 0x9800,
    // Triggering
    Trigger = 0x9900,
    // Diagnostics
    DiagData = 0x2300,
    // File Transfer
    FirmwareMaster = 0xF200,
    FirmwareReply = 0xF300,
    Directory = 0x9A00,
    DirectoryReply = 0x9B00,
    FileTnMaster = 0xF400,
    FileFnMaster = 0xF500,
    FileFnReply = 0xF600,
};

#endif
