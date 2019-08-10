#include <Ticker.h>
#include <SoftwareSerial.h>
#include <Wire.h> 
#include <ESP8266WiFi.h>;
#include <WiFiClient.h>;
#include <ThingSpeak.h>;
 
const char* ssid = "no internet acess"; //Your Network SSID
 
const char* password = "nothanks"; //Your Network Password
 
int val;
#define setPin 5
Ticker ticker;
SoftwareSerial HC12(12, 2);   // HC-12 TX Pin, HC-12 RX Pin
String node,rd,cmd1,cmd2,c,d;
float t1,t2;
volatile int q,l;
WiFiClient client;
 
unsigned long myChannelNumber = 694090; //Your Channel Number (Without Brackets)
 
const char * myWriteAPIKey = "JY5OXWK1T1SFPLOL"; //Your Write API Key
void setup() {
  ticker.attach(1,isrFunc);
  Serial.begin(74880);                   // Open serial port to computer
  HC12.begin(9600);                     // Open serial port to HC12
  pinMode(setPin, OUTPUT); 
 WiFi.begin(ssid, password);
ThingSpeak.begin(client);
}
void loop() {   
  val = (t1+ t2)/2 ;

 
  if(q==6){
 channel1();
 delay(200);
  Serial.print("average temperature = ");
  Serial.println(val);
  }
  if(q==12)
  {
channel2();
delay(200);
 Serial.print("average temperature = ");
  Serial.println(val);
  
  }
  if (q==18)
  {
    ThingSpeak.writeField(myChannelNumber, 1,val, myWriteAPIKey); //Update in ThingSpeak 
    Serial.println("uploaded to thingspeak");
     q=0;
  }
 while (HC12.available()>0) {     // If HC-12 has data
 
   // Serial.write(HC12.read());
    delay(500);
    rd = HC12.readStringUntil(',@');
    delay(500);
   // Serial.write("rd==");
    rd.trim();
    Serial.println(rd);
     node=rd.substring(0,1);
       //Serial.print("Node::");
     // Serial.println(node);
    
     if (node =="a" )
     {
      c = rd.substring(1, 3);
    // Send the data to Serial monitor
      delay(500);
    t1 = c.toFloat();
    delay(500);
    Serial.print("temperature1 ="); 
    Serial.println(t1);
     }
      if (node =="b" )
     { 
      d = rd.substring(1, 3);
       delay(500);
    // Send the data to Serial monitor
    t2 = d.toFloat();
     delay(500);
        Serial.print("temperature2 ="); Serial.println(t2);
        
  }
  }
}
//CHANNEL-1
void channel1() {
 
  digitalWrite(setPin, LOW);           // Set HC-12 into AT Command mode
  delay(100);                          // Wait for the HC-12 to enter AT Command mode
  HC12.print("AT+C023");               // Send AT Command to HC-12
  delay(200);
  Serial.println("");
  Serial.println("Channel 1 successfully changed");
  
  digitalWrite(setPin, HIGH);    // Serial port to HC12
  cmd1 = "@reqs$";
  delay(200);
  HC12.print(cmd1);
 Serial.println(cmd1);
 delay(500);
  
}
//CHANNEL-2
void channel2() {
  digitalWrite(setPin, LOW);           // Set HC-12 into AT Command mode
    delay(100);                          // Wait for the HC-12 to enter AT Command mode
    HC12.print("AT+C033");               // Send AT Command to HC-12
    delay(200);
Serial.println("");
    Serial.println("Channel 2 successfully changed");
    digitalWrite(setPin, HIGH);
    delay(100);
  cmd2 = "@resq$";
  delay(200);
  HC12.print(cmd2);
 Serial.println(cmd2);
delay(500);
}

void isrFunc(){
  q=q+1;
  
}

