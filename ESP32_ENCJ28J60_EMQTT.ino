/*
  Web client

 This sketch connects to a website (http://www.google.com)
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe, based on work by Adrian McEwen

 */

#include <SPI.h>
#include <EthernetENC.h>
#include "PubSubClient.h"
// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "www.google.com";    // name address for Google (using DNS)
#define CLIENT_ID       "UNERIA"
#define INTERVAL        3000 // 3 sec delay between publishing
// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 1, 177);
IPAddress myDns(192, 168, 1, 1);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;
PubSubClient mqttClient;
long previousMillis;
// Variables to measure the speed
unsigned long beginMicros, endMicros;
unsigned long byteCount = 0;
bool printWebData = true;  // set to false for better speed measurement

void setup() {
  // You can use Ethernet.init(pin) to configure the CS pin
  //Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip, myDns);
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }
   mqttClient.setClient(client);
   mqttClient.setServer("143.198.153.138",1883);
   Serial.println(F("MQTT client configured"));
previousMillis = millis();
  // give the Ethernet shield a second to initialize:
 

  // if you get a connection, report back via serial:

  beginMicros = micros();
}

void loop() {

   auto link = Ethernet.linkStatus();
  Serial.print("Link status: ");
  switch (link) {
    case Unknown:
      Serial.println("Unknown");
      break;
    case LinkON:
      Serial.println("ON");
      break;
    case LinkOFF:
      Serial.println("OFF");
      break;
  }
  delay(1000);

   if(millis() - previousMillis > INTERVAL) {
    sendData();
    previousMillis = millis();
  }
  mqttClient.loop();
}

void sendData() {
  char msgBuffer[20];
  float h=34;
  float t =34;
  if(mqttClient.connect(CLIENT_ID)) {
   mqttClient.publish("/TEMP", dtostrf(t, 6, 2, msgBuffer));

 //hal=hallway, DICHT=Closed, kamer=room, licht=light
 }
}
