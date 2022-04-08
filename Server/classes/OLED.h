extern Logger logger;

struct OledRowItem
{
    String name;
    String value;
};

const int len = 100;

class OLED
{
public:
    bool renderOled;
    struct OledRowItem data[len] = {};

    Adafruit_SSD1306 *display;

    enum
    {
        ARRAY_SIZE = sizeof(data) / sizeof(data[0])
    };

    OLED(int w, int h, int mosi, int clk, int dc, int re, int cs)
    {
        display = new Adafruit_SSD1306(w, h, mosi, clk, dc, re, cs);
        renderOled = true;
    }

    void initialize()
    {
        if (!display->begin(SSD1306_SWITCHCAPVCC, 0x3D))
        {
            logger.log("OLED initialization failed");
        }
        else
        {
            logger.log("OLED initialized");
        }

        display->clearDisplay();
        display->display();
        delay(1000);
    }

    int_fast64_t filledLines()
    {
        int result = 0;
        for (int i = 0; i < ARRAY_SIZE && data[i].name.length() > 0; i++)
        {
            result++;
        }

        return result;
    }

    void renderStringLines(bool scrollLong = false)
    {
        if (!renderOled)
        {
            return;
        }

        display->clearDisplay();
        display->setTextColor(SSD1306_WHITE);
        display->setTextSize(1);
        display->setCursor(0, 0);

        for (int i = 0; i < ARRAY_SIZE && data[i].name.length() > 0; i++)
        {
            display->println(data[i].name + ":" + data[i].value);
            logger.log("Add to OLED " + data[i].name + ":" + data[i].value);
        }

        display->display();

        if (scrollLong && filledLines() > 7) {
            startScrollScreen();
        }

        renderOled = false;

        logger.log("Render OLED string list");
    }

    void addLine(String name, String value)
    {
        OledRowItem item;
        item.name = name;
        item.value = value;

        int position = filledLines();
        data[position] = item;
        renderOled = true;

        logger.log("Added $" + name + "=" + value + " to oled list");
    }

    bool modifyItem(String name, String value)
    {
        bool found = false;
        for (int i = 0; i < ARRAY_SIZE && data[i].name.length() > 0; i++)
        {
            if (data[i].name == name)
            {
                data[i].value = value;
                found = true;
            }
        }

        return found;
    }

    void addOrChangeLine(String name, String value)
    {
        if (!modifyItem(name, value))
        {
            addLine(name, value);
        }
    }

    unsigned long previousScroll = 0;

    void startScrollScreen()
    {
        logger.log("Start scrolling");

        display->stopscroll();
        display->startscrolldiagleft(-1, -1);
    }

    void stopScrollScreen()
    {
        logger.log("Stop scrolling");

        display->stopscroll();
        previousScroll = 0;
    }
};