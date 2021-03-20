/**
 * ml - simple header-only mathematics library
 * 
 * functions to support fixed-point arithmetic using the CNL.
 * 
 * \author Felix Lubbe
 * \copyright Copyright (c) 2021
 * \license Distributed under the MIT software license (see accompanying LICENSE.txt).
 */

namespace ml
{

/** helper to obtain the integral part of a fixed-point value. same as floor. */
template<typename Rep, int Exponent>
typename std::enable_if<std::is_unsigned<Rep>::value, int>::type integral_part(cnl::scaled_integer<Rep, cnl::power<Exponent>> const& value)
{
    static_assert(8 * sizeof(Rep) > -(Exponent + 1), "integral_part will always return 0");
    return cnl::unwrap(value) >> (-Exponent);
}

template<typename Rep, int Exponent>
typename std::enable_if<!std::is_unsigned<Rep>::value, int>::type integral_part(cnl::scaled_integer<Rep, cnl::power<Exponent>> const& value)
{
    static_assert(8 * sizeof(Rep) > -(Exponent + 1), "integral_part will always return 0");

    if(value < 0)
    {
        return -(cnl::unwrap(-value) >> (-Exponent));
    }

    return cnl::unwrap(value) >> (-Exponent);
}

/** helper to round to nearest integer. */
template<typename Rep, int Exponent>
typename std::enable_if<std::is_unsigned<Rep>::value, int>::type round(cnl::scaled_integer<Rep, cnl::power<Exponent>> const& value)
{
    return integral_part(value + cnl::scaled_integer<Rep, cnl::power<Exponent>>{0.5});
}

template<typename Rep, int Exponent>
typename std::enable_if<!std::is_unsigned<Rep>::value, int>::type round(cnl::scaled_integer<Rep, cnl::power<Exponent>> const& value)
{
    if(value >= 0)
    {
        return integral_part(value + cnl::scaled_integer<Rep, cnl::power<Exponent>>{0.5});
    }

    return -integral_part(-value + cnl::scaled_integer<Rep, cnl::power<Exponent>>{0.5});
}

/** helper to convert a fixed-point value to float. */
template<typename Rep, int Exponent>
float to_float(cnl::scaled_integer<Rep, cnl::power<Exponent>> const& value)
{
    return static_cast<float>(cnl::unwrap(value)) / cnl::unwrap(cnl::scaled_integer<Rep, cnl::power<Exponent>>{1});
}

/** truncation for non-negative inputs. */
template<int Trunc, typename Rep, int Exp>
cnl::scaled_integer<Rep, cnl::power<Exp>> truncate_unchecked(cnl::scaled_integer<Rep, cnl::power<Exp>> const& value)
{
    static_assert(Trunc >= 0);
    return cnl::wrap<cnl::scaled_integer<Rep, cnl::power<Exp>>>((cnl::unwrap(value) >> Trunc) << Trunc);
}

} /* namespace ml */
