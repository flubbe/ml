/**
 * ml - simple header-only mathematics library
 *
 * provides closed_unit_interval<T>, a fixed-point representation of [0,1].
 *
 * \author Felix Lubbe
 * \copyright Copyright (c) 2026
 * \license Distributed under the MIT software license (see accompanying LICENSE.txt).
 */

namespace ml
{

/**
 * fixed-point type representing the interval [0,1].
 *
 * Note that this does not follow from cnl due to the choice of unit.
 */
template<typename T>
    requires(
      std::is_standard_layout_v<T>
      && std::is_trivial_v<T>
      && std::is_unsigned_v<T>
      && std::numeric_limits<T>::digits <= 32)
struct closed_unit_interval
{
    /** Representation for one. */
    static constexpr int bits = std::numeric_limits<T>::digits;
    static constexpr float scale = static_cast<float>(uint64_t(1) << bits);
    static constexpr T one = static_cast<T>((uint64_t(1) << bits) - 1);
    static constexpr T half = static_cast<T>(uint64_t(1) << (bits - 1));

    /** Fixed-point representation of data. */
    T data{0};

    /** Default constructor. */
    closed_unit_interval() noexcept = default;

    /** default copy constructor. */
    closed_unit_interval(const closed_unit_interval&) noexcept = default;

    /** Constructor. Clamps incoming numbers to the range [0,1]. */
    closed_unit_interval(float in) noexcept
    {
        // clamp and circumvent precision issue when near one.
        if(in <= 0.f)
        {
            data = 0;
        }
        else if(in >= 1.f)
        {
            data = one;
        }
        else if(in <= 0.5f)
        {
            // convert to fixed-point.
            data = static_cast<T>(in * scale);
        }
        else
        {
            in -= 0.5f;
            data = static_cast<T>(in * scale) + half;
        }
    }

    struct no_clamp
    {
    };

    /** Non-clamping constructor. Assumes incoming numbers to be in the range [0,1]. */
    closed_unit_interval(float in, const no_clamp&) noexcept
    {
        // circumvent precision issue when near one.
        if(in <= 0.5f)
        {
            // convert to fixed-point.
            data = static_cast<T>(in * scale);
        }
        else
        {
            in -= 0.5f;
            data = static_cast<T>(in * scale) + half;
        }
    }

    struct no_scale
    {
    };

    /** Construct from base representation. */
    closed_unit_interval(T n, const no_scale&) noexcept
    : data(n)
    {
    }

    /*
     * Comparison operators.
     */
    constexpr auto operator<=>(const closed_unit_interval&) const noexcept = default;

    /*
     * Arithmetic.
     */
    constexpr closed_unit_interval operator+(const closed_unit_interval& Other) const noexcept
    {
        /*
         * no overflow check. if desired, add:
         *
         * assert( data < std::numeric_limits<T>::max() - Other.data )
         */
        return closed_unit_interval{data + Other.data, no_scale()};
    }
    constexpr closed_unit_interval operator-(const closed_unit_interval& Other) const noexcept
    {
        /*
         * no underflow check. if desired, add:
         *
         * assert( data > Other.data )
         */
        return closed_unit_interval{data - Other.data, no_scale()};
    }

    /*
     * Assignment operators.
     */
    constexpr closed_unit_interval& operator=(const closed_unit_interval& Other) noexcept = default;
    constexpr closed_unit_interval& operator+=(const closed_unit_interval& Other) noexcept
    {
        data += Other.data;
        return *this;
    }
    constexpr closed_unit_interval& operator-=(const closed_unit_interval& Other) noexcept
    {
        data -= Other.data;
        return *this;
    }
};

template<typename T>
T unwrap(const closed_unit_interval<T>& i)
{
    return i.data;
}

template<typename T>
closed_unit_interval<T> wrap(const T& data)
{
    return closed_unit_interval<T>(data, typename closed_unit_interval<T>::no_scale());
}

/**
 * helper to convert a fixed-point value to float.
 * This should be used rarely since it may not return precise values.
 */
template<typename T>
float to_float(closed_unit_interval<T> const& value)
{
    return static_cast<float>(value.data) / closed_unit_interval<T>::one;
}

} /* namespace ml */