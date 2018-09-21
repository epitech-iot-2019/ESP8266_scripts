// https://github.com/martin-ger/uMQTTBroker
#include <MQTT.h>
#include <ESP8266WiFi.h>

void myDataCb(String& topic, String& data);
void myPublishedCb();
void myDisconnectedCb();
void myConnectedCb();

#define CLIENT_ID "ESP8266_Oui"

MQTT myMqtt(CLIENT_ID, "broker.hivemq.com", 1883);

#define WIFI_SSID     "OnePlus 6 Oui"
#define WIFI_PASSWORD "lolilol68"

const int PIN_BUTTON = 0;

int previousButtonState;

void setup() {
  Serial.begin(9600);

  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Connecting to MQTT server");  

  // setup callbacks
  myMqtt.onConnected(myConnectedCb);
  myMqtt.onDisconnected(myDisconnectedCb);
  myMqtt.onPublished(myPublishedCb);
  myMqtt.onData(myDataCb);
  
  Serial.println("connect mqtt...");
  myMqtt.connect();

delay(10);

  pinMode(PIN_BUTTON, INPUT_PULLUP);
  previousButtonState = 0;
}

// the loop function runs over and over again forever
void loop() {
  int val = digitalRead(PIN_BUTTON) == 0;

  if (val != previousButtonState) {
    if (val == 1) {
      String topic("/");
      topic += CLIENT_ID;
      topic += "/value";

      String value(random(0,50));
      myMqtt.publish(topic, value);
      Serial.print("Publishing: ");
      Serial.println(value);
    }
  }
  previousButtonState = val;
  delay(50);
}

void myConnectedCb()
{
  Serial.println("connected to MQTT server");
}

void myDisconnectedCb()
{
  Serial.println("disconnected. try to reconnect...");
  delay(500);
  myMqtt.connect();
}

void myPublishedCb()
{
  Serial.println("Published.");
}

void myDataCb(String& topic, String& data)
{
  
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(data);
}
