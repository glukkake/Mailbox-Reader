/************************ Adafruit IO Config *******************************/

// visit io.adafruit.com if you need to create an account,
// or if you need your Adafruit IO key.
#define IO_USERNAME "glukkake"
#define IO_KEY "aio_EeFv05g1fKbaix84yo3ATiYdcoLP"

/******************************* WIFI **************************************/

// the AdafruitIO_WiFi client will work with the following boards:
//   - HUZZAH ESP8266 Breakout -> https://www.adafruit.com/products/2471
//   - Feather HUZZAH ESP8266 -> https://www.adafruit.com/products/2821
//   - Feather M0 WiFi -> https://www.adafruit.com/products/3010
//   - Feather WICED -> https://www.adafruit.com/products/3056

#define WIFI_SSID "Nick-Setup"
#define WIFI_PASS "C00lb0x!"
const char* ssid = "Nick-Setup";
const char* pass = "C00lb0x!";
const char* key = "3b8a1738c712ffc033a1599883f807b7";
const char* key2 = "93d04074ceb35f5ab9287e15bf077c24";

// loading to our MQTT server just so I don't need to stress over something additional
#define mqtt_server "driver.cloudmqtt.com" // the MQTT server address
#define mqtt_user "rrhwxjnq"               // the user login
#define mqtt_password "7WkEDME6iYqb"       // the password
#define mqtt_port 18667 // make sure to set the correct port number (defined by the MQTT service provider)


#define feed1 "Melissa/weather"    // the name of the feed to publish to
#define feed2 "Melissa/lightWave" // the name of the feed to publish to

#define wifi_ssid "Nick-Setup" // YOUR network name
#define wifi_password "C00lb0x!" // YOUR network password

// comment out the following two lines if you are using fona or ethernet
//#include "AdafruitIO_WiFi.h"
//AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
