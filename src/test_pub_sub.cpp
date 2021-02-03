/*
 Long message ESP8266 MQTT example

 This sketch demonstrates sending arbitrarily large messages in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "greenBottles/#", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the sub-topic is a number, it publishes a "greenBottles/lyrics" message
    with a payload consisting of the lyrics to "10 green bottles", replacing
    10 with the number given in the sub-topic.

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>

// Update these with values suitable for your network.

const char* ssid = "S20+";
const char* password = "12345678abc";
const char* mqtt_server = "192.168.46.198";

const char* mqtt_username = "mickael"; // MQTT username
const char* mqtt_password = "mickael"; // MQTT password

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  char dose[length];
  char monitoring[60];

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    dose[i] = (char)payload[i];
  }
  Serial.println();
  
  Serial.println();

  if(dose[0] == '1'){
    Serial.print("Preparation cafe \n");
    Serial.print("Dose de cafe : ");
    Serial.print(dose[1]);
    Serial.print("\nDose de sucre : ");
    Serial.print(dose[2]);
    Serial.print("\n");
  }

  else if(dose[0] == '2'){
    Serial.print("Preparation the \n");
    Serial.print("Dose de the : ");
    Serial.print(dose[1]);
    Serial.print("\nDose de sucre : ");
    Serial.print(dose[2]);
    Serial.print("\n");
  }

  else if(dose[0] == '3'){
    Serial.print("Preparation chocolat \n");
    Serial.print("Dose de chocolat : ");
    Serial.print(dose[1]);
    Serial.print("\nDose de sucre : ");
    Serial.print(dose[2]);
    Serial.print("\n");
  }

  else{
    Serial.print("preparation inconnu\n");
  }

  float rand_cafe_qtt = random(100);
  float rand_the_qtt = random(100);
  float rand_chocolat_qtt = random(100);
  float rand_sucre_qtt = random(100);

  sprintf(monitoring,"Reservoirs : cafe : %.0f%% ",rand_cafe_qtt);
  client.publish("Monitoring", monitoring);

  sprintf(monitoring,"Reservoirs : the : %.0f%% ",rand_the_qtt);
  client.publish("Monitoring", monitoring);

  sprintf(monitoring,"Reservoirs : chocolat : %.0f%% ",rand_chocolat_qtt);
  client.publish("Monitoring", monitoring);

  sprintf(monitoring,"Reservoirs : sucre : %.0f%% ",rand_sucre_qtt);
  client.publish("Monitoring", monitoring);
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(),mqtt_username, mqtt_password)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("Monitoring", "Connecte au serveur MQTT");
      // ... and resubscribe
      client.publish("Monitoring", "subscribe to home/kfee");
      client.subscribe("home/kfee");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  randomSeed(analogRead(0));
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}