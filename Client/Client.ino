//--------------------------------------------------------------
// include libraries
//--------------------------------------------------------------
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "classes/SensorsReader.h";
#include "classes/Server.h";
#include "classes/WifiConnector.h";

//--------------------------------------------------------------
// pinouts
//--------------------------------------------------------------
#define DHT_PIN 2
#define SOIL_HUMIDITY_PIN 17
#define TEMPERATURE_PIN 5

//--------------------------------------------------------------
// global settings
//--------------------------------------------------------------
#define DHT_TYPE            DHT11
#define AP_SSID             ""
#define AP_PASSWORD         ""

//--------------------------------------------------------------
// initialize global settings/variables
//--------------------------------------------------------------

const int airValue = 740;   // max
const int waterValue = 305; // min

unsigned long previousTime_sensor = 0;
const unsigned int sensorsDelay = 1000 * 10;

unsigned long now = millis();

//--------------------------------------------------------------
// initialize global objects
//--------------------------------------------------------------
DHT dht(DHT_PIN, DHT_TYPE);
OneWire oneWire(TEMPERATURE_PIN);
DallasTemperature sensors(&oneWire);

void action()
{
  PlantCare::SensorsResult result = PlantCare::SensorsReader::read(sensors, dht, SOIL_HUMIDITY_PIN);

  const PlantCare::dataDictionary postData[]{
      {"soil_temperature", String(result.getSoilTemperature())},
      {"soil_humidity", String(result.getSoilHumidity())},
      {"air_temperature", String(result.getAirTemperature())},
      {"air_humidity", String(result.getAirHumidity())},
      {"light", String(result.getLight())},
      {"soil_humidity_percentage", String(result.getSoilHumidityPercentage(airValue, waterValue))}};

  PlantCare::Server::sendData("http://192.168.1.44:80", postData);
}

void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(2000);

  // Wait for serial to initialize.
  while (!Serial)
  {
    delay(100);
  }
  delay(1000);

  Serial.println("");
  Serial.println("-------------------------------------");
  Serial.println("PLANT CARE #" + String(ESP.getChipId()));
  Serial.println("-------------------------------------");
  Serial.println("");

   PlantCare::WifiConnector::connect(AP_SSID, AP_PASSWORD);

   // temperature & humidity sensor
   dht.begin();

   // temperature sensor
   sensors.begin();

   action();

  // Serial.println("Going into deep sleep for 5 seconds");
  // ESP.deepSleep(5e6);
}

void loop()
{
  now = millis();

  if (now - previousTime_sensor >= sensorsDelay)
  {
    action();
    previousTime_sensor = now;
  }
}
