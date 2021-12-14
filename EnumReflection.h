/**
 * @file EnumReflection.h
 * @author Aleksy Walczak (aleksyww@gmail.com)
 * @date 2021-04-09
 */

#ifndef __ENUM_REFLECTION_H__
#define __ENUM_REFLECTION_H__

#include <stdint.h>
#include <utility>
#include <cstring>


constexpr bool isValidIdentifier(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}


template<class E, E V>
constexpr char elementFirstSign()
{
    auto name = __PRETTY_FUNCTION__;
    int i = strlen(name);
    for (; i >= 0; --i)
        if (name[i] == ' ')
            break;

    return name[i + 1];
}


template <class E, E V>
constexpr bool isValid()
{
    auto name = __PRETTY_FUNCTION__;
    int i = strlen(name);
    for (; i >= 0; --i)
        if (name[i] == ' ')
            break;

    return isValidIdentifier(name[i + 1]);
}


template <class E>
constexpr uint8_t countValid()
{
    return 0;
}


template <class E, E A, E... B>
constexpr uint8_t countValid()
{
    bool _isValid = isValid<E, A>();
    return countValid<E, B...>() + (int)_isValid; 
}


template <class T, T... ints>
constexpr std::integer_sequence<T, 0, 1+ints...> make_bigger_seq(std::integer_sequence<T, ints...> int_seq)
{
  return std::integer_sequence<T, 0, 1+ints...>{};
}


template <class E, uint8_t... ints>
constexpr int internalElementCount(std::integer_sequence<uint8_t, ints...> unused)
{
    return countValid<E, (E)ints...>();
}


template <class E>
struct enum_elements_count : std::integral_constant<
    int, 
    internalElementCount<E>(make_bigger_seq(
        std::make_integer_sequence<uint8_t, 255>()))
> {};


template <typename T, bool B = std::is_enum<T>::value>
struct is_scoped_enum : std::false_type {};


template <typename T>
struct is_scoped_enum<T, true> : std::integral_constant<bool,
    !std::is_convertible<
        T,
        typename std::underlying_type<T>::type
    >::value
> {};


template <class E>
constexpr int countNormalized()
{
    return 0;
}


template <class E, E A, E... B>
constexpr int countNormalized()
{
    if (isValid<E, A>())
        return 1 + countNormalized<E, B...>();
    else return 0;
}


template <class E, uint8_t... ints>
constexpr int internalEnumNormalized(std::integer_sequence<uint8_t, ints...> unused)
{
    return countNormalized<E, (E)ints...>();
}


template <class E>
struct is_enum_normalized : std::integral_constant<
    bool, 
    internalEnumNormalized<E>(make_bigger_seq(
        std::make_integer_sequence<uint8_t, 255>()))
    ==
    enum_elements_count<E>()
> {};


#endif // __ENUM_REFLECTION_H__
