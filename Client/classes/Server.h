#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

namespace PlantCare
{
    typedef struct
    {
        String name;
        String value;
    } dataDictionary;

    class Server
    {
    public:
        static void sendData(String serverName, const dataDictionary data[])
        {
            if (WiFi.status() == WL_CONNECTED)
            {
                WiFiClient client;
                HTTPClient http;
                

                String getRequestPath = String(serverName) + "/update-sensor/?sensor_id=" + String(ESP.getChipId());
                for (uint8_t i = 0; i < sizeof(dataDictionary) / sizeof(data); ++i)
                {
                    getRequestPath += "&" + data[i].name + "=" + data[i].value;
                }

                http.begin(client, getRequestPath);
                int httpResponseCode = http.GET();
                if (httpResponseCode > 0)
                {
                    Serial.println("HTTP Response code: " + String(httpResponseCode));
                    Serial.println("HTTP Response data: " + http.getString());
                }
                else
                {
                    Serial.println("HTTP Response ERROR CODE: " + String(httpResponseCode));
                }

                http.end();
            }
            else
            {
                Serial.println("WiFi Disconnected");
            }
        }
    };
}