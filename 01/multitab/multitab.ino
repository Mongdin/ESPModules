
//#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid =  "potato2G";  // "illifamily2G"; //"iptime2.4GHz-JHS";
const char* password = "Rkawkxnlrla1"; // "illikim575859"; // "ace2022^!";

const char* mqtt_server =    "192.168.0.3"; //"tcp://192.168.43.202"; //"114.205.87.39";
const int mqtt_server_port =   1883;  // 1001; //1883;

WiFiClient espClient;
PubSubClient client(espClient);

const int T1 = 0;
const int T2 = 2;

char stat = 0;

void setup_wifi() {
  delay(10);
  Serial.println();
  WiFi.disconnect(true);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  Serial.println("Wifi begin called");
  delay(100);
  WiFi.begin(ssid, password);

  Serial.println("Wifi begin ended");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(String topic, byte* message, unsigned int length) {
  
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if (topic == "8266/Multitab/Order") {
    Tab_Control(messageTemp);
  }
  Serial.println();
  messageTemp = ""; //MT 초기화
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect

    if (client.connect("ESP8266Client2")) {
      Serial.println("connected");
      client.subscribe("8266/Multitab/Order");
      client.subscribe("8266/Mon");
      client.publish("8266/Multitab/Response", "Multitab ready");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}


void setup() {
  pinMode(T1, OUTPUT);
  pinMode(T2, OUTPUT);
  digitalWrite(T1,HIGH);
  digitalWrite(T2,HIGH);
  Serial.begin(9600);
  Serial.print("Powered on");
  setup_wifi();
  //mySerial.begin(9600);
  client.setServer(mqtt_server, mqtt_server_port);
  client.setCallback(callback);

}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  if (!client.loop()) {

    client.connect("ESP8266Client2");
  }
}

void Tab_Control(String messageTemp) {
  Serial.println("T1, T2 On Off Control");
  if (messageTemp == "T1on") {
    stat |= 0b01;
    client.publish("8266/Multitab/Response", "T1 On");
    digitalWrite(T1,LOW);
    }
  else if (messageTemp == "T1off") {
    stat &= 0b10;
    client.publish("8266/Multitab/Response", "T1 Off");
    digitalWrite(T1,HIGH);
    }
  else if (messageTemp == "T2on") {
    stat |= 0b10;
    client.publish("8266/Multitab/Response", "T2 On");
    digitalWrite(T2,LOW);
    }
  else if (messageTemp == "T2off") {
    stat &= 0b01;
    client.publish("8266/Multitab/Response", "T2 Off");
    digitalWrite(T2,HIGH);
    }
  
}

/*
void Monitor(String messageTemp)
{
  if(messageTemp == "Mon")
  {
    String T1stat;
    String T2stat;
    T1stat = (stat&0b10) ? "T1 ON" : "T1 OFF" ;
    T2stat = (stat&0b01) ? "T2 ON" : "T2 OFF" ;
    
    client.publish("8266/Multitab/Mon/T1",T1stat);
    client.publish("8266/Multitab/Mon/T2",(const char*)stat);
  }
}
*/
