// https://github.com/JanWielgus/SimpleDataStructures
#include "../../SimpleDataStructures/GrowingArray.h"
#include "../Logger.h"
#include <iostream>

using namespace Logging;

constexpr const char Headers[] = {'B', 'D', 'I', 'E'};

enum class LogType
{
	Base,
	Debug,
	Info,
	Error
};

Logger<LogType, Headers, 100> logger;

class Ser : public ILogMedium
{
	void log(const char *message) override
	{
		std::cout << message << std::endl;
	}
} ser;

int main()
{
	logger.bind(LogType::Base | LogType::Debug, &ser);
	auto a = logger.getLogMediums(LogType::Debug);

	for (uint8_t i = 0; i < 10; i++)
	{
		logger.log(LogType::Debug | LogType::Base, "tralfazz", 78U, "tralfazz?", ' ', true, "tralfazz!");
		logger.log(LogType::Debug, i);
	}
}
