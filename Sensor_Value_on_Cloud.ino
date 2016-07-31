#include <SoftwareSerial.h>
#include <stdlib.h>

// replace with your channel's thingspeak API key
String apiKey = "FW41E4VAW3RNZFXP";
SoftwareSerial ser(2, 3); // RX, TX
int i=1;

// this runs once
void setup() {                
  // initialize the sensor at 6th pin and A0 as an input.
  pinMode(6,INPUT);// Light Sensor
  pinMode(A0,INPUT);// POT
  
  // enable debug serial
  Serial.begin(9600); 
  // enable software serial
  ser.begin(9600);
  
  // reset ESP8266.
  ser.println("AT+RST");
}


// the loop 
void loop() {
  
  //reading the state of sensor1
  int lightsensor = digitalRead(6);         //Light Sensor
  int potentiometer = analogRead(A0);       //Potentiometer

  
  //Temperature Sensor Conversion
  /*float mv = ( sensor2State/1024.0)*5000; 
  float cel = mv/10;
  */
  String state1=String(lightsensor);
   
  String state2=String(potentiometer);
    
  // TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80";
  ser.println(cmd);
  Serial.println(cmd);
   
  if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }
  
  // prepare GET string
  String getStr = "GET /update?api_key=";  
  getStr += apiKey;
 
  getStr +="&field1=";
  getStr += String(state1);
  getStr +="&field2=";
  getStr += String(state2);
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ser.println(cmd);
  Serial.println(cmd);

  if(ser.find(">")){
    ser.print(getStr);
    Serial.print(getStr);
  }
  else{
    ser.println("AT+CIPCLOSE");
    // alert user
    Serial.println("AT+CIPCLOSE");
  }
    
  // thingspeak needs 15 sec delay between updates
  delay(15000);  
}
