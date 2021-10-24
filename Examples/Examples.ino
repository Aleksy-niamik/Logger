#include "/home/aleksy/cppProjects/Logger/Logger.h"
#include "/home/aleksy/cppProjects/Logger/Logger.cpp"


Logging::Logger<8> logger;


class Medium1 : public Logging::ILogMedium
{
    void log(const char* message) override
    {
        Serial.println(message);
    }
} medium1;


void setup()
{
    delay(1000);
    Serial.begin(115200);
    logger.bind(Logging::LogType::Base, &medium1);
    logger.setPrecision(3);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, 0);
}


void loop()
{   
    // logger.log(Logging::LogType::Base, -3.123345f, -4.8);
    // logger.log(Logging::LogType::Base, "hjkhk");
    // logger.error("temperatura wynosi ", -345.6, "K");
    //logger.error(';', 'l', 'k', 'j', 'h', 'g', 'f');
    logger.error("Ala ma k");
    logger.error("Ala ma ko");

    logger.error(12345678);
    logger.error(123456789);
    logger.error(-1234567);
    logger.error(-12345678);

    logger.error(1234.100f);
    logger.error(12345.120);
    logger.error(-123.100);
    logger.error(-1234.120);
    
    logger.setPrecision(0);
    logger.error(1234567.);
    logger.error(12345678.);
    logger.error(-123456.);
    logger.error(-1234567.);

    //scenariusze pojedynczego loga działają
    logger.error("-----");
    //pozostają testy dla łączenia logów (funkcje wariadyczne)

    logger.setPrecision(3);
    logger.error("1234", 'k','l','d','s','a','c');
    logger.error("asdfghgfdsasdf", "dsfss", 789.);
    logger.error(123.123, 123.123);
    logger.error(12345678, 123);
    logger.error(1234567, 123);
    logger.error(123456789, 123);
    
    delay(1000);
}
