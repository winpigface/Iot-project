#include<ESP8266WiFi.h>
#include<ESP8266WiFiMulti.h>
#include<ESP8266WebServer.h>
#include<ArduinoMqttClient.h>
#include<TickTwo.h>

const int sensor = D3;
const int buzzer = D7;
bool switchs = true;
ESP8266WiFiMulti wifi;
ESP8266WebServer webServer(80);
WiFiClient mqttWiFiClient;
MqttClient mqtt(mqttWiFiClient);

void mywebsite(String text);
void publishThingspeak();
TickTwo sendData(publishThingspeak,2000);

void mywebsite(String text){
  if (text == "ON"){
     webServer.send(200,"text/html",
      R"=====( 
        <!DOCTYPE html>
        <html>
          <head>
            <title> Iot project </title>
            <style>
              h1{
                color: red;
              }
              a{
                text-decoration: none;
              }
              .box1{
                text-align: center;
                background-color: #fdb987;
              }
              .OnText{
                color: green;
              }
              button{
                width: 100px;
              }
              .buttonOn:hover{
                background-color:green; 
                opacity: 0.4;
              }
              .buttonOff:hover{
                background-color:red;
                opacity: 0.4;
              }
            </style>
          </head>
          <body class='box1'>
            <div>
              <h1> Welcome </h1>
              <h2> Arif Patimin 6530613014 </h2>
              <h2> Natthapat Chuaycharoen 6530613017 </h2>
              <hr>
              <h2>Status : <span class='OnText'>ON</span></h2>
              <a href='/on'><button class='buttonOn'>ON</button></a> <a href='/off'><button class='buttonOff'>OFF</button></a>
              <h4>My code <a href='https://github.com/winpigface/Iot-project/blob/main/Iot-project.ino'>link</a><h4>
            </div>
          </body>
        </html>
      )====="   
     );
  }
  if (text == "OFF"){
     webServer.send(200,"text/html",
      R"=====(
        <!DOCTYPE html>
        <html>
          <head>
            <title> Iot project </title>
            <style>
              h1{
                color: red;
              }
              a{
                text-decoration: none;
              }
              .box1{
                text-align: center;
                background-color: #fdb987;
              }
              .OnText{
                color: red;
              }
              button{
                width: 100px;
              }
              .buttonOn:hover{
                background-color:green; 
                opacity: 0.4;
              }
              .buttonOff:hover{
                background-color:red;
                opacity: 0.4;
              }
            </style>
          </head>
          <body class='box1'>
            <div>
              <h1> Welcome </h1>
              <h2> Arif Patimin 6530613014 </h2>
              <h2> Natthapat Chuaycharoen 6530613017 </h2>
              <hr>
              <h2>Status : <span class='OnText'>OFF</span></h2>
              <a href='/on'><button class='buttonOn'>ON</button></a> <a href='/off'><button class='buttonOff'>OFF</button></a>
              <h4>My code <a href='https://github.com/winpigface/Iot-project/blob/main/Iot-project.ino'>link</a><h4>
            </div>
          </body>
        </html>
      )====="
     );  
  }
}
void publishThingspeak(){
  int magsensor = !digitalRead(sensor);
  String payload = "field1=" + String(magsensor);
  mqtt.beginMessage("channels/2531118/publish",false,0);
  mqtt.print(payload);
  mqtt.endMessage();
}

void setup() {
  Serial.begin(9600);
  pinMode(buzzer,OUTPUT);
  pinMode(sensor,INPUT);

  digitalWrite(sensor,HIGH);

  // Connect wifi
  wifi.addAP("iot-lab","computing");

  // check wifi is connect or not (waiting to connnect wifi)
  while (wifi.run() != WL_CONNECTED){
    //Serial.print(".");
    delay(500);
  }
  // Show wifi IP
  Serial.println("Connected");
  Serial.println(WiFi.localIP());

  // Website remote 
  webServer.on("/",[](){
      mywebsite("ON");
  });
  webServer.on("/on",[](){
      switchs = true;
      mywebsite("ON");
  });
  webServer.on("/off",[](){
      switchs = false;
      mywebsite("OFF");
  });
  // Mqtt
  mqtt.setId("OBgmDxoFBRkcFxgbDQgMKSE");
  mqtt.setUsernamePassword("OBgmDxoFBRkcFxgbDQgMKSE","N4EmT/u5v1w0av4+x9uMMcgB");
  if(!mqtt.connect("mqtt3.thingspeak.com",1883)){
    Serial.print("Unable to connect to broker");
    Serial.println(mqtt.connectError());
    while(true);
  }
  sendData.start();
  webServer.begin();

}

void loop() {
  sendData.update();
  mqtt.poll();
  webServer.handleClient();
  int magsensor = digitalRead(sensor);

  if(magsensor == LOW && switchs) // LOW = ON
  {
    Serial.println("Found ");
    digitalWrite(buzzer,LOW);  
  }
  else // HIGH == OFF
  {
    if(switchs){
      Serial.println("Not found ");
    }
    digitalWrite(buzzer,HIGH);
  }
  delay(500);
}
