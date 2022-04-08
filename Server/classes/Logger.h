class Logger
{
public:
    Logger()
    {
    }
    
    void log(String message) {
        Serial.println(message);
    }
};