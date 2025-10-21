/**
 * ml - simple header-only mathematics library
 * 
 * provides closed_unit_interval<T>, a fixed-point representation of [0,1].
 * 
 * \author Felix Lubbe
 * \copyright Copyright (c) 2021
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
struct closed_unit_interval
{
    /* only supports POD types */
    static_assert(std::is_standard_layout<T>::value && std::is_trivial<T>::value, "closed_unit_interval<T> only supports POD types");

    /** Representation for one. */
    static const T one{std::numeric_limits<T>::max()};
    static const T half{std::numeric_limits<T>::max() / 2};

    /** Fixed-point representation of data. */
    T data{0};

    /** Default constructor. */
    closed_unit_interval() = default;

    /** default copy constructor. */
    closed_unit_interval(const closed_unit_interval&) = default;

    /** Constructor. Clamps incoming numbers to the range [0,1]. */
    closed_unit_interval(float in)
    {
        // clamp incoming value.
#ifdef ML_NO_BOOST
        in = std::min(1.f, std::max(0.f, in));
#else
        in = boost::algorithm::clamp(in, 0.0f, 1.0f);
#endif

        // circumvent precision issue when near one.
        if(in < 0.5f)
        {
            // convert to fixed-point.
            data = static_cast<T>(in * one);
        }
        else
        {
            in -= 0.5f;
            data = static_cast<T>(in * one) + half;
        }
    }

    struct no_clamp
    {
    };

    /** Non-clamping constructor. Assumes incoming numbers to be in the range [0,1]. */
    closed_unit_interval(float in, const no_clamp&)
    {
        // circumvent precision issue when near one.
        if(in <= 0.5f)
        {
            // convert to fixed-point.
            data = static_cast<T>(in * one);
        }
        else
        {
            in -= 0.5f;
            data = static_cast<T>(in * one) + half;
        }
    }

    struct no_scale
    {
    };

    /** Construct from base representation. */
    closed_unit_interval(T n, const no_scale&)
    : data(n)
    {
    }

    /*
     * Comparison operators.
     */
    bool operator==(const closed_unit_interval& Other) const
    {
        return data == Other.data;
    }
    bool operator!=(const closed_unit_interval& Other) const
    {
        return data != Other.data;
    }

    bool operator<(const closed_unit_interval& Other) const
    {
        return data < Other.data;
    }
    bool operator<=(const closed_unit_interval& Other) const
    {
        return data <= Other.data;
    }

    bool operator>(const closed_unit_interval& Other) const
    {
        return data > Other.data;
    }
    bool operator>=(const closed_unit_interval& Other) const
    {
        return data >= Other.data;
    }

    /*
     * Arithmetic.
     */
    closed_unit_interval operator+(const closed_unit_interval& Other) const
    {
        /*
         * no overflow check. if desired, add:
         *
         * assert( data < std::numeric_limits<T>::max() - Other.data )
         */
        return closed_unit_interval(data + Other.data, no_scale());
    }
    closed_unit_interval operator-(const closed_unit_interval& Other) const
    {
        /*
         * no underflow check. if desired, add:
         *
         * assert( data > Other.data )
         */
        return closed_unit_interval(data - Other.data, no_scale());
    }

    /*
     * Assignment operators.
     */
    closed_unit_interval& operator=(const closed_unit_interval& Other)
    {
        data = Other.data;
        return *this;
    }
    closed_unit_interval& operator+=(const closed_unit_interval& Other)
    {
        *this = *this + Other;
        return *this;
    }
    closed_unit_interval& operator-=(const closed_unit_interval& Other)
    {
        *this = *this - Other;
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