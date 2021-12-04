/**
 * @file Logger.h
 * @author Aleksy Walczak (aleksyww@gmail.com)
 * @date 2021-04-09
 */

#ifndef LOGGER_H
#define LOGGER_H

#include "EnumReflection.h"
#include "FlagEnum.h"
#include "ILogMedium.h"
#include "Pair.h"

#include <GrowingArray.h>
#include <ArrayIterator.h>


namespace Logging
{
    template <class E, const char (&Headers)[EnumElementsCount<E>::value], uint16_t MaxLogSize = 30>
    class Logger
    {
        static_assert(std::is_enum<E>::value, " ");  // TODO: errrrr mess
        static_assert(is_scoped_enum<E>(), "enum musi byc enum class");
        // TODO: check if Headers have correct values

        static constexpr const int EnumCount = EnumElementsCount<E>::value;
        
        typedef FlagEnum<E> LogType;
        typedef Pair<uint32_t, ILogMedium*> Binding;

        SimpleDataStructures::GrowingArray<Binding> bindings;

        static constexpr std::size_t BufferSize = MaxLogSize + EnumCount + 1;
        char buffer[BufferSize + 1];
        uint8_t startIndex;
        uint8_t endIndex = EnumCount + 1;
        bool isOverflowed = false;

        char headerSeparator = '/';
        char bufferOverflowCharacter = '*';
        char decimalSeparator = '.';

        uint8_t precision = 2;
        uint32_t base = 100;
    public:
        /**
         * @brief Construct a new Logger object
         */
        Logger() {};
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

        void bind(LogType logType, ILogMedium* transmitter);
        void unbind(LogType logType, ILogMedium* transmitter);
        SimpleDataStructures::GrowingArray<ILogMedium*> getLogMediums(LogType logType) const;

        void setPrecision(uint8_t precision);

        void setHeaderSeparator(char headerSeparator);
        void setBufferOverflowCharacter(char bufferOverflowCharacter);
        void setDecimalSeparator(char decimalSeparator);

        char getHeaderSeparator() const;
        char getBufferOverflowCharacter()const;
        char getDecimalSeparator() const;

        template <class T>
        void log(LogType logType, T item);

        template <class First, class... Args>
        void log(LogType logType, First first, Args... args);

    private:
        template <class T>
        void addToBuffer(T integer);

        void addToBuffer(char c);
        void addToBuffer(const char* str);
        void addToBuffer(float number);
        void addToBuffer(double number);

        void prepareHeader(uint32_t logType);
    };
}


#include "Logger.tpp"


#endif // __LOGGER_H__
