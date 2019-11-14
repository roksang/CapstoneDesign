//0508 PONG

#include <SoftwareSerial.h>
#include "SNIPE.h"
#include <stdio.h>
#include <string.h>

#define TXpin 11
#define RXpin 10
#define ATSerial Serial

//16byte hex key
String lora_app_key = "11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff 00";  

SoftwareSerial DebugSerial(RXpin,TXpin);
SNIPE SNIPE(ATSerial);
int echo=12;
int trig=13;
int distance;
unsigned long duration;
String buff; //실수-문자열 변환
char buffer[20], temp[20];

void setup() {
  ATSerial.begin(115200);
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);

  // put your setup code here, to run once:
  while(ATSerial.read()>= 0) {}
  while(!ATSerial);

  DebugSerial.begin(115200);

  /* SNIPE LoRa Initialization */
  if (!SNIPE.lora_init()) {
    DebugSerial.println("SNIPE LoRa Initialization Fail!");
    while (1);
  }

  /* SNIPE LoRa Set Appkey */
  if (!SNIPE.lora_setAppKey(lora_app_key)) {
    DebugSerial.println("SNIPE LoRa app key value has not been changed");
  }
  
  /* SNIPE LoRa Set Frequency */
  if (!SNIPE.lora_setFreq(LORA_CH_1)) {
    DebugSerial.println("SNIPE LoRa Frequency value has not been changed");
  }

  /* SNIPE LoRa Set Spreading Factor */
  if (!SNIPE.lora_setSf(LORA_SF_7)) {
    DebugSerial.println("SNIPE LoRa Sf value has not been changed");
  }

  if (!SNIPE.lora_setRxtout(5000)) {
    DebugSerial.println("SNIPE LoRa Rx Timout value has not been changed");
  }  
    
  DebugSerial.println("SNIPE LoRa Pong Test");
}

void loop() {
              //초음파 센싱
              digitalWrite(trig, LOW);
              digitalWrite(echo, LOW);
              delayMicroseconds(2);
              digitalWrite(trig, HIGH);
              delayMicroseconds(10);
              digitalWrite(trig, LOW);
            
            // echoPin 이 HIGH를 유지한 시간을 저장 한다.
            duration = pulseIn(echo, HIGH); 
           // HIGH 였을 때 시간(초음파가 보냈다가 다시 들어온 시간)을 가지고 거리를 계산 한다.
            distance = ((float)(340 * duration) / 10000) / 2; 
            DebugSerial.print(distance);
            DebugSerial.println("cm");

           dtostrf(distance, 5, 0, temp);
           sprintf(buffer,"%s",temp);
           buff = String(buffer); 
           //buff = String(distance);
           buff.trim();
            DebugSerial.print("Before send=");
             DebugSerial.println(buff);
           if(SNIPE.lora_send(buff)){
             DebugSerial.println("Pongtest");    
             DebugSerial.print("After send=");
             DebugSerial.println(buff);
            DebugSerial.println("DISTANCE send success");
            }
            else
              {
                DebugSerial.println("distance send fail");
                delay(500);
              }
       
       delay(2000);

}
