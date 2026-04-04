/**
 * ml - simple header-only mathematics library
 *
 * functions to support fixed-point arithmetic using the CNL.
 *
 * \author Felix Lubbe
 * \copyright Copyright (c) 2026
 * \license Distributed under the MIT software license (see accompanying LICENSE.txt).
 */

namespace ml
{

/** Obtain the integral part of a fixed-point value, truncated toward zero. */
template<typename Rep, int Exponent>
constexpr auto integral_part(cnl::scaled_integer<Rep, cnl::power<Exponent>> const& value)
  -> std::enable_if_t<std::is_unsigned_v<Rep>,
                      std::decay_t<decltype(cnl::unwrap(value))>>
{
    using raw_type = std::decay_t<decltype(cnl::unwrap(value))>;

    static_assert(Exponent <= 0, "integral_part requires a non-positive exponent");
    static_assert(8 * sizeof(raw_type) > -(Exponent + 1), "integral_part will always return 0");

    return cnl::unwrap(value) >> (-Exponent);
}

template<typename Rep, int Exponent>
constexpr auto integral_part(cnl::scaled_integer<Rep, cnl::power<Exponent>> const& value)
  -> std::enable_if_t<!std::is_unsigned_v<Rep>,
                      std::decay_t<decltype(cnl::unwrap(value))>>
{
    using raw_type = std::decay_t<decltype(cnl::unwrap(value))>;

    static_assert(Exponent <= 0, "integral_part is only meaningful here for fixed-point values");
    static_assert(-Exponent < static_cast<int>(sizeof(raw_type) * 8),
                  "fractional bit count is too large for the underlying representation");

    const raw_type raw = cnl::unwrap(value);
    constexpr raw_type scale = raw_type{1} << (-Exponent);

    return raw / scale;
}

/** helper to round to nearest integer. */
template<typename Rep, int Exponent>
constexpr typename std::enable_if<std::is_unsigned<Rep>::value, Rep>::type
  round(
    cnl::scaled_integer<Rep, cnl::power<Exponent>> const& value)
{
    return integral_part(value + cnl::scaled_integer<Rep, cnl::power<Exponent>>{0.5});
}

template<typename Rep, int Exponent>
constexpr typename std::enable_if<!std::is_unsigned<Rep>::value, Rep>::type
  round(
    cnl::scaled_integer<Rep, cnl::power<Exponent>> const& value)
{
    if(value >= 0)
    {
        return integral_part(value + cnl::scaled_integer<Rep, cnl::power<Exponent>>{0.5});
    }

    return -integral_part(-value + cnl::scaled_integer<Rep, cnl::power<Exponent>>{0.5});
}

/** helper to convert a fixed-point value to float. */
template<typename Rep, int Exponent>
constexpr float to_float(
  cnl::scaled_integer<Rep, cnl::power<Exponent>> const& value)
{
    return static_cast<float>(cnl::unwrap(value)) / cnl::unwrap(cnl::scaled_integer<Rep, cnl::power<Exponent>>{1});
}

/** truncation for non-negative inputs. */
template<int Trunc, typename Rep, int Exp>
constexpr cnl::scaled_integer<Rep, cnl::power<Exp>>
  truncate_unchecked(cnl::scaled_integer<Rep, cnl::power<Exp>> const& value)
{
    static_assert(Trunc >= 0);
    return cnl::wrap<cnl::scaled_integer<Rep, cnl::power<Exp>>>((cnl::unwrap(value) >> Trunc) << Trunc);
}

/** static number trait. */
template<typename T>
struct static_number_traits;

template<
  int Digits, int Exponent,
  typename RoundingTag,
  typename OverflowTag,
  typename Narrowest>
struct static_number_traits<
  cnl::static_number<Digits, Exponent, RoundingTag, OverflowTag, Narrowest>>
{
    using type = cnl::static_number<
      Digits, Exponent, RoundingTag, OverflowTag, Narrowest>;

    static constexpr int digits = Digits;
    static constexpr int exponent = Exponent;
    static constexpr int fractional_bits = -Exponent;

    using narrowest = Narrowest;
    using rounding_tag = RoundingTag;
    using overflow_tag = OverflowTag;
    using rep = decltype(cnl::unwrap(std::declval<type>()));
    using wide_rep = cnl::set_digits_t<rep, 2 * cnl::digits<rep>::value>;
};

} /* namespace ml */
