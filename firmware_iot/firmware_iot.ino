/*
* Names: Alexandre Marques - Josias Goulart
* Date: 03/09/2018
* class main
*/

#include "configuration.h"
#include "operational.h"

Configuration configuration;
Operational operational;

const int DD2 = D2;

int value = 0;

void setup()
{ 
  pinMode(DD2, INPUT);

  value = digitalRead(DD2);

  if(value > 0)
  {
      configuration.setupSystem();
  }

  else
  {
      operational.initSystem();
  }
}

void loop() 
{
  if(value > 0)
  {
      configuration.loopSystem();
  }

  else
  {
      operational.process();
  }
}
