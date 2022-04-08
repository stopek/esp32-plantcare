extern Logger logger;

class LED
{
public:
    int red, green, blue, frequency, resolution;

    LED(int redPin, int greenPin, int bluePin, int freq = 5000, int res = 8)
    {
        red = redPin;
        green = greenPin;
        blue = bluePin;

        frequency = freq;
        frequency = res;
    }


    void initialize(int ledNumber)
    {
        int redChannel = ledNumber * 3;
        int greenChannel = redChannel + 1;
        int blueChannel = redChannel + 2;

        ledcAttachPin(red, redChannel);
        ledcAttachPin(green, greenChannel);
        ledcAttachPin(blue, blueChannel);

        ledcSetup(redChannel, frequency, resolution);
        ledcSetup(greenChannel, frequency, resolution);
        ledcSetup(blueChannel, frequency, resolution);

        logger.log("Led #" + String(ledNumber) + " has been initialized");
    }

    void setColor(int rgb[3], int ledNumber)
    {
        int redChannel = ledNumber * 3;
        int greenChannel = redChannel + 1;
        int blueChannel = redChannel + 2;

        ledcWrite(redChannel, rgb[0]);
        ledcWrite(greenChannel, rgb[1]);
        ledcWrite(blueChannel, rgb[2]);

        logger.log("Set rgb(" + String(rgb[0]) + "," + String(rgb[1]) + "," + String(rgb[2]) + ") color on LED #" + String(ledNumber));
    }

    void redLight(int ledNumber = 0)
    {
        int color[3] = {255, 0, 0};
        setColor(color, ledNumber);
    }

    void greenLight(int ledNumber = 0)
    {
        int color[3] = {0, 255, 0};
        setColor(color, ledNumber);
    }

    void blueLight(int ledNumber = 0)
    {
        int color[3] = {0, 0, 255};
        setColor(color, ledNumber);
    }

    void noLight(int ledNumber = 0) {
        int color[3] = {0, 0, 0};
        setColor(color, ledNumber);
    }

    unsigned long lastLightOn = 0;
    void lightAndOff(int ledNumber, int color[3], int time = 1000) {
        
    }
};