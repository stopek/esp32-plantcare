#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

namespace PlantCare
{
    class WifiConnector
    {
    public:
        static void connect(const char *wifiSsid, const char *wifiPassword)
        {
            Serial.println("Connecting to " + String(wifiSsid));

            WiFi.begin(wifiSsid, wifiPassword);

            WiFi.persistent(false);
            WiFi.mode(WIFI_OFF);
            WiFi.mode(WIFI_STA);
            WiFi.begin(wifiSsid, wifiPassword);

            while (WiFi.status() != WL_CONNECTED)
            {
                delay(500);
                Serial.print(".");
            }

            Serial.println("Connection established!");
            Serial.println("Client IP: " + WiFi.localIP().toString());
        }
    };
}