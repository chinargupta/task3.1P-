#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include <DHT.h>
#include <DHT_U.h>

// WiFi and ThingSpeak settings
#define PRIVATE_SSID "chinar"
#define PRIVATE_PASS "gupta123"
#define PRIVATE_CH_ID  123456 // Replace with your ThingSpeak channel ID
#define PRIVATE_WRITE_APIKEY "YourAPIKey"

// DHT sensor settings
#define SENSORDHT 2
#define SENSORDHTTYPE DHT22
DHT_Unified dht(SENSORDHT, SENSORDHTTYPE);

// ThingSpeak settings
char ssid[] = PRIVATE_SSID;
char pass[] = PRIVATE_PASS;
int keyIndex = 0;
WiFiClient client;

unsigned long myChannelNumber = 2250584;
const char *myWriteAPIKey =24HT9D5G1EPAP0UG ;

// Initialize sensor values
float temperature = 0.0;
float humidity = 0.0;
int index1 = 0;
int index2 = random(0, 100);
String Status = "";

void setup() {
  Serial.begin(115200);

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  // Connect to WiFi
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(5000);
  }
  Serial.println("\nConnected to WiFi");

  dht.begin();
  ThingSpeak.begin(client);
  delay(250);
}

void readSensorData() {
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (!isnan(event.temperature)) {
    temperature = event.temperature;
  }

  dht.humidity().getEvent(&event);
  if (!isnan(event.relative_humidity)) {
    humidity = event.relative_humidity;
  }
}
void loop() {
  readSensorData();
  updateThingSpeak();

  index1++;
  if (index1 > 99) {
    index1 = 0;
  }
  index2 = random(0, 100);

  delay(20000); // Wait 20 seconds before updating the channel again
}
void updateThingSpeak() {
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(3, index1); 
  ThingSpeak.setField(4, index2);

  if (index1 > index2) {
    Status = "field3 is greater than field4";
  } else if ( index1 < index2) {
    Status = "field3 is less than field4";
  } else {
    Status = "field3 equals field4";
  }

  ThingSpeak.setStatus(Status);

  int chinar = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (chinar == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(chinar));
  }
}
