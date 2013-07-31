/*
  S4.h - Library for the overall S4 Program
  Kevin Zack
  Sonoma State University
  NASA E/PO
  Released into the public domain.
*/
  #ifndef S4_h
  #define S4_h
  #include "Arduino.h"
  #include "WiFly.h"
  #include "S4GPS.h"
  #include "Wire.h"
  #include "SoftwareSerial.h"
  
  class S4
  {
        public:
               S4();
               void begin(char* DeviceName, char* RouterName);
               bool WiFiHankshake(char* DeviceName);
               bool writeData();
               int getCommand();
               int getIncommingMessage();
               void useWiFi(bool statement);
               void addData(char* key, int value);
               void addData(char* key, double value,int precision);
               void addData(char* key, char* value);
               void addData(char* key, long value);
               void addData(char* key, unsigned int value);
               void join(const char* ssid);
        private:
                
  };
  #endif
