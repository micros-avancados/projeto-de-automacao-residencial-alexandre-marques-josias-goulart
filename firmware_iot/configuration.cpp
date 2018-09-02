/*
* Names: Alexandre Marques - Josias Goulart
* Date: 03/09/2018
* class configuration implementation
*/

#include "configuration.h"

ESP8266WebServer server(80);

Configuration configs;

void Configuration::saveConfiguration()
{
    for (unsigned int t = 0; t < sizeof(wifiConfig); t++)
    {
        EEPROM.write(CONFIG_START + t, *((char*)&wifiConfig + t));
    }

    EEPROM.write(VERSION_START + 0, CONFIG_VERSION_MAJOR);
    EEPROM.write(VERSION_START + 1, CONFIG_VERSION_MINOR);
    EEPROM.commit();
}

void Configuration::loadConfiguration()
{
    if((char)EEPROM.read(VERSION_START + 0) == CONFIG_VERSION_MAJOR && (char)EEPROM.read(VERSION_START + 1) == CONFIG_VERSION_MINOR)
    {
        for (unsigned int t = 0; t < sizeof(wifiConfig); t++) *((char*)&wifiConfig + t) = (char)EEPROM.read(CONFIG_START + t);
    }

    else
    {
        String sid = "Rede Default";
        String psw = "admin@123";

        sid.toCharArray(wifiConfig.ssid, 50);
        psw.toCharArray(wifiConfig.password, 50);

        saveConfiguration();
    }
}

void Configuration::handleRoot()
{
    String html  = "<html><head><meta charset=\"UTF-8\"><title>Configuração Módulo IoT</title>";
           html += "</head><body>";
           html += "<div style=\"background-color: orange; color: white; font-size: 14pt; font-family: Arial, Helvetica, sans-serif; text-align: center;\"><p>Iniciar Configuração de Hardware</p>";
           html += "<p><a style=\"color: white; font-size: 14pt; font-family: Arial, Helvetica, sans-serif;\" href=/config>Configurar Servidor</a></p></div>";
           html += "</body></html>";

     server.send(200, "text/html", html);
}

void Configuration::configWiFi()
{
    String html  = "<html><head><meta charset=\"UTF-8\"><title>Configurar Servidor e WiFi</title>";
           html += "</head><body margin: 0px; style=\"overflow: hidden;\">";
           html += "<div style=\"background-color: orange; color: white; font-size: 14pt; font-family: Arial, Helvetica, sans-serif; text-align: center;\"><p>Configurar Servidor e WiFi</p></div>";
           html += "<div style=\"width: 100vw; height: 60vh; display: flex; flex-direction: row; justify-content: center; align-items: center;\"><div style=\"width: 500px; height: 300px;\"><form method=POST>";
           html += "<p style=\"font-weight: bold; font-family: Arial, Helvetica, sans-serif; font-size: 10pt;\">SSID: <input style=\"margin-left: 195px;\" name=tfssid type=text required value=\"";
           html += wifiConfig.ssid;
           html += "\" /></p>";
           html += "<p style=\"font-weight: bold; font-family: Arial, Helvetica, sans-serif; font-size: 10pt;\">Senha: <input style=\"margin-left: 186px;\" name=tfPswd type=password required value=\"";
           html += wifiConfig.password;
           html += "\" /></p>";
           html += "<p style=\"font-weight: bold; font-family: Arial, Helvetica, sans-serif; font-size: 10pt;\">MQTT-Server: <input style=\"margin-left: 143px;\" name=mqttServer type=text required value=\"";
           html += wifiConfig.mqttServer;
           html += "\" /></p>";
           html += "<p style=\"font-weight: bold; font-family: Arial, Helvetica, sans-serif; font-size: 10pt;\">MQTT-Usuário: <input style=\"margin-left: 135px;\" name=mqttUser type=text required value=\"";
           html += wifiConfig.mqttUser;
           html += "\" /></p>";
           html += "<p style=\"font-weight: bold; font-family: Arial, Helvetica, sans-serif; font-size: 10pt;\">MQTT-Senha: <input style=\"margin-left: 144px;\" name=mqttPassword type=password required value=\"";
           html += wifiConfig.mqttPassword;
           html += "\" /></p>";
           html += "<p style=\"font-weight: bold; font-family: Arial, Helvetica, sans-serif; font-size: 10pt;\">MQTT-Porta: <input style=\"margin-left: 150px;\" name=mqttPort type=text required value=\"";
           html += wifiConfig.mqttPort;
           html += "\" /></p>";
           html += "<p style=\"font-weight: bold; font-family: Arial, Helvetica, sans-serif; font-size: 10pt;\">MQTT-Topic: <input style=\"margin-left: 150px;\" name=mqttTopicSub type=text required value=\"";
           html += wifiConfig.mqttTopicSub;
           html += "\" /></p>";
           html += "<p style=\"font-weight: bold; font-family: Arial, Helvetica, sans-serif; font-size: 10pt;\">Timer-Temperatura: <input style=\"margin-left: 106px;\" name=timer type=text required value=\"";
           html += wifiConfig.timeTemp;
           html += "\" /></p>";
           
           html += "<p><input style=\"width: 110px; margin-top: 5%;\" name=btnSave type=submit value=Salvar /></p></form></div></div>";
           html += "</body></html>";

           server.send(200, "text/html", html);
}

void Configuration::configSubmit()
{
    String html  = "<html><head><title>Configurar WiFi Submit</title>";
           html += "</head><body>";
           html += "<div style=\"background-color: orange; color: white; font-size: 14pt; font-family: Arial, Helvetica, sans-serif; text-align: center;\"><p>Dados Gravados com Sucesso!</p></div>";

     String ssid          = server.arg("tfssid");
     String password      = server.arg("tfPswd");
     String mqttServer    = server.arg("mqttServer");
     String mqttUser      = server.arg("mqttUser");
     String mqttPswd      = server.arg("mqttPassword");
     String mqttPort      = server.arg("mqttPort");
     String mqttTopicSub  = server.arg("mqttTopicSub");
     String timeTemp      = server.arg("timer");

     ssid.toCharArray(wifiConfig.ssid, 50);
     password.toCharArray(wifiConfig.password, 50);
     mqttServer.toCharArray(wifiConfig.mqttServer, 50);
     mqttUser.toCharArray(wifiConfig.mqttUser, 50);
     mqttPswd.toCharArray(wifiConfig.mqttPassword, 50);
     mqttPort.toCharArray(wifiConfig.mqttPort, 50);
     mqttTopicSub.toCharArray(wifiConfig.mqttTopicSub, 50);
     timeTemp.toCharArray(wifiConfig.timeTemp, 50);

     html += "<form method=GET>";
     html += "<p><input name=btnSubmit type=submit value=Reiniciar /></p></form>";
     html += "</body></html>";

     server.send(200, "text/html", html);

     saveConfiguration();
}

void Configuration::setupSystem()
{
    Serial.begin(115200);
    Serial.println();
    Serial.println("Configurando Access Point...");

    EEPROM.begin(512);

    loadConfiguration();

    Serial.println("Rede de Configuração:");
    Serial.println(wifiConfig.ssid);
    Serial.println(wifiConfig.password);

    WiFi.disconnect();
    delay(200);
    WiFi.mode(WIFI_STA);
    WiFi.begin("", "");
    int connRes = WiFi.waitForConnectResult();
    WiFi.mode(WIFI_AP);

    WiFi.softAP(wifiConfig.ssid, wifiConfig.password);
    delay(500); 

    IPAddress ips = WiFi.softAPIP();

    server.on("/", [this](){handleRoot();});
    server.on("/config", HTTP_GET, [this](){configWiFi();});
    server.on("/config", HTTP_POST, [this](){configSubmit();});
    server.begin();
}

void Configuration::loopSystem()
{
    server.handleClient();
}
