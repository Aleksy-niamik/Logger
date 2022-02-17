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
    using namespace EnumReflection;
    using namespace SimpleDataStructures;

    /**
     * @brief Logger basic class used for logging messages through multiple mediums
     * 
     * @tparam E Log type enumeration
     * @tparam Headers to table of headers - one header for one log type 
     * @tparam MaxLogSize Max size (in bytes) of a single message
     */
    template <class E,
        const char (&Headers)[enum_elements_count<E>()],
        uint16_t MaxLogSize = 30>
    class Logger
    {
        static constexpr int EnumCount = enum_elements_count<E>();

        static constexpr bool checkHeadersPrintable(const char * headers, size_t length);

        static_assert(std::is_enum<E>(), "type E must be a scoped enumeration type");
        static_assert(is_scoped_enum<E>(), "type E must be a scoped enumeration");
        static_assert(EnumCount <= 32, "enum E can't be bigger than 32 elements (sorry)");
        static_assert(is_enum_normalized<E>(), "enum E must be normalized (begins from 0 and has no gaps)");
        static_assert(checkHeadersPrintable(Headers, EnumCount), "each header must be a printable character"); 

        
        /** 
         * @typedef  LogType is log type enumeration type or binary sum of log type enumerations
         */
        typedef FlagEnum<E> LogType;


        /**
         * @typedef Type of binding between logType ang log medium
         */
        typedef Pair<uint32_t, ILogMedium*> Binding;

        GrowingArray<Binding> bindings;

        static constexpr std::size_t BufferSize = MaxLogSize + EnumCount + 1;
        char buffer[BufferSize + 1];
        std::size_t startIndex;
        std::size_t endIndex = EnumCount + 1;
        bool isOverflowed = false;

        char headerSeparator = '/';
        char bufferOverflowCharacter = '*';
        char decimalSeparator = '.';

        uint8_t precision = 2;
        uint32_t base = 100;
    public:
        Logger() {};
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

        /**
         * @brief Bind transmitter medium with a log type
         * 
         * @param logType Log type or binary sum of log types
         * @param transmitter Pointer to an instance of a class that implements ILogMedium interface
         */
        void bind(LogType logType, ILogMedium* transmitter);

        /**
         * @brief Unbind transmitter medium and a log type
         * 
         * @param logType Log type or binary sum of log types
         * @param transmitter Pointer to an instance of a class that implements ILogMedium interface
         */
        void unbind(LogType logType, ILogMedium* transmitter);

        /**
         * @brief Get the GrowingArray of pointers to ILogMedium instances which are bound to at least one of specified log types
         * 
         * @param logType Log type or binary sum of log types
         */
        GrowingArray<ILogMedium*> getLogMediums(LogType logType) const;

        /**
         * @brief Set the precision of logged floating point numbers
         * 
         * @param precision Number of decimal places (2 by default)
         */
        void setPrecision(uint8_t precision);

        /**
         * @brief Set the Header Separator - sign between headers section and data
         * 
         * @param headerSeparator Sign between headers section and data ('/' by default)
         */
        void setHeaderSeparator(char headerSeparator);

        /**
         * @brief Set the Buffer Overflow Character - sign which is put on the end of logged message when message's length is bigger than MaxLogSize template parameter
         * 
         * @param bufferOverflowCharacter Sign which is put on the end of logged message when message's length is bigger than MaxLogSize template parameter ('*' by default)
         */
        void setBufferOverflowCharacter(char bufferOverflowCharacter);

        /**
         * @brief Set the Decimal Separator - sign between integer part of a number and mantissa
         * 
         * @param decimalSeparator Sign between integer part of a number and mantissa ('.' by default)
         */
        void setDecimalSeparator(char decimalSeparator);

        char getHeaderSeparator() const;
        char getBufferOverflowCharacter()const;
        char getDecimalSeparator() const;

        /**
         * @brief Log some message with specified log type
         * 
         * @tparam T Type of the message (eg. int, const char*)
         * @param logType Log type or binary sum of log types
         * @param item Message to log (eg. 123, "asdf", 'x')
         */
        template <class T>
        void log(LogType logType, T item);

        /**
         * @brief Log multiple messages at once with specified log type
         * 
         * @tparam First Type of first message (eg. int, const char*)
         * @tparam Args Variadic types of next messages
         * @param logType Log type or binary sum of log types
         * @param first Message to log (eg. 123, "asdf", 'x')
         * @param args Other variadic messages
         */
        template <class First, class... Args>
        void log(LogType logType, First first, Args... args);

    private:
        template <class T, bool B = std::is_integral<T>::value>
        void addToBuffer(T integer);

        void addToBuffer(bool b);
        void addToBuffer(char c);
        void addToBuffer(const char* str);
        void addToBuffer(float number);
        void addToBuffer(double number);

        void prepareHeader(uint32_t logType);
    };
}


#include "Logger.tpp"


#endif // __LOGGER_H__
