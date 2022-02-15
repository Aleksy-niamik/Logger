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

    /**
     * @brief 
     * 
     * @param Headers 
     * @param length 
     * @return true 
     * @return false 
     */
    constexpr bool checkHeadersPrintable(const char * Headers, size_t length)
    {
        for (size_t i = 0; i < length; ++i)
        {
            if (Headers[i] < ' ' || Headers[i] > '~')
                return false;
        }

        return true;
    }


    /**
     * @brief 
     * 
     * @tparam E 
     * @tparam (&Headers)[enum_elements_count<E>()] 
     * @tparam MaxLogSize 
     */
    template <class E,
        const char (&Headers)[enum_elements_count<E>()],
        uint16_t MaxLogSize = 30>
    class Logger
    {
        static constexpr const int EnumCount = enum_elements_count<E>();

        static_assert(std::is_enum<E>(), "type E must be scoped enumeration type");
        static_assert(is_scoped_enum<E>(), "type E must be scoped enumeration");
        static_assert(EnumCount <= 32, "enum E can't be bigger than 32 elements (sorry)");
        static_assert(is_enum_normalized<E>(), "enum E must be normalized (begins from 0 and has no gaps)");
        static_assert(checkHeadersPrintable(Headers, EnumCount), "each header must be printable character"); 

        
        /**
         * asdfsfg
         * 
         */
        typedef FlagEnum<E> LogType;


        /**
         * @brief Binding type
         * 
         */
        typedef Pair<uint32_t, ILogMedium*> Binding;

        SimpleDataStructures::GrowingArray<Binding> bindings;

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

        /**
         * @brief Get the Header Separator object
         * 
         * @return char 
         */
        char getHeaderSeparator() const;
        char getBufferOverflowCharacter()const;
        char getDecimalSeparator() const;

        template <class T>
        void log(LogType logType, T item);

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
