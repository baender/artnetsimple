# ArtNetSimple

ArtNetSimple is a lightweight library that brings ArtNet protocol support to Arduino-like platforms. Designed with flexibility in mind, this library allows you to dynamically switch between different network interfaces (e.g., from WiFi to Ethernet) during runtime—making it a versatile choice for modern ArtNet projects. 

Currently, the library is in its early stages of development and has been tested primarily on the ESP32 platform. Contributions and feedback are welcome to help expand its functionality and compatibility.

---

## Features

- **Dynamic Network Switching**: Seamlessly move between network types at runtime.
- **ArtNet Protocol Support**: Basic ArtNet features are implemented to facilitate DMX lighting control and data reception.
- **ESP32 Support**: Tested on ESP32, with potential for compatibility with other Arduino-like platforms.
- **Lightweight & Simple**: Designed to be straightforward to use and easy to integrate into your projects.

---

## Getting Started

### Installation

1. Clone or download the repository.
2. Place the library folder in your libraries directory.
3. Include `ArtNetSimple.h` in your project. 

---

## Minimal Example

Here’s a basic example of how to use the library for receiving ArtNet Data.
Create a `secrets.h` file in the `include` folder and add the following content:

```cpp
#ifndef _SECRETS_H_
#define _SECRETS_H_

constexpr char WIFI_SSID[] = "YOUR_SSID";
constexpr char WIFI_PASSWORD[] = "YOUR_PASSWORD";

#endif
```

Then use the following code in your `src/main.cpp`:

```cpp
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
    artnet.setNodeName("ArtNet Node", "ArtNet Node by Baender");
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
```

## License

This project is licensed under the MIT License, making it open and permissive. Feel free to use, modify, and distribute the library, as long as proper credit is given. See the [LICENSE](LICENSE) file for details.
