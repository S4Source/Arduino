/*
  S4Sensor.cpp - Library for the Sensors Supported by S4
  Kevin Zack
  Sonoma State University
  NASA E/PO
  Released into the public domain.
*/
  #include "S4Sensor.h"
  #include "Arduino.h"
  #include "WiFly.h"
  #include "S4GPS.h"
  #include "Wire.h"
  #include "SoftwareSerial.h"
  #include "BMP085.h"
  #include "ADXL345.h"
  
  
  BMP085 presSen = BMP085(); // Barometric Sensor Stuff
 // long Temperature = 0;
 // long Pressure = 0;
  
  #define addressHum 0x27// Humidity Stuff
  byte _status; 
  unsigned int H_dat, T_dat;
  float RH, T_C;

  #define addressMag  0x1E // Magnatometer Stuff
  
  ADXL345 adxl;
  
  S4Sensor::S4Sensor()
  {   
      
  }
  void S4Sensor::startBarometric()
  {
       presSen.init();
  }
       
  void S4Sensor::getBarometric(long & pressure, double &temperature)
  {
       long temp;
       presSen.getPressure(&pressure);
       presSen.getTemperature(&temp);
       temperature = (double)temp/10.0;
       
  }
  void S4Sensor::startHumidity()
  {
  }
  
  byte S4Sensor::getHumidity(double &humidity, double &temperature)
  {
      byte address, Hum_H, Hum_L, Temp_H, Temp_L, _status;
      unsigned int H_dat, T_dat;
      
      Wire.beginTransmission(addressHum); 
      Wire.endTransmission();
      
      Wire.requestFrom((int)addressHum, (int) 4);
      Hum_H = Wire.read();
      Hum_L = Wire.read();
      Temp_H = Wire.read();
      Temp_L = Wire.read();
      Wire.endTransmission();
      
      _status = (Hum_H >> 6) & 0x03;
      Hum_H = Hum_H & 0x3f;
      H_dat = (((unsigned int)Hum_H) << 8) | Hum_L;
      T_dat = (((unsigned int)Temp_H) << 8) | Temp_L;
      
      
      T_dat = T_dat / 4;
      humidity = (double) H_dat * 6.10e-3;
      temperature = (double)T_dat *1.007e-2-40.0;
      return(_status);
  }

  void S4Sensor::startAccelerometer()
  {

      
      /******setup accel **********/
      adxl.powerOn();
      adxl.setActivityThreshold(1); //62.5mg per increment
      adxl.setInactivityThreshold(10); //62.5mg per increment
      adxl.setTimeInactivity(255); // how many seconds of no activity is inactive?
 
  //look of activity movement on this axes - 1 == on; 0 == off 
      adxl.setActivityX(1);
      adxl.setActivityY(1);
      adxl.setActivityZ(1);
 
  //look of inactivity movement on this axes - 1 == on; 0 == off
      adxl.setInactivityX(1);
      adxl.setInactivityY(1);
      adxl.setInactivityZ(1);
  
  }
  void S4Sensor::getAccelerometer(double &accelX, double &accelY, double &accelZ)
  {
       int intAccelX, intAccelY, intAccelZ;
       adxl.readAccel(&intAccelX, &intAccelY, &intAccelZ);
       accelX = (double)intAccelX/255.0;
       accelY = (double)intAccelY/255.0;
       accelZ = (double)intAccelZ/255.0;
       
  }
  void S4Sensor::startMagnetometer()
  {    
      Wire.beginTransmission(addressMag); //open communication with HMC5883
      Wire.write(0x02); //select mode register
      Wire.write(0x00); //continuous measurement mode
      Wire.endTransmission();
  }
  void S4Sensor::getMagnetometer(int &x, int &y, int &z)
  {
      Wire.beginTransmission(addressMag);
      Wire.write(0x03); //select register 3, X MSB register
      Wire.endTransmission();
      
      Wire.requestFrom(addressMag, 6);
      if(6<=Wire.available())
      {
          x = Wire.read()<<8; //X msb
          x |= Wire.read(); //X lsb
          z = Wire.read()<<8; //Z msb
          z |= Wire.read(); //Z lsb
          y = Wire.read()<<8; //Y msb
          y |= Wire.read(); //Y lsb
      }
  }
