/**
 * @file FlagEnum.h
 * @author Aleksy Walczak (aleksyww@gmail.com)
 * @date 2021-04-09
 */

#ifndef __FLAGENUM_H__
#define __FLAGENUM_H__


constexpr uint32_t nth_power_of_2(uint8_t n)
{
    if (n == 0)
        return 1;
    else
        return 2 * nth_power_of_2(n-1);
}


template <class E>
class FlagEnum
{
    const uint32_t x;
public:
    constexpr FlagEnum(E e)
    : x(nth_power_of_2((uint8_t)e))
    {
    }

    explicit constexpr FlagEnum(uint32_t x_)
    : x(x_)
    {
    }

    explicit constexpr operator uint32_t() const 
    { 
        return x;
    }
};


template <class E>
constexpr FlagEnum<E> operator|(E lhs, E rhs)
{
    return FlagEnum<E>((uint32_t)(FlagEnum<E>(lhs)) | (uint32_t)(FlagEnum<E>(rhs)));
}

template <class E>
constexpr FlagEnum<E> operator|(E lhs, FlagEnum<E> rhs)
{
    return FlagEnum<E>((uint32_t)(FlagEnum<E>(lhs)) | (uint32_t)rhs);
}

template <class E>
constexpr FlagEnum<E> operator|(FlagEnum<E> lhs, E rhs)
{
    return FlagEnum<E>((uint32_t)lhs | (uint32_t)(FlagEnum<E>(rhs)));
}

template <class E>
constexpr FlagEnum<E> operator|(FlagEnum<E> lhs, FlagEnum<E> rhs)
{
    return FlagEnum<E>((uint32_t)lhs | (uint32_t)rhs);
}

#endif // __FLAGENUM_H__
