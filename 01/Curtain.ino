
//#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid =  "potato2G";  // "illifamily2G"; //"iptime2.4GHz-JHS";
const char* password = "Rkawkxnlrla1"; // "illikim575859"; // "ace2022^!";

const char* mqtt_server =    "192.168.0.3"; //"tcp://192.168.43.202"; //"114.205.87.39";
const int mqtt_server_port =   1883;  // 1001; //1883;

WiFiClient espClient;
PubSubClient client(espClient);

const int M0 = 0;
const int M1 = 2;
const int SW0 = 1;
const int SW1 = 3;

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

  if (topic == "8266/Curtain/Order") {
    Curtain_Control(messageTemp);
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
      client.subscribe("8266/Curtain/Order");
      client.subscribe("8266/Mon");
      client.publish("8266/Curtain/Response", "Curtain ready");
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
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(SW0, INPUT);
  pinMode(SW1, INPUT);
  
  digitalWrite(M0,HIGH);
  digitalWrite(M1,HIGH);
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

  statread();
  
}

void Curtain_Control(String messageTemp) {
  Serial.println("Curtain open close Control");
  if (messageTemp == "Copen") {
    stat |= 0b01;
    client.publish("8266/Curtain/Response", "Curtain Opening");
    }
  else if (messageTemp == "Cclose") {
    stat |= 0b10;
    client.publish("8266/Curtain/Response", "Curtain Closing");
  }
  else if (messageTemp == "Cstop") {
    stat = 0;
    client.publish("8266/Curtain/Response", "Curtain Stopped");
  }
  
}


void Switch(void) {
  char s0 = digitalRead(SW0);
  char s1 = digitalRead(SW1);
  if(s0 == HIGH) {
    stat = 0;
  }
  if(s1 == HIGH){
    stat = 0;
  }
}


void statread(void) {
  if(stat==0) {
    analogWrite(M0,0);
    analogWrite(M1,0);
  }
  
  else if(stat==0b01) {
    analogWrite(M0,50);
    analogWrite(M1,0);
  }

  else if(stat == 0b10) {
    analogWrite(M0,0);
    analogWrite(M1,50);
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
