/*
* Names: Alexandre Marques - Josias Goulart
* Date: 03/09/2018
* class operational implementation
*/

#include "operational.h"
#include "configuration.h"
#include "controller.h"

Controller ctrl;

#define DEBUG
#define SEND_TIME 20000

int outputpin = A0;

const char* mqttServer    = configs.wifiConfig.mqttServer; //"m12.cloudmqtt.com";
const char* mqttUser      = configs.wifiConfig.mqttUser; // "ikgkgtgi";
const char* mqttPassword  = configs.wifiConfig.mqttPassword; //"zXmgJfFXdbqp";
const int   mqttPort      = 17382; //17382
const char* mqttTopicSub  = configs.wifiConfig.mqttTopicSub; //"sensor/T1";

int finalSendMQTT = 0;

WiFiClient espClient;

PubSubClient client(espClient);

void Operational::initSystem()
{   
    Serial.begin(115200);

    Serial.println();
    Serial.println("Iniciando o sistema...");

    EEPROM.begin(512);
    
    configs.loadConfiguration();

    ctrl.initController();

    WiFi.disconnect();
    delay(200);
    WiFi.mode(WIFI_STA);
    WiFi.begin("", "");
    int connRes = WiFi.waitForConnectResult();
    WiFi.mode(WIFI_AP);
    
    WiFi.begin(configs.wifiConfig.ssid, configs.wifiConfig.password);

    while(WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        #ifdef DEBUG
        Serial.println("Conectado ao WiFi...");
        #endif
    }

    client.setServer(mqttServer, mqttPort);
    client.setCallback([this] (char* topic, byte* payload, unsigned int length) { this->callback(topic, payload, length); });

    while(!client.connected())
    {
        #ifdef DEBUG
        Serial.println("Conectado ao Broker MQTT...");
        #endif

        if(client.connect("ESP8266Client", mqttUser, mqttPassword))
        {
            #ifdef DEBUG
            Serial.println("Conectado");
            #endif
        }

        else
        {
            #ifdef DEBUG
            Serial.print("Falha estado: ");
            Serial.print(client.state());
            #endif

            delay(2000);
        }
    }

    client.subscribe(mqttTopicSub);
}

void Operational::process()
{
    if(!client.connected())
    {
        reconect();
    }

    if((millis() - finalSendMQTT) > SEND_TIME)
    {
        sendTemp();
        finalSendMQTT = millis();
    }

    client.loop();
}

void Operational::callback(char* topic, byte* payload, unsigned int length)
{
    payload[length] = '\0';
    String strMsg = String((char*) payload);

    float convert = strMsg.toFloat();

    ctrl.monitorController(convert);

    #ifdef DEBUG
    Serial.print("Mensagem chegou do tópico ");
    Serial.println(topic);
    Serial.print("Mensagem ");
    Serial.print(strMsg);
    Serial.println();
    Serial.println("------------------------------");
    #endif
}

void Operational::reconect()
{
    while(!client.connected())
    {
        #ifdef DEBUG
        Serial.print("Tentando conectar ao servidor MQTT");
        #endif

        bool conected = strlen(mqttUser) > 0 ? client.connect("ESP8266Client", mqttUser, mqttPassword) : client.connect("ESP8266Client");
    
        if(conected)
        {
            #ifdef DEBUG
            Serial.println("Conectado!");
            #endif

            client.subscribe(mqttTopicSub, 1);
        }

        else
        {
            #ifdef DEBUG
            Serial.println("Falha durante a conexão. Code: ");
            Serial.println(String(client.state()).c_str());
            Serial.println("Tentando se conectar novamente em 10s...");
            #endif;

            delay(10000);
        }
    }
}

void Operational::sendTemp()
{
    char MsgTemperaturaMQTT[10];
  
    float analogValue = (float(analogRead(outputpin)) * 5 / (1023)) / 0.01;

    if(isnan(analogValue))
    {
        #ifdef DEBUG
        Serial.println("Falha na leitura do sensor");
        #endif
    }

    else
    {
        #ifdef DEBUG
        Serial.print("Temperatura: ");
        Serial.print(analogValue);
        Serial.print(" ºC ");
        #endif

        sprintf(MsgTemperaturaMQTT,"%f",analogValue);
        client.publish("sensor/T1", MsgTemperaturaMQTT);
    }
}
