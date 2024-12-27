#include <WiFi.h>
#include <WiFiUdp.h>

#include "ArtNetSimple.h"

ArtNetSimple artnet;

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

WiFiUDP udp;

void configureArtNet() {
    // uint16_t universe15Bit = 0x0321;
    uint16_t universe15Bit = 0x0000;
    artnet.setNetwork(&WiFi, &udp);
    artnet.setIndicator(ART_STATUS1_INDICATOR_STATE::NORMAL);
    artnet.setAddressAuthority(ART_STATUS1_ADDRESS_AUTHORITY::UNKNOWN);
    artnet.setUniverse(universe15Bit);
    artnet.setEndpoint(0, universe15Bit, ART_PORT_TYPE_DIRECTION::INPUT_ONLY, ART_PORT_TYPE_PROTOCOL::DMX512,
                       ART_GOOD_INPUT::DATA_RECEIVED);
    artnet.setNodeName("TriggerCube", "TriggerCube by Baender");
}

void setup() {
    Serial.begin(115200);
    // while (!Serial) {
    // }

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to Wi-Fi");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Start UDP listener
    udp.begin(ARTNET_DEFAULT_PORT);

    configureArtNet();
    Serial.print("Listening on UDP port ");
    Serial.println(ARTNET_DEFAULT_PORT);
}

void loop() {
    ArtNetStatus status;
    status = artnet.parse();

    if (status != ArtNetStatus::NO_PACKET_RECEIVED) {
        Serial.println();
        Serial.print("Code: ");
        Serial.println(static_cast<uint8_t>(status));
        Serial.println();
    }
    // int packetSize = udp.parsePacket();
    // if (packetSize) {
    //     Serial.print("Received packet of size ");
    //     Serial.println(packetSize);

    //     // Read packet into buffer
    //     char packetBuffer[512]; // Adjust size for your needs
    //     int len = udp.read(packetBuffer, sizeof(packetBuffer) - 1);
    //     if (len > 0) {
    //     packetBuffer[len] = '\0';
    //     }

    //     Serial.print("Received UDP packet: ");
    //     Serial.println(packetBuffer);
    // }
}
