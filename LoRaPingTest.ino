//0508 PING
//PING 코드 - wifi 연결ne
#include <SoftwareSerial.h>
#include "SNIPE.h"
#include <SPI.h>
#include <Client.h>
#include "ESP8266.h"

#define ATSerial Serial

#define SSID       "KT_WLAN_38A6"
#define PASSWORD    "000000fa68"
#define HOST_NAME   "172.30.1.47"
#define DEBUG true

SoftwareSerial mySerial(2,3); // RX:D2, TX:D3 wifi
ESP8266 wifi(mySerial);

bool isConnected=false;
//16byte hex key
String lora_app_key = "11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff 00";  


SNIPE SNIPE(ATSerial);
float distance;
String Buffer; //실수-문자열 변환

void setup() {
  ATSerial.begin(115200);
  //Serial.begin(115200);
  mySerial.begin(9600);
  Serial.print("setup begin\r\n");

   boolean connected=false;  
   for(int i=0;i<10;i++)  
   {  
       if(connectWiFi())  
       {  
         connected = true;  
         break;  
       }  
   }  
   mySerial.println("AT+CIPMUX=0");  
  
  // put your setup code here, to run once:
  while(ATSerial.read()>= 0) {}
  while(!ATSerial);


  /* SNIPE LoRa Initialization */
  if (!SNIPE.lora_init()) {
    Serial.println("SNIPE LoRa Initialization Fail!");
    while (1);
  }

  /* SNIPE LoRa Set Appkey */
  if (!SNIPE.lora_setAppKey(lora_app_key)) {
    Serial.println("SNIPE LoRa app key value has not been changed");
  }
  
  /* SNIPE LoRa Set Frequency */
  if (!SNIPE.lora_setFreq(LORA_CH_1)) {
    Serial.println("SNIPE LoRa Frequency value has not been changed");
  }

  /* SNIPE LoRa Set Spreading Factor */
  if (!SNIPE.lora_setSf(LORA_SF_7)) {
    Serial.println("SNIPE LoRa Sf value has not been changed");
  }


  if (!SNIPE.lora_setRxtout(5000)) {
    Serial.println("SNIPE LoRa Rx Timout value has not been changed");
  }  
    
  Serial.println("SNIPE LoRa Ping Test");
 
}

void loop() {
          Buffer=SNIPE.lora_recv();
           while(Buffer=="AT_ERROR")Buffer=SNIPE.lora_recv();
           Serial.println("Pingtest");                
           Serial.print("distance : ");
           Serial.println(Buffer);            


            //서버
            String cmd = "AT+CIPSTART=\"TCP\",\"";  
               cmd += HOST_NAME;  
               cmd += "\",80";  
               Serial.println(cmd);  
               mySerial.println(cmd);  
              if(mySerial.find("Error"))  
              {  
                Serial.println( "TCP connect error" );  
                return;  
              } 
              
             String web = "GET /php/newfile.php?Buffer="+Buffer+"\r\n";// HTTP/1.1\r\n";   

             mySerial.print("AT+CIPSEND=");  
             mySerial.println(web.length());  
              
              Serial.println(web);  
             
              if(mySerial.find(">"))  
               {  
                 Serial.print(">");  
                 }else  
                 {  
                   mySerial.println("AT+CIPCLOSE");  
                   Serial.println("connect");  
                   delay(1000);  
                   return;  
                 }  
              mySerial.println(web); 
              delay(2000); 
              while (Serial.available())  
                 {  
                   char c = Serial.read();  
                   mySerial.write(c);  
                   if(c=='\r') mySerial.print('\n');  
                 } 
             
               delay(60000); 
}


boolean connectWiFi()  
{  
   //mySerial.println("AT+CWMODE=1");  
     
   String cmd="AT+CWJAP=\"";  
   cmd+=SSID;  
   cmd+="\",\"";  
   cmd+=PASSWORD;  
   cmd+="\"";  
   mySerial.println(cmd);  
   Serial.println(cmd);  
   delay(3000);  
    
   if(mySerial.find("OK"))  
   {  
     Serial.println("OK, Connected to WiFi.");  
     return true;  
   }  
   else  
   {  
     Serial.println("Can not connect to the WiFi.");  
     return false;  
   }  
 }  
