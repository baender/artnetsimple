#include "ArtNetSimple.h"

void ArtNetSimple::setNetwork(WiFiClass* network, WiFiUDP* udp) {
    _network = network;
    _udp = udp;

    IPAddress ipAddress = _network->localIP();
    uint32_t ipAddressUint32 = convertIpAddress(ipAddress);
    setIpAddress(ipAddressUint32);

    uint8_t macAddress[6];
    _network->macAddress(macAddress);
    setMacAddress(macAddress);
}

ArtNetStatus ArtNetSimple::parse() {
    _packetSize = _udp->parsePacket();
    if (_packetSize == 0) {
        return ArtNetStatus::NO_PACKET_RECEIVED;
    }

    _udp->readBytes(&(_buffer[0]), _packetSize);

    ArtHeader* artHeader = reinterpret_cast<ArtHeader*>(_buffer);
    if (strncmp(artHeader->id, ARTNET_ID, sizeof(ARTNET_ID)) != 0) {
        return ArtNetStatus::ID_NOT_FOUND;
    }

    if ((static_cast<uint16_t>(artHeader->protocolVersionHi) << 8) | artHeader->protocolVersionLo != PROTOCOL_VERSION) {
        return ArtNetStatus::INCOMPATIBLE_PROTOCOL_VERSION;
    }

    ArtNetStatus status;
    switch (static_cast<OpCode>(artHeader->opCode)) {
        case OpCode::Poll: {
            status = handlePoll();
            break;
        }
        case OpCode::Dmx: {
            status = handleDmx();
            break;
        }
        case OpCode::Nzs: {
            // handleNzs();
            status = ArtNetStatus::UNSUPPORTED;
            break;
        }
        case OpCode::Trigger: {
            // handleTrigger();
            status = ArtNetStatus::UNSUPPORTED;
            break;
        }
        case OpCode::Sync: {
            // handleSync();
            status = ArtNetStatus::UNSUPPORTED;
            break;
        }
        default: {
            status = ArtNetStatus::UNSUPPORTED;
            break;
        }
    }
    _udp->flush();
    return status;
}

ArtNetStatus ArtNetSimple::handlePoll() {
    if ((_packetSize < sizeof(ArtHeader) + sizeof(ArtPoll::flags) + sizeof(ArtPoll::diagPriority)) ||
        (_packetSize > sizeof(ArtPoll))) {
        return ArtNetStatus::PACKET_SIZE_MISMATCH;
    }
    if (!ArtPollReplyHandler::loadPacketToBuffer(_buffer, sizeof(ArtPollReply))) {
        return ArtNetStatus::BUFFER_TOO_SMALL;
    }
    delay(random(1000));  // Defined by the Art-Net specifications to reduce packet bunching
    sendPacket(_buffer, sizeof(ArtPollReply));
    return ArtNetStatus::OK;
}

ArtNetStatus ArtNetSimple::handleDmx() {
    if (_packetSize > sizeof(ArtDmx)) {
        return ArtNetStatus::PACKET_SIZE_MISMATCH;
    }
    ArtDmx* artDmx = reinterpret_cast<ArtDmx*>(_buffer);
    uint16_t dmxLength = (static_cast<uint16_t>(artDmx->lengthHi) << 8) | artDmx->lengthLo;
    uint16_t universe15Bit = static_cast<uint16_t>(artDmx->net) << 8 | artDmx->subUni;

    if (_packetSize != sizeof(ArtHeader) + 6 + dmxLength) {  // Find better solution than hardcode 6
        return ArtNetStatus::PACKET_SIZE_MISMATCH;
    }

    if (_artDmxCallback) {
        _artDmxCallback(artDmx->data, dmxLength, universe15Bit, artDmx->sequence);
    }

    return ArtNetStatus::OK;

    // ArtNetDmxMetadata metadata = getMetadataFromPacket(_packet);
    // if (callback_art_dmx) {
    //     callback_art_dmx(getArtDmxData(), size - HEADER_SIZE, metadata,
    //     remote_info);
    // }
    // for (auto& cb : callback_art_dmx_universes) {
    //     if (getArtDmxUniverse15bit() == cb.first) {
    //         cb.second(getArtDmxData(), size - HEADER_SIZE, metadata,
    //         remote_info);
    //     }
    // }
}

// ArtNetStatus ArtNetSimple::handleNzs() {
//     art_nzs::Metadata metadata =
//     art_nzs::generateMetadataFrom(packet.data()); for (auto& cb :
//     callback_art_nzs_universes) {
//         if (getArtDmxUniverse15bit() == cb.first) {
//             cb.second(getArtDmxData(), size - HEADER_SIZE, metadata,
//             remote_info);
//         }
//     }
// }

// ArtNetStatus ArtNetSimple::handleTrigger() {
//     if (callback_art_trigger) {
//         ArtTriggerMetadata metadata = {
//             .oem = getArtTriggerOEM(),
//             .key = getArtTriggerKey(),
//             .sub_key = getArtTriggerSubKey(),
//             .payload = getArtTriggerPayload(),
//             .size = static_cast<uint16_t>(size - art_trigger::PAYLOAD),
//         };
//         callback_art_trigger(metadata, remote_info);
//     }
// }

// ArtNetStatus ArtNetSimple::handleSync() {
//     if (callback_art_sync) {
//         callback_art_sync(remote_info);
//     }
// }

void ArtNetSimple::sendPacket(uint8_t* packet, size_t size) {
    IPAddress remoteIp = _udp->remoteIP();
    uint16_t remotePort = _udp->remotePort();
    _udp->beginPacket(remoteIp, remotePort);
    _udp->write(packet, size);
    _udp->endPacket();
}

uint32_t ArtNetSimple::convertIpAddress(IPAddress ipAddress) {
    uint32_t ip = 0;
    ip |= static_cast<uint32_t>(ipAddress[3]) << 24;
    ip |= static_cast<uint32_t>(ipAddress[2]) << 16;
    ip |= static_cast<uint32_t>(ipAddress[1]) << 8;
    ip |= static_cast<uint32_t>(ipAddress[0]);
    return ip;
}
