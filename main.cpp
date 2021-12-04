// #include <mbed.h>

#include "Logger.h"


// DigitalOut  myled(PC_13);   // on-board LED
// Serial pc(PA_9, PA_10); // tx, rx

enum class Bal
{
  bal0, bal1, bal2, bal3, bal4, bal5, bal6, bal7
};

constexpr const char Headers[]={'8','A','a', 'b', 'c', 'd', 'e', 'g'};

Logging::Logger<Bal, Headers, 100> logger2;

class Ser : public Logging::ILogMedium
{
  void log(const char* message) override
  {
    // pc.printf(message);
  }
} ser;


int main()
{
  logger2.bind(Bal::bal1, &ser);
  logger2.bind(Bal::bal2, &ser);
  logger2.unbind(Bal::bal3, &ser);
  auto a = logger2.getLogMediums(Bal::bal2);
  while(1)
  {
    logger2.log(Bal::bal3 | Bal::bal1 | Bal::bal2, "tralfazz", 78, "tralfazz?", true, "tralfazz!", '\n');
    // wait_ms(3000);  
  }
}