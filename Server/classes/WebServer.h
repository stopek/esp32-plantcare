#include "../helpers/webserver.h";

String sensorId = "";
String airHumidityValue = "";
String airTemperatureValue = "";
String soilTemperatureValue = "";
String soilHumidityValue = "";
String lightValue = "";
String soilHumidityPercentageValue = "";

extern OLED oled;

class WebServer
{
private:
    int receivedDataCounter = 0;
    void notFound(AsyncWebServerRequest *request)
    {
        request->send(404, "text/plain", "Not found");
    }

public:
    AsyncWebServer *server;

    WebServer(int port) {
        server = new AsyncWebServer(port);
    }

    void inititialize()
    {
        server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
            AsyncResponseStream *response = request->beginResponseStream("text/html");

            response->addHeader("Server", "ESP Async Web Server");
            response->printf(index_html, sensorId, soilTemperatureValue, soilHumidityValue, soilHumidityPercentageValue, airTemperatureValue, airHumidityValue, lightValue);

            request->send(response); 
        });

        server->on("/update-sensor", HTTP_GET, [&oled](AsyncWebServerRequest *request) {
            AsyncResponseStream *response = request->beginResponseStream("text/html");
            
            response->addHeader("Server", "UpdateSensor");

            if (request->hasParam("sensor_id")) {
                sensorId = request->getParam("sensor_id")->value();
                oled.addOrChangeLine("#", sensorId);
            }

            if (request->hasParam("soil_temperature")) {
                soilTemperatureValue = request->getParam("soil_temperature")->value();
                oled.addOrChangeLine("SoilTemp", soilTemperatureValue + "C");
            }

            if (request->hasParam("soil_humidity")) {
                soilHumidityValue = request->getParam("soil_humidity")->value();
                oled.addOrChangeLine("SoilHum", soilHumidityValue);
            }

            if (request->hasParam("soil_humidity_percentage")) {
                soilHumidityPercentageValue = request->getParam("soil_humidity_percentage")->value();
                oled.addOrChangeLine("SoilHum", soilHumidityPercentageValue + "%");
            }


            if (request->hasParam("air_temperature")) {
                airTemperatureValue = request->getParam("air_temperature")->value();
                oled.addOrChangeLine("AirTemp", airTemperatureValue + "C");
            }

            if (request->hasParam("air_humidity")) {
                airHumidityValue = request->getParam("air_humidity")->value();
                oled.addOrChangeLine("AirHum", airHumidityValue + "%");
            }

            if (request->hasParam("light")) {
                lightValue = request->getParam("light")->value();
                oled.addOrChangeLine("Light", lightValue);
            }

            logger.log("Data has been received.");

            response->print("Data has been received.");
            request->send(response); 
        });

        server->begin();

        oled.addLine("//", WiFi.localIP().toString() + ":80");
        logger.log("Server started");
    }
};