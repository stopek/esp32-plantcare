#include <WiFi.h>

extern OLED oled;
extern LED led0;
extern Logger logger;

class AccessPoint
{
public:
    int clients = 0;

    AccessPoint()
    {
    }

    void APStart(WiFiEvent_t event, WiFiEventInfo_t info)
    {
        led0.blueLight(0);
    }

    void APClientDisconnected(WiFiEvent_t event, WiFiEventInfo_t info)
    {
        logger.log("Clients DOWN");
        clients--;
    }

    void APClientConnected(WiFiEvent_t event, WiFiEventInfo_t info)
    {

        logger.log("Clients UP");
        clients++;
    }

    void connectToWiFi(const char *ssid, const char *password)
    {
        WiFi.mode(WIFI_STA);

        WiFi.begin(ssid, password);
        Serial.println();
        Serial.print("Connecting to WiFi");

        while (WiFi.status() != WL_CONNECTED)
        {
            Serial.print('.');
            delay(1000);
        }

        IPAddress myIP = WiFi.localIP();
        // oled.addLine("wifi-ip", myIP.toString());
        logger.log(myIP.toString());
    }

    // void createAccessPoint(const char *ssid, const char *password)
    // {
    //     WiFi.mode(WIFI_STA);
    //     WiFi.disconnect();
    //     delay(100);

    //     WiFi.onEvent(APStart, SYSTEM_EVENT_AP_START);

    //     WiFi.begin(ssid, password);
    //     if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    //         Serial.println("WiFi Failed!");
    //     }

    //     // IPAddress localIp(192, 168, 4, 1);
    //     // IPAddress gateway(192, 168, 1, 1);
    //     // IPAddress subnet(255, 255, 255, 0);

    //     // Serial.println();
    //     // Serial.println("Configuring access point...");

    //     // // WiFi.onEvent(APClientConnected, SYSTEM_EVENT_AP_STACONNECTED);
    //     // // WiFi.onEvent(APClientDisconnected, SYSTEM_EVENT_AP_STADISCONNECTED);

    //     // if (!WiFi.softAPConfig(localIp, gateway, subnet)) {
    //     //     Serial.println("WiFi - soft AP config set up problems");
    //     //     return;
    //     // }

    //     // Serial.println("WiFi - set AP config has been set up");
    //     // if (!WiFi.softAP(ssid, password)) {
    //     //     Serial.println("WiFi - soft AP starts up problems");
    //     //     return;
    //     // }


    //     // Serial.println("Access Point IP: " + myIP.toString());
    // }

    IPAddress getWifiIP()
    {
        return WiFi.softAPIP();
    }
};