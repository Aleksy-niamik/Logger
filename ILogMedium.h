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
    class ILogMedium
    {
    public:
        virtual ~ILogMedium() {}

        virtual void log(const char* message) = 0;
    };
}


#endif // __ILOGMEDIUM_H__
