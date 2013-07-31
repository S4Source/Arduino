#ifndef  __S4WIFI_H__
#define __S4WIFI_H__

//#include "../WiFly/WiFlyDevice.h"

class WiFlyDevice
{
  public:
         void setup(const char *deviceName, const char *accessPoint, boolean isWep, const char *key);
         void begin();
         void AddAdhoc();
         void JoinRouter(const char *ssid);
         void beginAdhoc();
         void getEverything();
         
          
          
};
#endif
