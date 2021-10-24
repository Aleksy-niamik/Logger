/**
 * @file Logger.h
 * @author Aleksy Walczak (aleksyww@gmail.com)
 * @date 2021-04-09
 */

#ifndef LOGGER_H
#define LOGGER_H

#include "ILogMedium.h"
#include "LogType.h"
#include "Pair.h"
#include <GrowingArray.h>
#include <ArrayIterator.h>

#include <stdint.h>

namespace Logging
{
    template <uint16_t MaxLogSize = 30>
    class Logger
    {
        typedef Pair<LogType, ILogMedium*> Binding;

        SimpleDataStructures::GrowingArray<Binding> bindings;

        static constexpr size_t BufferSize = MaxLogSize + LogType::Count + 1;
        char buffer[BufferSize + 1];
        uint8_t startIndex;
        uint8_t endIndex = LogType::Count + 1;
        bool isOverflowed = false;

        char headerSeparator = '/';
        char bufferOverflowCharacter = '*';
        char decimalSeparator = '.';

        uint8_t precision = 2;
        public: uint32_t base = 100;
    public:
        Logger() {};
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

        void bind(LogType logType, ILogMedium* transmitter);
        void unbind(LogType logType, ILogMedium* transmitter);
        SimpleDataStructures::GrowingArray<ILogMedium*> getLogMediums(LogType logType);

        void setPrecision(uint8_t precision);

        void setHeaderSeparator(char headerSeparator);
        void setBufferOverflowCharacter(char bufferOverflowCharacter);
        void setDecimalSeparator(char decimalSeparator);

        char getHeaderSeparator();
        char getBufferOverflowCharacter();
        char getDecimalSeparator();

        template <class T>
        void log(LogType logType, T item);

        template <class First, class... Args>
        void log(LogType logType, First first, Args... args);

        template <class First, class... Args>
        void error(First first, Args... args);

        template <class First, class... Args>
        void debug(First first, Args... args);

        template <class First, class... Args>
        void info(First first, Args... args);

    private:
        template <class T>
        void addToBuffer(T integer);

        void addToBuffer(char c);
        void addToBuffer(const char* str);
        void addToBuffer(float number);
        void addToBuffer(double number);

        void prepareHeader(LogType logType);
    };

    
    template <uint16_t MaxLogSize>
    template <class T>
    void Logger<MaxLogSize>::log(LogType logType, T item)
    {
        using SimpleDataStructures::ArrayIterator;

        addToBuffer(item);
        if (isOverflowed)
            buffer[BufferSize-1] = bufferOverflowCharacter;
            
        buffer[endIndex] = '\0';

        auto iter = ArrayIterator<Binding>(bindings);
        while (iter.hasNext())
        {
            auto binding = iter.next();
            if (binding.first & logType)
            {
                prepareHeader((LogType)(logType & binding.first));
                binding.second->log(buffer + startIndex);
            }
        }

        endIndex = LogType::Count + 1;
        isOverflowed = false;
    }


    template <uint16_t MaxLogSize>
    template <class First, class... Args>
    void Logger<MaxLogSize>::log(LogType logType, First first, Args... args)
    {
        addToBuffer(first);
        log(logType, args...);
    }


    template <uint16_t MaxLogSize>
    template <class First, class... Args>
    void Logger<MaxLogSize>::error(First first, Args... args)
    {
        log(LogType::Error | LogType::Base, first, args...);
    }


    template <uint16_t MaxLogSize>
    template <class First, class... Args>
    void Logger<MaxLogSize>::debug(First first, Args... args)
    {
        log(LogType::Debug | LogType::Base, first, args...);
    }


    template <uint16_t MaxLogSize>
    template <class First, class... Args>
    void Logger<MaxLogSize>::info(First first, Args... args)
    {
        log(LogType::Info | LogType::Base, first, args...);
    }
}


#endif
