/*
  S4.cpp - Library for the overall S4 Program
  Kevin Zack
  Sonoma State University
  NASA E/PO
  Released into the public domain.
*/
  #include "S4.h"
  #include "Arduino.h"
  #include "WiFly.h"

  #include "../S4GPS/S4GPS.h"
  #include "../S4GPS/S4GPS.cpp"
 
  #include "Wire.h"
  
  #include "SoftwareSerial.h"

  
  
  #define microRX 5
  #define microTX 4
  #define buadRate 9600
  
  #define BUFFSIZE 90
  
  
  
  S4GPS S4GPS;  

  bool WiFiIsOn = true; // used to turn on or off the WiFi connection
  bool gpsIsOn = true;
  
  char gps[BUFFSIZE];
  String sensorData;
  
  SoftwareSerial microSerial = SoftwareSerial(microRX, microTX); 
  char* deviceName;


  
  S4::S4()
  {   
  }
  
  void S4::useGPS(bool setUseGPS)
  {
       gpsIsOn = setUseGPS;
  }
  
  void S4::begin(char* DeviceName, char* RouterName)
  {
       deviceName = DeviceName;
       WDTCSR |= (1 << WDCE) | (1 << WDE);
       WDTCSR = 0;
       microSerial.begin(buadRate);  // this had to be before the gps
       Wire.begin(); 
       
       
       S4GPS.begin(buadRate);
       
       if(WiFiIsOn)
       {
           WiFly.begin();                // start the WiFly process
           join(RouterName); // connect to the router
           WiFiHankshake(DeviceName);    // connect to the server
       }
      
      
       
       
       sensorData = "<sensor>";
       sensorData += deviceName;
       sensorData += ",";
       Serial.println("Starting");
  }
  
  bool S4::WiFiHankshake(char* DeviceName)
  {
       
       boolean start = false;
       long previousMillis = 0;
       long interval = 1000;
       char tempChar;
       
       while(!start)
       {
          SpiSerial.println("open");
          delay(100);
          while(SpiSerial.available() > 0) 
          {
              tempChar = SpiSerial.read();
              Serial.write(tempChar);
              if(tempChar == '#')
              {
                  SpiSerial.print("DeviceName");
                  SpiSerial.println(DeviceName);
              }
              if(tempChar == '@')
              {
                  delay (1000);
                  start = true;
              }
          }
          delay(900);
       }
          Serial.println("START");
                 
       
      return start;
  }
  
  int S4::getIncommingMessage()
  {
      char tempChar;
      int command = 0;
      while(SpiSerial.available() > 0) 
      {
        tempChar = SpiSerial.read();
        
        if(tempChar == '&')
        {
          tempChar = SpiSerial.read();
          command = tempChar;
        }
      }
      return command-48;  // to drop the askii down to the number
  } 
       
  int S4::writeData()
  {
      bool wait = true;
      int command = 0;
      while(wait)
      {
          if(gpsIsOn)
          {
              if(S4GPS.getGPS(gps))
              {   
                  sensorData += "</sensor>";
                 
                 
                  Serial.print("<gps>");
                  Serial.print(deviceName);
                  Serial.print(",");  
                  Serial.print(gps);
                  Serial.println("</gps>");
                  Serial.println(sensorData);
                  
                  
                  microSerial.print("<gps>");
                  microSerial.print(deviceName);
                  microSerial.print(",");
                  microSerial.print(gps);
                  microSerial.println("</gps>");
                  microSerial.println(sensorData);
                  
                  if(WiFiIsOn)
                  {
                      SpiSerial.print("<gps>");
                      SpiSerial.print(deviceName);
                      SpiSerial.print(",");  
                      SpiSerial.print(gps);
                      SpiSerial.println("</gps>");
                      SpiSerial.println(sensorData);
                      
                      command = getIncommingMessage(); 
                  }
                  sensorData = "<sensor>";
                  sensorData += deviceName;
                  wait = false;
              }
          }
          else
          {
              sensorData += "</sensor>";
              Serial.println(sensorData);
              microSerial.println(sensorData);
              if(WiFiIsOn)
              {
                  SpiSerial.println(sensorData);
                  command = getIncommingMessage();
              } 
              sensorData = "<sensor>";
              sensorData += deviceName;
              wait = false; 
          }
      }
      return command;
  } 
  
  int S4::getCommand()
  {
     int command; 
     command = getIncommingMessage();  
     return command; 
  }
 
 
  void S4::addData(char* key, int value)
  {
      sensorData += ",";
      sensorData += key;
      sensorData += ",";
      sensorData += value;
  }
   
  void S4::addData(char* key, char* value)
  {
      sensorData += ",";
      sensorData += key;
      sensorData += ",";
      sensorData += value;  
  }
  void S4::addData(char* key, long value)
  {
      sensorData += ",";
      sensorData += key;
      sensorData += ",";
      sensorData += value;   
  }
  void S4::addData(char* key, unsigned int value)
  {
      sensorData += ",";
      sensorData += key;
      sensorData += ",";
      sensorData += value; 
  }   
  void S4::addData(char* key, double value, int precision)
  {
      sensorData += ",";
      sensorData += key;
      sensorData += ",";
      char output[10];
      dtostrf(value,2,precision,output);
      sensorData += output; 
  } 
  
  void S4::useWiFi(bool statement)
  {
      WiFiIsOn = statement;
  }
 void S4::join(const char* ssid)
   {   
       WiFly.uart.begin();
       WiFly.reboot(); // Reboot to get device into known state
       
       WiFly.enterCommandMode();
       Serial.print("joining ");
       Serial.println(ssid);
   // sendCommand("join DeviceTesting \r",true);
       WiFly.sendCommand("join ",true,"");
       WiFly.sendCommand(ssid,true,"");
       WiFly.sendCommand("\r",true,"");
       Serial.println("assocated");
    
       WiFly.sendCommand("open \r",true,"");
       WiFly.uart.println();
    
    
      delay(100);                   
   }
