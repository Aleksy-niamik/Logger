/**
 * @file Logger.cpp
 * @author Aleksy Walczak (aleksyww@gmail.com)
 * @date 2021-04-10
 */

#include "Logger.h"
#include "LogType.h"

using SimpleDataStructures::GrowingArray;
using SimpleDataStructures::ArrayIterator;


template <uint16_t MaxLogSize>
void Logging::Logger<MaxLogSize>::bind(LogType logType, ILogMedium* transmitter)
{
    if (transmitter == nullptr)
        return;

    auto iter = ArrayIterator<Binding>(bindings);
    bool transmitterExists = false;
    while (iter.hasNext())
    {
        auto binding = iter.next();
        if (binding.second == transmitter)
        {
            binding.first = logType | binding.first;
            transmitterExists = true;
        }
    }

    if (!transmitterExists)
        bindings.add(Binding(logType, transmitter));
}


template <uint16_t MaxLogSize>
void Logging::Logger<MaxLogSize>::unbind(LogType logType, ILogMedium* transmitter)
{
    if (transmitter == nullptr)
        return;

    auto iter = ArrayIterator<Binding>(bindings);
    while (iter.hasNext())
    {
        auto binding = iter.next();
        if (binding.second == transmitter)
            binding.first = (LogType)(~logType & binding.first);
    }
}


template <uint16_t MaxLogSize>
GrowingArray<Logging::ILogMedium*> Logging::Logger<MaxLogSize>::getLogMediums(LogType logType)
{
    auto iter = ArrayIterator<Binding>(bindings);
    uint8_t n = 0;
    while (iter.hasNext())
        if (iter.next().first == logType) n++;

    auto array = GrowingArray<ILogMedium*>(n);

    iter = ArrayIterator<Binding>(bindings);
    while (iter.hasNext())
        array.add(iter.next().second);

    return array;
}


template <uint16_t MaxLogSize>
void Logging::Logger<MaxLogSize>::setPrecision(uint8_t precision)
{
    this->precision = precision;
    this->base = 1;
    for (uint8_t i = 0; i < precision; i++)
        this->base *= 10;
}


template <uint16_t MaxLogSize>
void Logging::Logger<MaxLogSize>::setHeaderSeparator(char headerSeparator)
{
    this->headerSeparator = headerSeparator;
}


template <uint16_t MaxLogSize>
void Logging::Logger<MaxLogSize>::setBufferOverflowCharacter(char bufferOverflowCharacter)
{
    this->bufferOverflowCharacter = bufferOverflowCharacter;
}


template <uint16_t MaxLogSize>
void Logging::Logger<MaxLogSize>::setDecimalSeparator(char decimalSeparator)
{
    this->decimalSeparator = decimalSeparator;
}


template <uint16_t MaxLogSize>
char Logging::Logger<MaxLogSize>::getHeaderSeparator()
{
    return headerSeparator;
}


template <uint16_t MaxLogSize>
char Logging::Logger<MaxLogSize>::getBufferOverflowCharacter()
{
    return bufferOverflowCharacter;
}


template <uint16_t MaxLogSize>
char Logging::Logger<MaxLogSize>::getDecimalSeparator()
{
    return decimalSeparator;
}


template <uint16_t MaxLogSize>
void Logging::Logger<MaxLogSize>::addToBuffer(const char* str)
{
    while(*str != '\0' && endIndex < BufferSize)
        buffer[endIndex++] = *(str++);

    if (*str != '\0')
        isOverflowed = true;
}

template <uint16_t MaxLogSize>
void Logging::Logger<MaxLogSize>::addToBuffer(char c)
{
    if (endIndex < BufferSize)
        buffer[endIndex++] = c;
    else
        isOverflowed = true;
}

/**
 * @brief funkcja
 * 
 * @param number 
 */
template <uint16_t MaxLogSize>
template <class T>
void Logging::Logger<MaxLogSize>::addToBuffer(T number)
{
    if (number < 0)
    {
        if (endIndex < BufferSize)
            buffer[endIndex++] = '-';
        else
        {
            isOverflowed = true;
            return;
        }

        number = -number;
    }

    int n = 0;
    
    for (T i = number; i > 0; i = i / 10)
        n++;

    for (int i = 0; i < n; i++)
    {
        if (endIndex + n - 1  - i < BufferSize)
            buffer[endIndex + n - 1 - i] = (number % 10) + '0';
        number = number / 10;
    }

    if (endIndex + n - 1 < BufferSize)
        endIndex += n;
    else
    {
        endIndex = BufferSize;
        isOverflowed = true;
    }
}


template <uint16_t MaxLogSize>
void Logging::Logger<MaxLogSize>::addToBuffer(float number)
{
    if (number < 0)
    {
        addToBuffer('-');
        number = -number;
    }

    uint64_t integer = (uint64_t)(number * base);

    uint64_t floor = integer / base;

    uint64_t mantissa = integer % base;

    addToBuffer(floor);
    addToBuffer(decimalSeparator);
    addToBuffer(mantissa);
}


template <uint16_t MaxLogSize>
void Logging::Logger<MaxLogSize>::addToBuffer(double number)
{
    if (number < 0)
    {
        addToBuffer('-');
        number = -number;
    }

    uint64_t integer = (uint64_t)(number * base);

    uint64_t floor = integer / base;

    uint64_t mantissa = integer % base;

    addToBuffer(floor);
    addToBuffer(decimalSeparator);
    addToBuffer(mantissa);
}


template <uint16_t MaxLogSize>
void Logging::Logger<MaxLogSize>::prepareHeader(LogType logType)
{
    buffer[LogType::Count] = headerSeparator;
    startIndex = LogType::Count;

    LogType type = LogType::Base;
    for (uint8_t i = 0; i < LogType::Count; i++)
    {
        if (logType & type)
            buffer[--startIndex] = Headers[i];

        type = (LogType)(type*2);
    }
}
