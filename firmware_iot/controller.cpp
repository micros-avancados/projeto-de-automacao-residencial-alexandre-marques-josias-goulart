/*
* Names: Alexandre Marques - Josias Goulart
* Date: 03/09/2018
* class controller implementation
*/

#include "controller.h"
#include "Arduino.h"

#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRtimer.h>

int recvPin = 2;

IRsend irsend(recvPin);

const int pinoLedIR = 8; 

uint16_t irLiga[]    = {4400, 4350, 600, 1550, 550, 500, 600, 1550, 600, 1600, 550, 500, 600, 500, 550, 1600, 550, 500, 600, 500, 550, 1600, 600, 450, 600, 500, 600, 1550, 600, 1550, 600, 500, 550, 1600, 600, 450, 600, 500, 600, 1550, 600, 1550, 600, 1600, 550, 1550, 600, 1600, 550, 1550, 600, 1600, 550, 1600, 550, 500, 600, 500, 550, 550, 550, 500, 600, 500, 600, 450, 600, 500, 600, 1550, 600, 500, 600, 450, 600, 500, 600, 450, 600, 500, 600, 500, 550, 1600, 550, 500, 600, 1550, 600, 1600, 550, 1600, 550, 1600, 550, 1600, 550, 1600, 550}; // SAMSUNG B24D3FC0
uint16_t irDesliga[] = {4400, 4350, 600, 1550, 600, 500, 600, 1550, 550, 1600, 600, 450, 600, 500, 600, 1550, 600, 500, 600, 450, 600, 1600, 550, 500, 600, 500, 600, 1550, 550, 1600, 600, 450, 600, 1600, 550, 500, 600, 1550, 600, 1550, 600, 1550, 600, 1600, 550, 500, 600, 1550, 600, 1550, 600, 1550, 600, 500, 550, 550, 550, 500, 600, 500, 550, 1600, 550, 500, 600, 500, 600, 1550, 600, 1550, 600, 1550, 600, 500, 550, 500, 600, 500, 550, 550, 550, 500, 600, 500, 600, 450, 600, 500, 600, 1550, 600, 1550, 600, 1550, 600, 1550, 600, 1550, 600}; // SAMSUNG B24D7B84

const int weather = 24;
int contOn = 1;
unsigned int khz = 38;

void Controller::initController()
{
    Serial.begin(115200);
    pinMode(pinoLedIR, OUTPUT);
}

void Controller::monitorController(float temp)
{
    if(temp > weather && contOn == 1)
    {
        irsend.sendRaw(irLiga, sizeof(irLiga) / sizeof(irLiga), khz); //Note the approach used to automatically calculate the size of the array.
        irsend.sendRaw(irLiga, sizeof(irLiga) / sizeof(irLiga), khz); //Note the approach used to automatically calculate the size of the array.
        irsend.sendRaw(irLiga, sizeof(irLiga) / sizeof(irLiga), khz); //Note the approach used to automatically calculate the size of the array.

        contOn = 2;
        
        Serial.println("Ligando...");
    }

    if(temp < weather && contOn == 2)
    {
        contOn = 1;

        irsend.sendRaw(irDesliga, sizeof(irDesliga) / sizeof(irDesliga), khz); //Note the approach used to automatically calculate the size of the array.
        irsend.sendRaw(irDesliga, sizeof(irDesliga) / sizeof(irDesliga), khz); //Note the approach used to automatically calculate the size of the array.
        irsend.sendRaw(irDesliga, sizeof(irDesliga) / sizeof(irDesliga), khz); //Note the approach used to automatically calculate the size of the array.
        
        Serial.println("Desligando...");
    }
}
