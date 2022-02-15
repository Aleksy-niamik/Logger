// #include <mbed.h>

#include "Logger.h"

using namespace EnumReflection;

// DigitalOut  myled(PC_13);   // on-board LED
// Serial pc(PA_9, PA_10); // tx, rx

enum class Bal
{
  bal0, bal1, bal2, bal3, bal4, bal5, bal6, bal7
};

constexpr const char Headers[]={'8','A','a', 'b', 'L'};

enum class LogType
{
  Base, Debug=2, Info=1, Error=3, qasdfghjk
};

class Ser : public Logging::ILogMedium
{
  void log(const char* message) override
  {
    // pc.printf(message);
  }
} ser;

Logging::Logger<LogType, Headers, 100> logger2;

int main()
{
  __asm__("#asd");
  volatile uint8_t zmienna = enum_elements_count<LogType>();
  __asm__("#test");

  
  logger2.bind(LogType::Base, &ser);
  logger2.unbind(LogType::Base, &ser);
  auto a = logger2.getLogMediums(LogType::Debug);
  while(1)
  {
    logger2.log(LogType::Debug | LogType::Base, "tralfazz", 78U, "tralfazz?", true, "tralfazz!", '\n');
    logger2.log(LogType::Debug, zmienna);
    // wait_ms(3000); 
  }
}