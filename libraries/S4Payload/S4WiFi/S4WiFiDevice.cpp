#include "S4WiFi.h"
#include "../WiFly/WiFly.h"
#include "../WiFly/WiFlyDevice.cpp"

void WiFlyDevice::begin() {
  /*
   */

  DEBUG_LOG(1, "Entered WiFlyDevice::begin()");

  uart.begin();
  reboot(); // Reboot to get device into known state
  requireFlowControl();
  //setConfiguration();
}

void WiFlyDevice::getEverything()
{
     DEBUG_LOG(1, "Entered WiFlyDevice::getEverything()"); 
     enterCommandMode();
     sendCommand("get everything \r",false);  
}

void WiFlyDevice::JoinRouter(const char *ssid)
 {
    DEBUG_LOG(1, "Entered WiFlyDevice::AddRouter");   
    uart.begin();
    reboot(); // Reboot to get device into known state
  //  requireFlowControl();
    
    enterCommandMode();
    Serial.print("joining ");
    Serial.println(ssid);
   // sendCommand("join DeviceTesting \r",true);
    sendCommand("join ",true);
    sendCommand(ssid,true);
    sendCommand("\r",true);
    Serial.println("assocated");
    
    sendCommand("open \r",true);
    uart.println();
 }
void WiFlyDevice::AddAdhoc()
{
    DEBUG_LOG(1, "Entered WiFlyDevice::AddAdhoc()");   
    uart.begin();
    reboot(); // Reboot to get device into known state
  //  requireFlowControl();
    enterCommandMode();
    sendCommand("join S4 \r",true);
   
 }

void WiFlyDevice::setup(const char *deviceName, const char *accessPoint, boolean isWep, const char *key)
{
    DEBUG_LOG(1, "Entered WiFlyDevice::setup");   
    uart.begin();
    reboot();
    
    Serial.print("Setup");
    requireFlowControl();
    
    enterCommandMode();
    if(isWep)
    {
       sendCommand("set wlan auth 1 \r",true);
       
       sendCommand("set wlan key ",true);
       sendCommand(key,true);
       sendCommand("\r",true);
    }
    else
    {
       sendCommand("set wlan auth 0 \r",true);
    }
    sendCommand("set wlan ssid ",true);
    sendCommand(accessPoint,true);
    sendCommand("\r",true);
    
    sendCommand("set opt deviceid ",true);
    sendCommand(deviceName,true);
    sendCommand("\r",true);
    
    delay(3000);
    
    sendCommand("save \r",true);
     
}

SpiUartDevice SpiSerial;
WiFlyDevice WiFly(SpiSerial);
