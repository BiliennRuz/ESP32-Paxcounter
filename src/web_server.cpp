#include "web_server.h"     // Web server definition

//#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

//AsyncWebServer server(80);


void Setup_SPIFFS()
{
    ESP_LOGI(TAG,"------------ SPIFFS files --------------");
    if(!SPIFFS.begin())
    {
        ESP_LOGE(TAG, "Erreur SPIFFS...");
        return;
    }

    File root = SPIFFS.open("/");
    File file = root.openNextFile();

    while(file)
    {
        ESP_LOGI(TAG,"File: %s", file.name());
        file.close();
        file = root.openNextFile();
    }
}

/*
void Setup_Web_Server()
{
    ESP_LOGI(TAG,"------------- Web server -----------");
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(SPIFFS, "/index.html", "text/html");
    });

    server.on("/wifi.html", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(SPIFFS, "/wifi.html", "text/html");
    });

    server.on("/balance.html", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(SPIFFS, "/balance.html", "text/html");
    });

    server.on("/about.html", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(SPIFFS, "/about.html", "text/html");
    });

    server.on("/w3.css", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(SPIFFS, "/w3.css", "text/css");
    });

    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(SPIFFS, "/script.js", "text/javascript");
    });

    server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(SPIFFS, "/favicon.ico", "image/x-icon");
    });

    server.on("/readDeviceName", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        ESP_LOGD(TAG, "HTTP_GET /readDeviceName %s", deviceName);
        request->send(200, "text/plain", deviceName);
    });

    server.on("/readStatus", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        ESP_LOGD(TAG, "HTTP_GET /readStatus %s", statusDevice);
        request->send(200, "text/plain", statusDevice);
    });

    server.on("/readTemperature", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        ESP_LOGD(TAG, "HTTP_GET /readTemeprature %s", msgTemp);
        request->send(200, "text/plain", msgTemp);
    });

    server.on("/readHumidity", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        ESP_LOGD(TAG, "HTTP_GET /readHumidity %s", msgHumi);
        request->send(200, "text/plain", msgHumi);
    });

    server.on("/readMass", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        ESP_LOGD(TAG, "HTTP_GET /readMass %s", msgMass);
        request->send(200, "text/plain", msgMass);
    });

    server.on("/readLoadBattery", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        ESP_LOGD(TAG, "HTTP_GET /readLoadBattery %s", loadBattery);
        request->send(200, "text/plain", loadBattery);
    });

    server.on("/readBatteryLife", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        ESP_LOGD(TAG, "HTTP_GET /readBatteryLife %s", msgBatLife);
        request->send(200, "text/plain", msgBatLife);
    });

    server.on("/readCalibrationValue", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        int calibrationValue = getCalibrationValueHX711();
        String calibration = String(calibrationValue);
        ESP_LOGD(TAG, "HTTP_GET /readCalibrationValue %s", calibration);
        request->send(200, "text/plain", calibration);
    });

    server.on("/readMassAbsolute", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        String massAbsolute = String(valueMassAbsolute);
        ESP_LOGD(TAG, "HTTP_GET /readMassAbsolute %s", massAbsolute);
        request->send(200, "text/plain", massAbsolute);
    });

    server.on("/readMassMaxAlarm", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        String massMaxAlarm = String(valueMassMaxAlarm);
        ESP_LOGD(TAG, "HTTP_GET /readMassMaxAlarm %s", massMaxAlarm);
        request->send(200, "text/plain", massMaxAlarm);
    });

    server.on("/readMassMaxLimit", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        String massMaxLimit = String(valueMassMaxLimit,0);
        ESP_LOGD(TAG, "HTTP_GET /readMassMaxLimit %s", massMaxLimit);
        request->send(200, "text/plain", massMaxLimit);
    });

    server.on("/readMassSwarming", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        String massSwarming = String(valueMassSwarming,0);
        ESP_LOGD(TAG, "HTTP_GET /readMassSwarming %s", massSwarming);
        request->send(200, "text/plain", massSwarming);
    });

    server.on("/readProjectName", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        ESP_LOGD(TAG, "HTTP_GET /readWProjectName %s", projectName);
        request->send(200, "text/plain", projectName);
    });

    server.on("/readVersionFirmware", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        ESP_LOGD(TAG, "HTTP_GET /readWVersionFirmware %s", versionFirmware);
        request->send(200, "text/plain", versionFirmware);
    });

    server.on("/tare", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        ESP_LOGD(TAG, "HTTP_GET /tare");
        tareRequestHX711 = true;
        request->send(200);
    });

    server.on("/calibrate", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        ESP_LOGD(TAG, "HTTP_GET /calibrate");
        calibrationRequestHX711 = true;
        request->send(200);
    });

    server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        ESP_LOGD(TAG, "HTTP_GET /update");
        request->send(200);
    });

    server.begin();
    ESP_LOGI(TAG, "Serveur actif!");
}
*/