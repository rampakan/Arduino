#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
#define BLYNK_PRINT Serial

#include <PubSubClient.h>

//Config ขา Output
#define BT1 D1 //คือต่อรีเลย์ขา D1 กำหนดเป็นชื่อ BT1
#define BT2 D2 //คือต่อรีเลย์ขา D2 กำหนดเป็นชื่อ BT2 ***
#define BT3 D3 //คือต่อรีเลย์ขา D3 กำหนดเป็นชื่อ BT3
#define BT4 D4 //คือต่อรีเลย์ขา D4 กำหนดเป็นชื่อ BT4

#define WIFI_STA_NAME "Bunyarit"
#define WIFI_STA_PASS "BP161299"

#define MQTT_SERVER   "broker.emqx.io"
#define MQTT_PORT     1883
#define MQTT_USERNAME ""
#define MQTT_PASSWORD ""
#define MQTT_NAME     "ESP32_MQTT"

#define LED_PIN 23


WiFiClient client;
PubSubClient mqtt(client);

void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0';
  String topic_str = topic, payload_str = (char*)payload;
  Serial.println("[" + topic_str + "]: " + payload_str);

  if (payload_str == "1") {
    digitalWrite(BT1, HIGH);
    digitalWrite(BT2, HIGH);
    digitalWrite(BT3, HIGH);
    digitalWrite(BT4, HIGH);
    mqtt.publish("Janeen-feedback", "1");
  } else {
    digitalWrite(BT1, LOW);
    digitalWrite(BT2, LOW);
    digitalWrite(BT3, LOW);
    digitalWrite(BT4, LOW);
    mqtt.publish("Janeen-feedback", "0");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BT1, OUTPUT);
  pinMode(BT2, OUTPUT);
  pinMode(BT3, OUTPUT);
  pinMode(BT4, OUTPUT);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_STA_NAME);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_STA_NAME, WIFI_STA_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }

  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt.setCallback(callback);
}

bool flag = true;

void loop() {
  if (mqtt.connected() == false) {
    Serial.print("MQTT connection... ");
    if (mqtt.connect(MQTT_NAME, MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.println("connected");
      mqtt.subscribe("Janeen-light");
      if(flag){
        mqtt.publish("Janeen-feedback", "0");
        flag = false;
        }
    } else {
      Serial.println("failed");
      delay(5000);
    }
  } else {
    mqtt.loop();
  }
}
