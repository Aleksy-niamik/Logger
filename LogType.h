/**
 * @file LogType.h
 * @author Aleksy Walczak (aleksyww@gmail.com)
 * @date 2021-04-09
 * 
 */

#ifndef LOGTYPE_H
#define LOGTYPE_H


namespace Logging
{
    enum LogType
    {
        Base = 1,
        Info = 2,
        Debug = 4,
        Error = 8,

        Count = 4
    };

    const char Headers[] = {'B', 'I', 'D', 'E'}; 

    static_assert(Logging::LogType::Count == (sizeof(Logging::Headers) / sizeof(Logging::Headers[0])), "Every LogType value must have proper header in Headers table");


    LogType operator|(LogType lhs, LogType rhs)
    {
        return (LogType)((int)lhs | (int)rhs);
    }
}


#endif
