/**
 * @file ILogMedium.h
 * @author Jan Wielgus
 * @brief Base interface for all logging mediums.
 * @date 2021-08-19
 */

#ifndef __ILOGMEDIUM_H__
#define __ILOGMEDIUM_H__


namespace Logging
{   
    /**
     * @brief Interface for classes which can send some strings (char arrays)
     */
    class ILogMedium
    {
    public:
        virtual ~ILogMedium() {}

        /**
         * @brief send data as C-string (char array ended up with \0)
         * @param message pointer to the char array (ended up with \0) to send
         */
        virtual void log(const char* message) = 0;
    };
}


#endif // __ILOGMEDIUM_H__
