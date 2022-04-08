//--------------------------------------------------------------
// include libraries
//--------------------------------------------------------------
#include <SPI.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ezButton.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "settings.h";
#include "classes/Logger.h";
#include "classes/OLED.h";
#include "classes/LED.h";
#include "classes/AccessPoint.h";
#include "classes/WebServer.h";

Logger logger;

OLED oled(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
LED led0(LED_1_RED_PIN, LED_1_GREEN_PIN, LED_1_BLUE_PIN);
AccessPoint accessPoint;
WebServer webServer(80);

unsigned long now = millis();

ezButton button1(BUTTON_1_PIN);
ezButton button2(BUTTON_2_PIN);
ezButton button3(BUTTON_3_PIN);
ezButton button4(BUTTON_4_PIN);
ezButton button5(BUTTON_5_PIN);
ezButton button6(BUTTON_6_PIN);

//--------------------------------------------------------------
// loop & setup.
//--------------------------------------------------------------
void setup()
{
  Serial.begin(115200);

  // Wait for serial to initialize.
  while (!Serial)
  {
    delay(100);
  }
  delay(1000);

  logger.log("");
  logger.log("-------------------------------------");
  logger.log("PLANT CARE SERVER");
  logger.log("-------------------------------------");
  logger.log("");

  // initialize LED
  led0.initialize(0);

  // initialize OLED SCREEN
  oled.initialize();

  // initialize access point
  accessPoint.connectToWiFi(AP_SSID, AP_PASSWORD);

  // create server instance
  webServer.inititialize();

  led0.greenLight(0);

  // set debounce time
  button1.setDebounceTime(DEBOUNCE_TIME);
  button2.setDebounceTime(DEBOUNCE_TIME);
  button3.setDebounceTime(DEBOUNCE_TIME);
  button4.setDebounceTime(DEBOUNCE_TIME);
  button5.setDebounceTime(DEBOUNCE_TIME);
  button6.setDebounceTime(DEBOUNCE_TIME);
}

void loop()
{
  now = millis();

  button1.loop();
  button2.loop();
  button3.loop();
  button4.loop();
  button5.loop();
  button6.loop();

  // listen for oled text changes
  oled.renderStringLines(true);

  // start auto scrolling oled
  if (button1.isPressed())
  {
    logger.log("Button #1 - pressed");
    oled.startScrollScreen();
  }

  // start auto scrolling oled
  if (button2.isPressed())
  {
    logger.log("Button #2 - pressed");
    oled.stopScrollScreen();
  }

  if (button3.isPressed()) {
    logger.log("Button #3 - pressed");
    led0.redLight();
  }

  if (button4.isPressed()) {
    logger.log("Button #4 - pressed");
    led0.greenLight();
  }

  if (button5.isPressed()) {
    logger.log("Button #5 - pressed");
    led0.blueLight();
  }

  if (button6.isPressed()) {
    logger.log("Button #6 - pressed");
    led0.noLight(0);
  }
}
