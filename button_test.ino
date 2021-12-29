//--------------------------------------------------------------
// include libraries
//--------------------------------------------------------------
#include <Tone32.hpp>
#include <ezButton.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Stepper.h>
#include <DHT.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
  
//--------------------------------------------------------------
// pinouts
//--------------------------------------------------------------
#define LED_1_RED_PIN       22
#define LED_1_GREEN_PIN     1
#define LED_1_BLUE_PIN      17

#define BUTTON_1_PIN        15
#define BUTTON_2_PIN        5

#define MOTOR_IN1_PIN       32
#define MOTOR_IN2_PIN       33
#define MOTOR_IN3_PIN       25
#define MOTOR_IN4_PIN       26

#define OLED_MOSI_PIN       23
#define OLED_CLK_PIN        18
#define OLED_DC_PIN         4
#define OLED_CS_PIN         2
#define OLED_RESET_PIN      21

#define ENCODER_CLK_PIN     3
#define ENCODER_DT_PIN      19

#define LIGHT_SENSOR_PIN    36
#define DHT_PIN             27
#define MOTION_PIN          35
#define BUZZER_PIN          14

//--------------------------------------------------------------
// global settings
//--------------------------------------------------------------
#define SCREEN_WIDTH        128
#define SCREEN_HEIGHT       64
#define DEBOUNCE_TIME       50
#define REVOLUTION_STEPS    2048
#define DHT_TYPE            DHT11
#define MOTION_STOP_TIME    3000
#define BUZZER_CHANNEL      10

//--------------------------------------------------------------
// initialize global objects
//--------------------------------------------------------------
DHT dht(DHT_PIN, DHT_TYPE);

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  OLED_MOSI_PIN,
  OLED_CLK_PIN,
  OLED_DC_PIN,
  OLED_RESET_PIN,
  OLED_CS_PIN
);

ezButton button1(BUTTON_1_PIN);
ezButton button2(BUTTON_2_PIN);

Stepper myStepper(
  REVOLUTION_STEPS,
  MOTOR_IN1_PIN,
  MOTOR_IN3_PIN,
  MOTOR_IN2_PIN,
  MOTOR_IN4_PIN
);

Tone32 _tone32(BUZZER_PIN, BUZZER_CHANNEL);

AsyncWebServer server(80);

//--------------------------------------------------------------
// initialize global data variables
//--------------------------------------------------------------
int clickValue =                        0;
int lightValue =                        0;
int rotateValue =                       0;
int speedMotor =                        0;
float temperatureValue =                0.00;
float humidityValue =                   0.00;
String headerValue;

//--------------------------------------------------------------
// initialize interval variables
//--------------------------------------------------------------
const long intervalTime_motor =         5000;
const long intervalTime_light =         2000;


//--------------------------------------------------------------
// initialize temponary variables
//--------------------------------------------------------------
unsigned long previousTime_motor =      0;
unsigned long previousTime_light =      0;
unsigned long previousValue_motorCLK =  0;

unsigned long now =                     millis();
unsigned long lastTrigger =             0;
boolean startTimer =                    false;
boolean isRerenderOled =                true;
const int NOTES =                       7;
const note_t C_SCALE[NOTES] =           { 
                                          NOTE_C, 
                                          NOTE_D, 
                                          NOTE_E, 
                                          NOTE_F, 
                                          NOTE_G, 
                                          NOTE_A,
                                          NOTE_B 
                                        };
const char C_SCALE_CHARS[NOTES] =       { 
                                          'C', 'D', 'E', 'F', 'G', 'A', 'B'
                                        };
note_t _lastNote = NOTE_C;

//--------------------------------------------------------------
// initialize global settings/variables
//--------------------------------------------------------------
const char* ssid     =                  "";
const char* password =                  "";

//--------------------------------------------------------------
// functions
//--------------------------------------------------------------
void setLEDColor(int rgb[3], int ledNumber)  
{
  int redChannel = ledNumber * 3;
  int greenChannel = redChannel + 1;
  int blueChannel = redChannel + 2;
  
  ledcWrite(redChannel, rgb[0]);   
  ledcWrite(greenChannel, rgb[1]);  
  ledcWrite(blueChannel, rgb[2]);   
}

void initializeLED(int redPin, int greenPin, int bluePin, int ledNumber, int frequency = 5000, int resolution = 8) {
  int redChannel = ledNumber * 3;
  int greenChannel = redChannel + 1;
  int blueChannel = redChannel + 2;
  
  ledcAttachPin(redPin, redChannel);  
  ledcAttachPin(greenPin, greenChannel);  
  ledcAttachPin(bluePin, blueChannel);
  
  ledcSetup(redChannel, frequency, resolution);  
  ledcSetup(greenChannel, frequency, resolution);  
  ledcSetup(blueChannel, frequency, resolution);   
}

void renderOLED() {
  if (!isRerenderOled) {
    return;
  }
  
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  
  display.setTextSize(1);
  display.setCursor(0,0);
  String clickString = "Clicked: " + String(clickValue);
  display.print(clickString);

  display.setTextSize(1);
  display.setCursor(0,10);
  String light = "Light: " + String(lightValue);
  display.print(light);

  display.setCursor(0,20);
  String rotate = "Rotate: " + String(rotateValue);
  display.print(rotate);

  display.setCursor(0,30);
  String motor = "Motor: " + String(speedMotor);
  display.print(motor);

  display.setCursor(0,40);
  String temperature = "Temperature: " + String(temperatureValue) + " C";
  display.print(temperature);

  display.setCursor(0,50);
  String humidity = "Humidity: " + String(humidityValue) + " %";
  display.print(humidity);
  
  display.display();
  
  isRerenderOled = false;
}

void play(int index, int duration = 500, int octave = 4) {
  note_t note = C_SCALE[index];

  if (_lastNote != note) {
    _tone32.playNote(note, octave, duration);
  }

  _tone32.update();
  _lastNote = note;
}

void blueLight() {
  int color[3] = { 0, 0, 255 };
  setLEDColor(color, 0); 
}

void redLight() {
  int color[3] = { 255, 0, 0 };
  setLEDColor(color, 0);
}

void greenLight() {
  int color[3] = { 0, 255, 0 };
  setLEDColor(color, 0);
}

void noLight() {
  int color[3] = { 0, 0, 0 };
  setLEDColor(color, 0);
}

void anyColor(char *hexColor) {
  Serial.println("Any color");
  long int rgb=atol(hexColor+1);
  
  char r=(char)(rgb>>16);
  char g=(char)(r>>8);
  char b=(char)rgb;
 
  int color[3] = { r, g, b };
  setLEDColor(color, 0);
}

void IRAM_ATTR detectsMovement() {
  Serial.println("MOTION DETECTED!!!");
  startTimer = true;
  lastTrigger = millis();
}

const char index_html[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML>
  <html>
  <head>
      <title>ESP SERVER</title>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1">
  
      <link rel="preconnect" href="https://fonts.googleapis.com">
      <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
      <link href="https://fonts.googleapis.com/css2?family=Bebas+Neue&display=swap" rel="stylesheet">
      <style>
          body, html {
              box-sizing: border-box;
              margin: 0;
              background: #2b2b2b;
              padding: 0;
              width: 100%;
              height: 100%;
              font-family: 'Bebas Neue', cursive;
          }
  
          #content {
              display: flex;
              gap: 15px;
              flex-direction: column;
              padding: 15px;
          }
  
          .actions {
              word-spacing: 100vw;
          }
  
          .actions, .sensors {
              display: flex;
              gap: 15px;
              line-height: 1;
              flex-wrap: wrap;
          }
  
          .actions .action {
              flex-grow: 1;
              flex-shrink: 1;
              flex-basis: 0;
              text-decoration: none;
              padding: 20px;
              background: black;
              color: white;
              font-size: 50px;
              display: flex;
              align-items: center;
              gap: 15px;
          }
  
          .actions .action.blue {
              background: #1cb5c9;
          }
  
          .actions .action.red {
              background: #c91c41;
          }
  
          .actions .action form {
              display: flex;
              gap: 5px;
          }
  
          .sensor {
              padding: 15px 35px;
              flex-direction: column;
              background: white;
              display: flex;
              justify-content: center;
              line-height: 1;
              white-space: nowrap;
              flex-grow: 1;
              flex-shrink: 1;
              flex-basis: 0;
          }
  
          .sensor span {
              font-size: 25px;
          }
  
          .sensor strong {
              font-size: 45px;
          }
      </style>
  </head>
  <body>
  <div id="content">
      <div class="sensors">
          <div class="sensor">
              <span>Temp</span>
              <strong>%s °C</strong>
          </div>
          <div class="sensor">
              <span>Humidity</span>
              <strong>%s %</strong>
          </div>
          <div class="sensor">
              <span>Light</span>
              <strong>%s</strong>
          </div>
      </div>
      <div class="actions">
          <a class="action blue" href="/led/blue">
              BLUE LED
          </a>
          <a class="action red" href="/led/red">
              RED LED
          </a>
      </div>
  
      <div class="actions">
          <a class="action" href="/captain/left">
              CAPTAIN LEFT
          </a>
          <a class="action" href="/captain/right">
              CAPTAIN RIGHT
          </a>
      </div>

      <div class="actions">
        <a class="action" href="/alert">
          ALERT
        </a>
      </div>
  </div>
  </body>
</html>
)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void homePageResponse(AsyncWebServerRequest *request) {
  AsyncResponseStream *response = request->beginResponseStream("text/html");
  
  response->addHeader("Server","ESP Async Web Server");
  response->printf(index_html, String(temperatureValue), String(humidityValue), String(lightValue));
  
  request->send(response);
}

void initializeBuzz(int buzzerPin, int channel = 0, int frequency = 5000, int resolution = 8) {
  // configure LED PWM functionalitites
  ledcSetup(channel, frequency, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(buzzerPin, channel);
}

//--------------------------------------------------------------
// loop & setup.
//--------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  
  // initialize pins
  pinMode (ENCODER_CLK_PIN, INPUT);
  pinMode (ENCODER_DT_PIN, INPUT);
  pinMode (MOTION_PIN, INPUT_PULLUP);

  // temperature & humidity sensor
  dht.begin();
  
  // set debounce time
  button1.setDebounceTime(DEBOUNCE_TIME);
  button2.setDebounceTime(DEBOUNCE_TIME);

  // initialize LED RGB
  initializeLED(LED_1_RED_PIN, LED_1_GREEN_PIN, LED_1_BLUE_PIN, 0);

  // initialize BUZZER
  initializeBuzz(BUZZER_PIN, BUZZER_CHANNEL);

  myStepper.setSpeed(10);

  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println("SSD1306 allocation failed");
  }

  attachInterrupt(digitalPinToInterrupt(MOTION_PIN), detectsMovement, RISING);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
  }
  
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Setup done!");

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) { 
    homePageResponse(request);
  });
  
  server.on("/led/blue", HTTP_GET, [] (AsyncWebServerRequest *request) {
    blueLight();
    homePageResponse(request);
  });
  
  server.on("/led/red", HTTP_GET, [] (AsyncWebServerRequest *request) {
    redLight();
    homePageResponse(request);
  });

  server.on("/captain/left", HTTP_GET, [] (AsyncWebServerRequest *request) {
    myStepper.step(-500);
    isRerenderOled = true;
    homePageResponse(request);
  });
  
  server.on("/captain/right", HTTP_GET, [] (AsyncWebServerRequest *request) {
    myStepper.step(500);
    isRerenderOled = true;
    homePageResponse(request);
  });
  
  server.on("/alert", HTTP_GET, [] (AsyncWebServerRequest *request) {
    startTimer = true;
    lastTrigger = now;
    homePageResponse(request);
  });
  
 
  server.onNotFound(notFound);
  server.begin();
}

unsigned long previousTime_blink = 0;
bool lastIsRed = false;
void blinking(unsigned long currentTime) {
  if (currentTime - previousTime_blink > 500) {
    if (lastIsRed) {
      blueLight();
      play(5 , 6);
    } else {
      redLight();
      play(4, 6);
    }

    lastIsRed = !lastIsRed;
    previousTime_blink = currentTime;
  }
}

void loop() {
  now = millis();

  // motion sensor
  {
    if(startTimer) {
      if ((now - lastTrigger > MOTION_STOP_TIME)) {
        startTimer = false;
        _tone32.stopPlaying();
        noLight();
      } else {
        blinking(now);
      }
    }
  }

  // temperature & humidity sensor
  if (now - previousTime_light >= intervalTime_light) {
    lightValue = analogRead(LIGHT_SENSOR_PIN);
    float tempTemperature = dht.readTemperature();
    if (!isnan(tempTemperature)) {
      temperatureValue = tempTemperature;
      isRerenderOled = true;
    }

    float tempHumidity = dht.readHumidity();
    if (!isnan(tempHumidity)) {
      humidityValue = tempHumidity;
      isRerenderOled = true;
    }
    
    previousTime_light = now;
  }

  // encoder rotator
  {
    int motorCLKValue = digitalRead(ENCODER_CLK_PIN);
    
    if (motorCLKValue != previousValue_motorCLK) {
      if (digitalRead(ENCODER_DT_PIN) != motorCLKValue) {
        rotateValue--;
      } else {
        rotateValue++;
      }
      
      isRerenderOled = true;
    }
    
    previousValue_motorCLK = motorCLKValue;
  }

  // buttons
  {
    button1.loop();
    button2.loop();
    
    if (button1.isPressed()) {
      clickValue--;
  
      speedMotor-=10;
      myStepper.step(speedMotor);
  
      redLight();
      isRerenderOled = true;
    }
      
    if (button2.isPressed()) {
      clickValue++;
      speedMotor+=10;
      myStepper.step(speedMotor);
      
      blueLight();
      isRerenderOled = true;
    }
  }
  
  renderOLED();
}


//  pinMode(TRIG_PIN, OUTPUT); // Sets the trigPin as an Output
//  pinMode(ECHO_PIN, INPUT); // Sets the echoPin as an Input

//  float distanceCm = checkDistance();
//  play(distanceCm);

//#define TRIG_PIN 5
//#define ECHO_PIN 19

////define sound speed in cm/uS
//#define SOUND_SPEED 0.034
//long duration;
//float checkDistance() {
//  // Clears the trigPin
//  digitalWrite(TRIG_PIN, LOW);
//  delayMicroseconds(2);
//  // Sets the trigPin on HIGH state for 10 micro seconds
//  digitalWrite(TRIG_PIN, HIGH);
//  delayMicroseconds(10);
//  digitalWrite(TRIG_PIN, LOW);
//
//  // Reads the echoPin, returns the sound wave travel time in microseconds
//  duration = pulseIn(ECHO_PIN, HIGH);
//
//  // Calculate the distance
//  return duration * SOUND_SPEED / 2;
//}
