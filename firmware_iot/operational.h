/*
* Names: Alexandre Marques - Josias Goulart
* Date: 03/09/2018
* class operational
*/

#ifndef operational_h
#define operational_h

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

class Operational
{
    public:
        void callback(char* topic, byte* payload, unsigned int length);
        void reconect();
        void sendSensor();
        void initSystem();
        void process();
        void sendTemp();
};

#endif
