/*****

  All the resources for this project:
  https://randomnerdtutorials.com/

*****/

// Loading the ESP8266WiFi library and the PubSubClient library
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

String Lamp_Control(String messageTemp);
String Motor_Control(String messageTemp);
String LEDPWM_Control(String SDuty);

SoftwareSerial mySerial(13, 15); //
// Change the credentials below, so your ESP8266 connects to your router
const char* ssid =  "iptime2.4GHz-JHS";  // "illifamily2G"; //"iptime2.4GHz-JHS";
const char* password = "ace2022^!"; // "illikim575859"; // "ace2022^!";

// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server =    "192.168.43.202"; //"tcp://192.168.43.202"; //"114.205.87.39";
const int mqtt_server_port =   9099;  // 1001; //9099; //1883;

// Initializes the espClient
WiFiClient espClient;
PubSubClient client(espClient);
//int timeIn=0;
// Connect an LED to each GPIO of your ESP8266
const int ledGPIO5 = D1;
const int PWM1 = D6;
// Don't change the function below. This functions connects your ESP8266 to your router
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  Serial.println("Wifi begin called");
  delay(100);
  
  WiFi.begin(ssid, password);
  
  //catch()
  Serial.println("Wifi begin ended");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

// This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that
// your ESP8266 is subscribed you can actually do something
void callback(String topic, byte* message, unsigned int length) {


  //client.publish("esp8266/function/Lamp", "1");

  
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;


  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
  //시간을 받아 저장
  /*String inString="";
     for (int i = 0; i < length; i++) {
      inString += (char)message[i];
    }*/
  //timeIn=inString.toInt();
  //timeIn*=1000;


  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic home/office/esp1/gpio2,
  // you check if the message is either 1 or 0. Turns the ESP GPIO according to the message
  // 우리가 실제적으로 건드려야할 곳

  Serial.print(messageTemp);
  if (topic == "esp8266/function/Lamp") {
    Lamp_Control(messageTemp);
  }
  if (topic == "esp8266/function/Motor") {
    Motor_Control(messageTemp);
  }
  if (topic == "esp8266/function/LED") {
    LEDPWM_Control(messageTemp);
  }
  /*if(topic=="esp8266/read"){
     if(messageTemp == "0"){
      Serial.print("READ:");
      send_data();
    }
    }

    if(Serial.available()){
    //Serial.println(timeIn);

    for (int i = 0; i < length; i++) {
            a=Serial.write(message[i]);
      }

    }*/
  Serial.println();
  // callback 함수 안에 있음
  messageTemp=""; //MT 초기화
}



// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266
// 8266이 MQTT broker에 재 접속하게 된다.
// 8266에서 더 많은 토픽을 구독하고 싶을때 아래 기능을 바꾸면 된다.
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect

    if (client.connect("ESP8266Client2")) {
      Serial.println("connected");
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      // 더 많은 토픽을 구독 할 수 있다.
      // 토픽을 바꾸면 여기도 바꿔줘야
      //client.subscribe("esp8266/4");
      //client.subscribe("esp8266/5");
      client.subscribe("esp8266/function/Lamp");
      client.subscribe("esp8266/function/Motor");
      //Serial.print("띠용");
      client.subscribe("esp8266/function/LED");
      client.publish("esp8266/text", "Welcome Smart Home");
      client.subscribe("esp8266/read");
      //client.subscribe("esp8266/write");
      client.subscribe("esp8266/test");


    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// The setup function sets your ESP GPIOs to Outputs, starts the serial communication at a baud rate of 115200
// Sets your mqtt broker and sets the callback function
// The callback function is what receives messages and actually controls the LEDs
/*void send_data()
  {
  long lastmsg = 0;
  long now = millis();
  // millis()는 아두이노로 프로그램을 돌리기 시작한 후 지난 시간을 밀리 초 숫자로 return,
  // 50일 후에 오버플로우 되어 초기화
  char temp[50]; // 50글자를 받아오는 배열 한글은 한글자에 2바이트 20글자, 영어는 널문자 포함 40글자
  for (int a=0;a<50;a++ ){
    temp[a] = 0;
    } // 버퍼 초기화
  if(now - lastmsg > 1000 ) // 1초에 한번씩 확인 할 수 있음
  {
    lastmsg = now;

    byte leng = Serial.readBytes(temp, 50);

    Serial.print("Input data Lenght : ");
    Serial.println(leng);
    for(int i = 0; i < leng; i++)
      {
        Serial.print(temp[i]);
      }
    client.publish("esp8266/write",temp); // esp8266/write로 발행하고, temp데이터를 보냄
  }
  }*/
void setup() {
  pinMode(ledGPIO5, OUTPUT);
  pinMode(PWM1,OUTPUT);
  Serial.begin(9600);
  Serial.print("Powered on");
  setup_wifi();
  mySerial.begin(9600);
  client.setServer(mqtt_server, mqtt_server_port);
  client.setCallback(callback);

}

// For this project, you don't need to change anything in the loop function.
// Basically it ensures that you ESP is connected to your broker
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  if (!client.loop()) {

    client.connect("ESP8266Client2");
  }

  client.publish("esp8266/test"," pubtest!");
  Serial.print("(jjs send)\n");
  delay(2000);

}


String Lamp_Control(String messageTemp){
  Serial.println("LAMP On Off Control");
    if (messageTemp == "1") {
     /* for (int i = 0; i < length; i++) {
        mySerial.write(message[i]);
      }*/
      Serial.print("On");
      digitalWrite(ledGPIO5,HIGH); //led D1
      return "LED ON";
    }// 형광등 제어
    else if (messageTemp == "3") {
     /* for (int i = 0; i < length; i++) {
        mySerial.write(message[i]);
      }*/
      Serial.print("Off");
      digitalWrite(ledGPIO5,LOW); //led D1
      return "LED OFF";
    }
}

String Motor_Control(String messageTemp){
  
}

String LEDPWM_Control(String SDuty){
  int Duty = atoi(SDuty.c_str());
  Serial.print("\n Duty : ");
  Serial.println(Duty);
  analogWrite(PWM1,Duty*100);
  //analogWrite(PWM1,1023);
  
}
