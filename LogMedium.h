/**
 * @file LogMedium.h
 * @author Jan Wielgus
 * @brief Base class for all logging mediums.
 * @date 2021-08-19
 */

#ifndef __LOGMEDIUM_H__
#define __LOGMEDIUM_H__


namespace Logging
{
    class LogMedium
    {
    public:
        virtual ~LogMedium() {}

        virtual void log(const char* message) = 0;
    };
}


#endif // __LOGMEDIUM_H__
