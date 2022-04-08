namespace PlantCare
{
    class SensorsResult
    {
    private:
        float airHumidityValue = 0.00;
        float airTemperatureValue = 0.00;

        float soilTemperatureValue = 0.00;
        float soilHumidityValue = 0.00;

        float lightValue = 0.00;

    public:
        void setAirHumidity(float humidity)
        {
            airHumidityValue = humidity;
        }

        void setAirTemperature(float temperature)
        {
            airTemperatureValue = temperature;
        }

        void setSoilTemperature(float temperature)
        {
            soilTemperatureValue = temperature;
        }

        void setSoilHumidity(float humidity)
        {
            soilHumidityValue = humidity;
        }

        void setLight(float light)
        {
            lightValue = light;
        }

        float getAirTemperature()
        {
            return airTemperatureValue;
        }

        float getAirHumidity()
        {
            return airHumidityValue;
        }

        float getSoilTemperature()
        {
            return soilTemperatureValue;
        }

        float getSoilHumidity()
        {
            return soilHumidityValue;
        }

        float getLight()
        {
            return lightValue;
        }

        int getSoilHumidityPercentage(int airValue, int waterValue)
        {
            return map(soilHumidityValue, airValue, waterValue, 0, 100);
        }
    };
}