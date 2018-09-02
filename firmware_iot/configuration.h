/*
* Names: Alexandre Marques - Josias Goulart
* Date: 03/09/2018
* class configuration
*/

#ifndef configuration_h
#define configuration_h

#include "Arduino.h"
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define VERSION_START  500
#define CONFIG_START   6
#define CONFIG_VERSION_MAJOR ((char)'1')
#define CONFIG_VERSION_MINOR ((char)'a')

class Configuration
{
  public:
      struct ConfigStruct
      {
          char ssid[50];
          char password[50];
          char mqttServer[50];
          char mqttUser[50];
          char mqttPassword[50];
          char mqttPort[50];
          char mqttTopicSub[50];
          char timeTemp[50];
      }wifiConfig;

      void handleRoot();
      void configWiFi();
      void configSubmit();
      void setupSystem();
      void loopSystem();
      void saveConfiguration();
      void loadConfiguration();
};

extern Configuration configs;

#endif
