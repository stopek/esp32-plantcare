#include "SensorsResult.h";

namespace PlantCare
{
    class SensorsReader
    {
    public:
        static SensorsResult read(DallasTemperature sensors, DHT dht, int soilHumiditySensorPin)
        {
            SensorsResult sensorsData;

            // load soil temperature
            sensors.requestTemperatures();
            float soilTemperature = sensors.getTempCByIndex(0);
            if (!isnan(soilTemperature))
            {
                Serial.println("SoilTemperature: " + String(soilTemperature));
                sensorsData.setSoilTemperature(soilTemperature);
            }

            // load soil humidity
            int soilHumidity = analogRead(soilHumiditySensorPin);
            if (!isnan(soilHumidity))
            {
                sensorsData.setSoilHumidity(soilHumidity);
                Serial.println("SoilHumidity: " + String(soilHumidity));
            }

            // load air temperature
            float airTemperature = dht.readTemperature();
            if (!isnan(airTemperature))
            {
                sensorsData.setAirTemperature(airTemperature);
                Serial.println("AirTemperature: " + String(airTemperature));
            }

            // load air humidity
            float airHumidity = dht.readHumidity();
            if (!isnan(airHumidity))
            {
                sensorsData.setAirHumidity(airHumidity);
                Serial.println("AirHumidity: " + String(airHumidity));
            }

            return sensorsData;
        }
    };
}