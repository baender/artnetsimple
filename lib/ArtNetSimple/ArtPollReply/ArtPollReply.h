#ifndef _ART_POLL_REPLY_H_
#define _ART_POLL_REPLY_H_

#include <Arduino.h>

#include "Common.h"

enum class ART_STATUS1_INDICATOR_STATE : uint8_t {
    NORMAL = 0xC0,
    MUTE = 0x80,
    IDENTIFY = 0x40,
    UNKNOWN = 0x00,
};
enum class ART_STATUS1_ADDRESS_AUTHORITY : uint8_t {
    NOT_USED = 0x0C,
    PROGRAMMED = 0x08,
    PANEL = 0x04,
    UNKNOWN = 0x00,
};

enum class ART_PORT_TYPE_DIRECTION : uint8_t {
    INPUT_OUTPUT = 0xC0,
    INPUT_ONLY = 0x80,
    OUTPUT_ONLY = 0x40,
};
enum class ART_PORT_TYPE_PROTOCOL : uint8_t {
    DALI = 0x06,
    ART_NET = 0x05,
    ADB_62_5 = 0x04,
    COLORTRAN_CMX = 0x03,
    AVAB = 0x02,
    MIDI = 0x01,
    DMX512 = 0x00,
};

enum class ART_GOOD_INPUT : uint8_t {
    DATA_RECEIVED = 0x80,
    INCLUDES_DMX512_TEST_PACKETS = 0x40,
    INCLUDES_DMX512_SIP = 0x20,
    INCLUDES_DMX512_TEXT_PACKETS = 0x10,
    INPUT_DISABLED = 0x08,
    RECEIVE_ERROR = 0x04,
    // UNUSED =                       0x02,
    CONVERT_TO_SACN = 0x01,
    NONE = 0x00,
};
inline ART_GOOD_INPUT operator|(ART_GOOD_INPUT lhs, ART_GOOD_INPUT rhs) {
    return static_cast<ART_GOOD_INPUT>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

enum class ART_GOOD_OUTPUTA : uint8_t {
    CONVERT_TO_DMX512 = 0x80,
    INCLUDES_DMX512_TEST_PACKETS = 0x40,
    INCLUDES_DMX512_SIP = 0x20,
    INCLUDES_DMX512_TEXT_PACKETS = 0x10,
    MERGE_ARTNET = 0x08,
    SHORT_DETECTED = 0x04,
    MERGE_MODE_LTP = 0x02,
    CONVERT_FROM_SACN = 0x01,
    NONE = 0x00,
};
inline ART_GOOD_OUTPUTA operator|(ART_GOOD_OUTPUTA lhs, ART_GOOD_OUTPUTA rhs) {
    return static_cast<ART_GOOD_OUTPUTA>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

// enum STYLE : uint8_t {
//     VISUAL =     0x06,
//     CONFIG =     0x05,
//     BACKUP =     0x04,
//     ROUTE =      0x03,
//     MEDIA =      0x02,
//     CONTROLLER = 0x01,
//     NODE =       0x00,
// };

// enum STATUS2 : uint8_t {
//     SUPPORT_RDM_CONTROL =         0x80,
//     SUPPORT_SWITCH_OUTPUT_STYLE = 0x40,
//     SQUAWKING =                   0x20,
//     SUPPORT_SWITCH_ARTNET_SACN =  0x10,
//     SUPPORT_15BIT_ADDRESS =       0x08,
//     SUPPORT_DHCP =                0x04,
//     CONIFGURED_DHCP =             0x02,
//     SUPPORT_WEBBROWSER =          0x01,
// };

// enum GOOD_OUTPUTB : uint8_t {
//     RDM_DISABLED =                  0x80,
//     STYLE_CONTINUOUS =              0x40,
//     DISCOVERY_NOT_RUNNING =         0x20,
//     BACKGROUND_DISCOVERY_DISABLED = 0x10,
// };

// enum STATUS3 : uint8_t {
//     FAILSAFE_PLAYBACK_SCENE =       0xC0,
//     FAILSAFE_ALL_OUTPUTS_FULL =     0x80,
//     FAILSAFE_ALL_OUTPUTS_ZERO =     0x40,
//     FAILSAFE_HOLD_LAST_STATE =      0x00,
//     SUPPORT_PROGRAMMABLE_FAILSAFE = 0x20,
//     SUPPORT_LLRP =                  0x10,
//     SUPPORT_SWITCHING_PORT_SWAP =   0x08,
//     SUPPORT_RDM_NET =               0x04,
//     SUPPORT_BACKGROUND_QUEUE =      0x02,
//     SUPPORT_BACKGROUND_DISCOVERY =  0x01,
// };

enum class ART_DIAGNOSTIC_PRIORITY : uint8_t {
    DP_VOLATILE = 0xF0,
    DP_CRITICAL = 0xE0,
    DP_HIGH = 0x80,
    DP_MEDIUM = 0x40,
    DP_LOW = 0x20,
};

struct ArtPoll {
    ArtHeader header;
    uint8_t flags;
    uint8_t diagPriority;
    uint8_t targetPortAddressTopHi;  // Optional fields starting with this one
    uint8_t targetPortAddressTopLo;
    uint8_t targetPortAddressBottomHi;
    uint8_t targetPortAddressBottomLo;
    uint8_t estaManHi;
    uint8_t estaManLo;
    uint8_t OemHi;
    uint8_t OemLo;
};

/**
 * @brief Unicast reply to ArtPoll sent by controller (also unsolicited on change in configuration)
 *
 * A device, in response to a Controller’s ArtPoll, sends the ArtPollReply. The device should wait for a random delay of
 * up to 1s before sending the reply. This mechanism is intended to reduce packet bunching when scaling up to very large
 * systems.
 */

struct ArtPollReply {
    const char id[8] = "Art-Net";
    const uint16_t opCode = static_cast<uint16_t>(OpCode::PollReply);
    uint8_t ipAddress[4];
    const uint16_t port = 0x1936;
    const uint8_t versionInfoH = 0x00;
    const uint8_t versionInfoL = 14;  // Version of Art-Net (???). Current version is 14
    uint8_t
        netSwitch;  // Bits 14-8 of the 15 bit Port-Address are encoded into the bottom 7 bits of this field. This is
                    // used in combination with SubSwitch and SwIn[] or SwOut[] to produce the full universe address.
    uint8_t
        subSwitch;  //  Bits 7-4 of the 15 bit Port-Address are encoded into the bottom 4 bits of this field. This is
                    //  used in combination with NetSwitch and SwIn[] or SwOut[] to produce the full universe address.
    const uint8_t oemHi = 0x00;
    const uint8_t oem = 0xFF;
    const uint8_t ubeaVersion = 0;  // This field contains the firmware version of the User Bios Extension Area (UBEA).
                                    // If the UBEA is not programmed, this field contains zero.
    uint8_t status1;
    uint8_t estaManLo;
    uint8_t estaManHi;
    char portName[18] = "Art-Net Node";
    char longName[64] = "Generic Art-Net Node";
    char nodeReport[64] = "#0000 [12345] Dummy report";
    const uint8_t numPortsHi = 0x00;  // Can be ignored by nodes; ports are passed by array of 4 in next fields
    const uint8_t numPortsLo = 0x00;
    uint8_t portTypes[ARTNET_ENDPOINTS] = {0};
    uint8_t goodInput[ARTNET_ENDPOINTS] = {0};
    uint8_t goodOutputA[ARTNET_ENDPOINTS] = {0};
    uint8_t swIn[ARTNET_ENDPOINTS] = {0};
    uint8_t swOut[ARTNET_ENDPOINTS] = {0};
    uint8_t acnPriority;
    uint8_t swMacro;
    uint8_t swRemote;
    const uint8_t spare[3] = {0};
    uint8_t style;
    uint8_t macAddress[6];
    // uint8_t bindIp[4];
    // uint8_t bindIndex;
    // uint8_t status2;
    // uint8_t goodOutputB[ARTNET_ENDPOINTS];
    // uint8_t status3;
    // uint8_t DefaulRespUID[6];               // MSB first (big-endian)
    // uint8_t user[2];                        // MSB first (big-endian)
    // uint8_t refreshRate[2] = {0x00, 0x10};  // MSB first (big-endian); Maximum refresh rate in Hertz; value between
    // 0-44 uint8_t BackgroundQueuePolicy; uint8_t filler[10] = {0};
};

class ArtPollReplyHandler {
   public:
    void setNodeName(const char* shortName, const char* longName);
    void setUniverse(uint16_t universe15Bit);
    void setEndpoint(uint8_t portNr, uint16_t universe15Bit, ART_PORT_TYPE_DIRECTION direction,
                     ART_PORT_TYPE_PROTOCOL protocol, ART_GOOD_INPUT goodInput = ART_GOOD_INPUT::NONE,
                     ART_GOOD_OUTPUTA goodOutputA = ART_GOOD_OUTPUTA::NONE, ArtCallback callback = nullptr);
    // STATUS1
    inline void setIndicator(ART_STATUS1_INDICATOR_STATE indicator) {
        _artPollReply.status1 = (_artPollReply.status1 & 0x3F) | static_cast<uint8_t>(indicator);
    }
    inline void setAddressAuthority(ART_STATUS1_ADDRESS_AUTHORITY addressAuthority) {
        _artPollReply.status1 = (_artPollReply.status1 & 0xC0) | static_cast<uint8_t>(addressAuthority);
    }

   protected:
    bool loadPacketToBuffer(uint8_t* buffer, size_t size);
    inline void setIpAddress(uint32_t& localIpAddress) {
        memcpy(_artPollReply.ipAddress, &localIpAddress, sizeof(_artPollReply.ipAddress));
    }
    inline void setMacAddress(uint8_t (&macAddress)[6]) {
        memcpy(_artPollReply.macAddress, macAddress, sizeof(_artPollReply.macAddress));
    }

   private:
    ArtPollReply _artPollReply;
    static constexpr size_t _size = sizeof(ArtPollReply);
};

#endif
