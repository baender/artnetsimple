#include "ArtDmx.h"

// ArtNetDmxMetadata getMetadataFromPacket(const uint8_t &packet) {
//     ArtNetDmxMetadata metadata;
//     metadata.sequence = packet[ArtNetDmxIndex::SEQUENCE];
//     metadata.physical = packet[ArtNetDmxIndex::PHYSICAL];
//     metadata.net = packet[ArtNetDmxIndex::NET];
//     metadata.subnet = (packet[ArtNetDmxIndex::SUBUNI] >> 4) & 0x0F;
//     metadata.universe = (packet[ArtNetDmxIndex::SUBUNI] >> 0) & 0x0F;
//     return metadata;
// }

// void setPacketMetadata(uint8_t (&packet)[PACKET_SIZE], ArtNetDmxMetadata metadata) {
//     setPacketMetadata(packet, metadata.sequence, metadata.physical, metadata.net, metadata.subnet,
//     metadata.universe);
// }

// void setPacketMetadata(uint8_t (&packet)[PACKET_SIZE], uint8_t sequence, uint8_t physical, uint8_t net, uint8_t
// subnet, uint8_t universe) {
//     memcpy(packet + ArtNetDmxIndex::ID, ARTNET_ID, sizeof(ARTNET_ID));
//     packet[ArtNetDmxIndex::OP_CODE_L] = (static_cast<uint16_t>(OpCode::Dmx) >> 0) & 0x00FF;
//     packet[ArtNetDmxIndex::OP_CODE_H] = (static_cast<uint16_t>(OpCode::Dmx) >> 8) & 0x00FF;
//     packet[ArtNetDmxIndex::PROTOCOL_VER_H] = (PROTOCOL_VER >> 8) & 0x00FF;
//     packet[ArtNetDmxIndex::PROTOCOL_VER_L] = (PROTOCOL_VER >> 0) & 0x00FF;
//     packet[ArtNetDmxIndex::SEQUENCE] = sequence;
//     packet[ArtNetDmxIndex::PHYSICAL] = physical & 0x03;
//     packet[ArtNetDmxIndex::NET] = net & 0x7F;
//     packet[ArtNetDmxIndex::SUBUNI] = ((subnet & 0x0F) << 4) | (universe & 0x0F);
//     packet[ArtNetDmxIndex::LENGTH_H] = (CHANNEL_SIZE >> 8) & 0xFF;
//     packet[ArtNetDmxIndex::LENGTH_L] = (CHANNEL_SIZE >> 0) & 0xFF;
// }

// void setPacketData(uint8_t (&packet)[PACKET_SIZE], const uint8_t* const data, uint16_t size) {
//     memcpy(packet + ArtNetDmxIndex::DATA, data, size);
// }

// void setPacketData(uint8_t (&packet)[PACKET_SIZE], uint16_t channelOffset, uint8_t data) {
//     packet[ArtNetDmxIndex::DATA + channelOffset] = data;
// }
