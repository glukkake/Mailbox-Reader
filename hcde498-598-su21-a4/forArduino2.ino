//#include "WiFi.h"        // Use this line for ESP32
//// #include <ESP8266WiFi.h>    // Use this line for ESP8266
//#include <PubSubClient.h>   //
//#include <ArduinoJson.h>    //
//
//#include "config.h" // include the configuration of credentials in the config.h file
//
//
//WiFiClient espClient;             //espClient
//PubSubClient mqtt(espClient);     //tie PubSub (mqtt) client to WiFi client
//
//
///*
// We need a 'truly' unique client ID for our esp8266, all client names on the server must be unique.
// Every device, app, other MQTT server, etc that connects to an MQTT server must have a unique client ID.
// This is the only way the server can keep every device separate and deal with them as individual devices/apps.
// The client ID is unique to the device.
//*/
//char clientID[6]; //A MAC address is a 'truly' unique ID for each device, lets use that as our 'truly' unique user ID!!!
//
///////SETUP/////
//void setup() {
//  Serial.begin(115200);
//
//  Serial.println();
//  Serial.print("This board is running: ");
//  Serial.println(F(__FILE__));
//  Serial.print("Compiled: ");
//  Serial.println(F(__DATE__ " " __TIME__));
//
//  setup_wifi();
//  mqtt.setServer(mqtt_server, mqtt_port); // setup the MQTT object
//  mqtt.setCallback(callback);             // to receive MQTT messages we need to register a callback function
//}
//
///////LOOP/////
//void loop() {
//
//} //end Loop
//
