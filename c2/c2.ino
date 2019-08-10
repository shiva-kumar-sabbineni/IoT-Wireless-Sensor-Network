#include <SoftwareSerial.h>

#define setPin 5  
#define DHTPIN A0 
#include <DHT.h>
SoftwareSerial HC12(3, 2);   
String node,rd,cmd;
String  a;
int t=32;
DHT dht(DHTPIN, DHT11);
void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600); 
  dht.begin();// Open serial port to computer
  HC12.begin(9600);   
  pinMode(setPin, OUTPUT);
  digitalWrite(setPin, LOW);           // Set HC-12 into AT Command mode
   delay(100);                          // Wait for the HC-12 to enter AT Command mode
   HC12.println("AT+C033");              // Send AT Command to HC-12 ("AT+C001")
   delay(200);
  if (HC12.available()>0) {           // If HC-12 has data (the AT Command response)
     Serial.write(HC12.read());  
  }
     digitalWrite(setPin, HIGH);
 delay(100);// Serial port to HC12
}

void loop() {
 node="\0";
    t = dht.readTemperature();
    if (t==0)
    {
      t=32;
    }
     Serial.print("temperature::");Serial.println(t);
   if (HC12.available()>0 ) { 
      // If HC-12 has data (the AT Command response)
    rd = HC12.readStringUntil("$");
    rd.trim();
    //Serial.print("Rd");
   Serial.println(rd);
    //Serial.println(rd.length());
   
    if(rd.startsWith("@")&&rd.endsWith("$")){
      node = rd.substring(1,5);
      Serial.print("Node::");Serial.println(node);
      if(node=="resq"){
     cmd="@";
    cmd+="b";
    
    if(t<10)
    {
      cmd+="0";
    }
  cmd+=t;
  cmd+=",@";  
  Serial.println(cmd);
   HC12.println(cmd);
}
     }
   } 
  delay(500);   
      
}

