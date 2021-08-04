/* HCDE 498/598 Assignment A4

    Starter Code

    For this assignment, you should Find two (2) APIs that you can
    access via the HTTPClient and capture that data for use on your
    ESP. The data should be retrieved as a JSON object if possible,
    and can be presented as textual narrative in the Serial Monitor,
    similar to the sketches presented in class.

    A config.h file is included so you can provide your secret key
    and network credentials.

    Add your code below.
*/
#include "config.h"
#include "WiFi.h"
#include <PubSubClient.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h> //adding for https

WiFiClient espClient;             //espClient
PubSubClient mqtt(espClient);     //tie PubSub (mqtt) client to WiFi client

#define ARDUINOJSON_DECODE_UNICODE 1
#include <ArduinoJson.h> //provides the ability to parse and construct JSON objects

int sensorValue;

char clientID[6]; //A MAC address is a 'truly' unique ID for each device, lets use that as our 'truly' unique user ID!!!


//here we're creating several new data type definition; a box to hold other data types
typedef struct {
  String cy;
} GeoData;     //then we give our new data structure a name so we can use it in our code

GeoData location; //we have created a GeoData type, but not an instance of that type,
//so we create the variable 'location' of type GeoData

typedef struct {
  String wth;
} WeatherData;     //then we give our new data structure a name so we can use it in our code

WeatherData desc; //we have created a GeoData type, but not an instance of that type,
//so we create the variable 'location' of type GeoData



void setup() {
  Serial.begin(115200);
  delay(10);


  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(); Serial.println("WiFi connected"); Serial.println();

  getGeo();
  getWeather();

  Serial.print("Your ESP is currently in or near " + location.cy + " \n");

  setup_wifi();
  mqtt.setServer(mqtt_server, mqtt_port); // setup the MQTT object
  //  mqtt.setCallback(callback);             // to receive MQTT messages we need to register a callback function
}



void loop() {
  // put your main code here, to run repeatedly:

  mqtt.loop(); //this keeps the mqtt connection 'active'
  
  //Let's get a reading off our light sensor & send to the serial monitor for viewing
//  sensorValue = analogRead(A1); // read analog input pin A1
//  Serial.print(sensorValue, DEC); // prints the value read
//  Serial.print(" \n"); // prints a space between the numbers
//  delay(10000); // wait 100ms for next reading

  ////let's check on our light meter
    sendLightReading();

  // ESP.deepSleep(60e6); //depending on how things work, might use this to help save battery life!

  //here's where we're sorting out our MQTT packet
  if (!mqtt.connected()) {
    reconnect();
  }
} //end loop

// we're gonna use the same trick we used in A2 to grab the city name to pass to the weather app
String getIP() {
  HTTPClient theClient;
  String ipAddress;

  theClient.begin("http://api.ipify.org/?format=json");
  int httpCode = theClient.GET();

  if (httpCode > 0) {
    if (httpCode == 200) {
      StaticJsonDocument<100> doc;
      String payload = theClient.getString();
      //   JsonObject& root = jsonBuffer.parse(payload);
      deserializeJson(doc, payload);
      ipAddress = doc["ip"].as<String>();

    } else {
      Serial.println("Something went wrong with connecting to the location endpoint.");
      return "error";
    }
  }
  Serial.println(ipAddress);
  return ipAddress;
} //end getIP



String getGeo() {
  HTTPClient theClient;
  Serial.println("Making HTTP request");
  theClient.begin("http://api.ipstack.com/" + getIP() + "?access_key=" + key); //return city as .json object
  // http://api.ipstack.com/97.113.62.196?access_key=...

  int httpCode = theClient.GET();

  if (httpCode > 0) {
    if (httpCode == 200) {
      Serial.println("Received HTTP payload.");
      //   alternatively use:  DynamicJsonDocument doc(1024); // specify JSON document and size(1024)
      StaticJsonDocument<1024> doc;
      String payload = theClient.getString();
      Serial.println("Parsing...");
      deserializeJson(doc, payload);

      DeserializationError error = deserializeJson(doc, payload);
      // Test if parsing succeeds.
      if (error) {
        Serial.print("deserializeJson() failed with error code ");
        Serial.println(error.c_str());
        Serial.println(payload);
//        return;
      }

      //Some debugging lines below:
      Serial.println(payload);
      //      root.printTo(Serial);

      //Using .dot syntax, we refer to the variable "location" which is of
      //type GeoData, and place our data into the data structure.

      location.cy = doc["city"].as<String>();

    } else {
      Serial.println("Something went wrong with connecting to the location endpoint.");
    }
  }
} //end getGeo

//Here we can summon a weather description
void getWeather() {
  HTTPClient theClient;
  Serial.println("Making HTTP request");
  theClient.begin("http://api.openweathermap.org/data/2.5/weather?q=" + location.cy + "&APPID=" + key2); //querying for the weather
  // api.openweathermap.org/data/2.5/weather?q={city}&APPID=...

  int httpCode = theClient.GET();

  if (httpCode > 0) {
    if (httpCode == 200) {
      Serial.println("Received HTTP payload.");
      //   alternatively use:  DynamicJsonDocument doc(1024); // specify JSON document and size(1024)
      StaticJsonDocument<1024> doc;
      String payload = theClient.getString();
      Serial.print("http://api.openweathermap.org/data/2.5/weather?q=" + location.cy + "&APPID=" + key);
      Serial.println("Testing...");
      deserializeJson(doc, payload);
      JsonArray repos = doc["weather"];
      
      DeserializationError error = deserializeJson(doc, payload);
      // Test if parsing succeeds.
      if (error) {
        Serial.print("deserializeJson() failed with error code ");
        Serial.println(error.c_str());
        Serial.println(payload);
        return;
      }

      //Some debugging lines below:
      Serial.println(payload);
      //      root.printTo(Serial);

      //Using .dot syntax, we refer to the variable "location" which is of
      //type GeoData, and place our data into the data structure.
      for (JsonObject repo: repos) {
      desc.wth = repo["description"].as<String>();
      }
    } else {
      Serial.println("Something went wrong with connecting to the location endpoint.");
    }
  }
}// end getWeather


//OK, so the important thing is to switch the light sensor reading into a binary of Open/Closed before we send all this info off

void sendLightReading() {
    //Let's get a reading off our light sensor & send to the serial monitor for viewing
  Serial.print("Checking.... \n");
  sensorValue = analogRead(A1); // read analog input pin A1
  Serial.print(sensorValue, DEC); // prints the value read
  Serial.print(" \n"); // prints a space between the numbers
  if (sensorValue > -1) {
    Serial.println("Mailbox was opened!");
    Serial.println("The weather outside is " + desc.wth);
    StaticJsonDocument<256> outputDoc;
    outputDoc["Mailbox"] = sensorValue;
    outputDoc["Weather"] = desc.wth;
    char buffer[256];
    serializeJson(outputDoc, buffer);
    mqtt.publish(feed1, buffer);
  } else {
    Serial.print("No light? \n");
  }
  delay(10000); // wait for next reading

}//end getLightReading

/////CONNECT/RECONNECT/////Monitor the connection to MQTT server, if down, reconnect
void reconnect() {
  // Loop until we're reconnected
  while (!mqtt.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqtt.connect(clientID, mqtt_user, mqtt_password)) { //<<---using MAC as client ID, always unique!!!
      Serial.println("connected");
      mqtt.subscribe(feed1);  // Subscribe to the feeds.
      mqtt.subscribe(feed2);  // You could also do this in a single line by subscribing to all feeds
    } else {                        //please change 'theTopic' to reflect your topic you are subscribing to
      Serial.print("failed, rc=");
      Serial.print(mqtt.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
} //end reconnect

/////SETUP_WIFI/////
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");  //get the unique MAC address to use as MQTT client ID, a 'truly' unique ID.
  Serial.println(WiFi.macAddress());  //.macAddress() returns a byte array 6 bytes representing the MAC address
  String temp = WiFi.macAddress();
  temp.toCharArray(clientID, 6);
}  //end setup_wifi
