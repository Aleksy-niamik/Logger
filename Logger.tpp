/**
 * @file Logger.tpp
 * @author Aleksy Walczak (aleksyww@gmail.com)
 * @date 2021-04-12
 */

namespace Logging
{
    template <class E, const char (&Headers)[enum_elements_count<E>()], uint16_t MaxLogSize>
    template <class T>
    void Logger<E, Headers, MaxLogSize>::log(LogType logType, T item)
    {        
        addToBuffer(item);
        if (isOverflowed)
            buffer[BufferSize-1] = bufferOverflowCharacter;
            
        buffer[endIndex] = '\0';

        auto iter = bindings.getIterator();
        
        while (iter.hasNext())
        {
            auto binding = iter.next();
            if (binding.first & (uint32_t)logType)
            {
                prepareHeader((uint32_t)logType & binding.first);
                binding.second->log(buffer + startIndex);
            }
        }

        endIndex = EnumCount + 1;
        isOverflowed = false;
    }


    template <class E, const char (&Headers)[enum_elements_count<E>()], uint16_t MaxLogSize>
    template <class First, class... Args>
    void Logger<E, Headers, MaxLogSize>::log(LogType logType, First first, Args... args)
    {
        addToBuffer(first);
        log(logType, args...);
    }


    template <class E, const char (&Headers)[enum_elements_count<E>()], uint16_t MaxLogSize>
    void Logger<E, Headers, MaxLogSize>::bind(LogType logType, ILogMedium* transmitter)
    {
        if (transmitter == nullptr)
            return;

        
        auto iter = bindings.getIterator();
        bool transmitterExists = false;
        while (iter.hasNext())
        {
            
            auto& binding = iter.next();
            if (binding.second == transmitter)
            {
                binding.first = (uint32_t)logType | binding.first;
                transmitterExists = true;
            }
            
        }

        if (!transmitterExists)
            bindings.add(Binding((uint32_t)logType, transmitter));
    }


    template <class E, const char (&Headers)[enum_elements_count<E>()],uint16_t MaxLogSize>
    void Logger<E, Headers, MaxLogSize>::unbind(LogType logType, ILogMedium* transmitter)
    {
        if (transmitter == nullptr)
            return;

        auto iter = bindings.getIterator();
        while (iter.hasNext())
        {
            auto& binding = iter.next();
            if (binding.second == transmitter)
                binding.first = ~((uint32_t)logType) & binding.first;
        }
    }


    template <class E, const char (&Headers)[enum_elements_count<E>()],uint16_t MaxLogSize>
    SimpleDataStructures::GrowingArray<ILogMedium*> Logger<E, Headers, MaxLogSize>::getLogMediums(LogType logType) const
    {
        auto iter = bindings.getIterator();
        uint8_t n = 0;
        while (iter.hasNext())
            if (iter.next().first & (uint32_t)logType) n++;

        auto array = SimpleDataStructures::GrowingArray<ILogMedium*>(n);

        iter.reset(bindings);
        while (iter.hasNext())
        {
            auto binding = iter.next();
            if (binding.first & (uint32_t)logType)
                array.add(binding.second);
        }

        return array;
    }


    template <class E, const char (&Headers)[enum_elements_count<E>()],uint16_t MaxLogSize>
    void Logger<E, Headers, MaxLogSize>::setPrecision(uint8_t precision)
    {
        this->precision = precision;
        this->base = 1;
        for (uint8_t i = 0; i < precision; i++)
            this->base *= 10;
    }


    template <class E, const char (&Headers)[enum_elements_count<E>()],uint16_t MaxLogSize>
    void Logger<E, Headers, MaxLogSize>::setHeaderSeparator(char headerSeparator)
    {
        this->headerSeparator = headerSeparator;
    }


    template <class E, const char (&Headers)[enum_elements_count<E>()],uint16_t MaxLogSize>
    void Logger<E, Headers, MaxLogSize>::setBufferOverflowCharacter(char bufferOverflowCharacter)
    {
        this->bufferOverflowCharacter = bufferOverflowCharacter;
    }


    template <class E, const char (&Headers)[enum_elements_count<E>()],uint16_t MaxLogSize>
    void Logger<E, Headers, MaxLogSize>::setDecimalSeparator(char decimalSeparator)
    {
        this->decimalSeparator = decimalSeparator;
    }


    template <class E, const char (&Headers)[enum_elements_count<E>()],uint16_t MaxLogSize>
    char Logger<E, Headers, MaxLogSize>::getHeaderSeparator() const
    {
        return headerSeparator;
    }


    template <class E, const char (&Headers)[enum_elements_count<E>()],uint16_t MaxLogSize>
    char Logger<E, Headers, MaxLogSize>::getBufferOverflowCharacter() const
    {
        return bufferOverflowCharacter;
    }


    template <class E, const char (&Headers)[enum_elements_count<E>()],uint16_t MaxLogSize>
    char Logger<E, Headers, MaxLogSize>::getDecimalSeparator() const
    {
        return decimalSeparator;
    }


    template <class E, const char (&Headers)[enum_elements_count<E>()],uint16_t MaxLogSize>
    void Logger<E, Headers, MaxLogSize>::addToBuffer(const char* str)
    {
        while(*str != '\0' && endIndex < BufferSize)
            buffer[endIndex++] = *(str++);

        if (*str != '\0')
            isOverflowed = true;
    }

    template <class E, const char (&Headers)[enum_elements_count<E>()],uint16_t MaxLogSize>
    void Logger<E, Headers, MaxLogSize>::addToBuffer(char c)
    {
        if (endIndex < BufferSize)
            buffer[endIndex++] = c;
        else
            isOverflowed = true;
    }


    template <class E, const char (&Headers)[enum_elements_count<E>()],uint16_t MaxLogSize>
    template <class T, bool B>
    void Logger<E, Headers, MaxLogSize>::addToBuffer(T number)
    {
        static_assert(B, "type T has to be an integral type");

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
        
        if (number != 0)
        {
            for (T i = number; i > 0; i = i / 10)
                n++;

            for (int i = 0; i < n; i++)
            {
                if (endIndex + n - 1  - i < BufferSize)
                    buffer[endIndex + n - 1 - i] = (number % 10) + '0';
                number = number / 10;
            }
        }
        else
        {
            n = 1;
            if (endIndex + n - 1 < BufferSize)
                buffer[endIndex + n - 1] = '0';
        }

        if (endIndex + n - 1 < BufferSize)
            endIndex += n;
        else
        {
            endIndex = BufferSize;
            isOverflowed = true;
        }
    }


    template <class E, const char (&Headers)[enum_elements_count<E>()],uint16_t MaxLogSize>
    void Logger<E, Headers, MaxLogSize>::addToBuffer(bool b)
    {
        if (endIndex < BufferSize)
            buffer[endIndex++] = b ? '1' : '0';
        else
            isOverflowed = true;
    }


    template <class E, const char (&Headers)[enum_elements_count<E>()],uint16_t MaxLogSize>
    void Logger<E, Headers, MaxLogSize>::addToBuffer(float number)
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


    template <class E, const char (&Headers)[enum_elements_count<E>()],uint16_t MaxLogSize>
    void Logger<E, Headers, MaxLogSize>::addToBuffer(double number)
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


    template <class E, const char (&Headers)[enum_elements_count<E>()],uint16_t MaxLogSize>
    void Logger<E, Headers, MaxLogSize>::prepareHeader(uint32_t logType)
    {
        buffer[EnumCount] = headerSeparator;
        startIndex = EnumCount;

        uint32_t type = 1;
        for (uint8_t i = 0; i < EnumCount; i++)
        {
            if ((uint32_t)logType & type)
                buffer[--startIndex] = Headers[i];

            type *= 2;
        }
    }
}
