#include <WiFi.h>
#include <WiFiUdp.h>

#include "ArtNetSimple.h"
#include "secrets.h"

ArtNetSimple artnet;

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

WiFiUDP udp;

void callback(const uint8_t* data, uint16_t size, uint16_t universe, uint8_t sequence) {
    Serial.print("Size: ");
    Serial.println(size);
    Serial.print("Universe: ");
    Serial.println(universe);
    Serial.print("Sequence: ");
    Serial.println(sequence);
    Serial.println("Data: ");
    for (uint16_t i = 0; i < size; i++) {
        Serial.print("0x");
        if (data[i] < 0x10) {
            Serial.print("0");
        }
        Serial.print(data[i], HEX);
        if (i % 32 == 31) {
            Serial.println();
        } else {
            Serial.print(" ");
        }
    }
    Serial.println();
    Serial.println();
}

void configureArtNet() {
    uint16_t universe15Bit = 0x0001;
    artnet.setCallback(callback);
    artnet.setNetwork(&WiFi, &udp);
    artnet.setIndicator(ART_STATUS1_INDICATOR_STATE::NORMAL);
    artnet.setAddressAuthority(ART_STATUS1_ADDRESS_AUTHORITY::PROGRAMMED);
    artnet.setUniverse(universe15Bit);
    artnet.setEndpoint(0, universe15Bit, ART_PORT_TYPE_DIRECTION::RECEIVE_ONLY, ART_PORT_TYPE_PROTOCOL::DMX512,
                       ART_GOOD_INPUT::NONE, ART_GOOD_OUTPUTA::CONVERT_TO_DMX512);
    artnet.setNodeName("TriggerCube", "TriggerCube by Baender");
}

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("--- START ---");

    // Connect to Wi-Fi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    // Start UDP listener
    udp.begin(ARTNET_DEFAULT_PORT);

    configureArtNet();

    Serial.println("\nConnected to Wi-Fi");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Listening on UDP port ");
    Serial.println(ARTNET_DEFAULT_PORT);
    Serial.println();
    Serial.println();
}

void loop() {
    ArtNetStatus status;
    status = artnet.parse();

    if ((status != ArtNetStatus::NO_PACKET_RECEIVED) && (status != ArtNetStatus::OK)) {
        Serial.println();
        Serial.print("Code: ");
        Serial.println(static_cast<uint8_t>(status));
        Serial.println();
    }
    delay(1);
}
