/**
 * ml - simple header-only mathematics library
 * 
 * general mathematical functions.
 * 
 * \author Felix Lubbe
 * \copyright Copyright (c) 2021
 * \license Distributed under the MIT software license (see accompanying LICENSE.txt).
 */

namespace ml
{

/** generic linear interpolation */
template<typename T>
const T lerp( float t, const T& a, const T& b )
{
    return a*(1.0f-t) + b*t;
}

/** specialization */
inline float lerp( float t, float a, float b )
{
    return std::fmaf(t, b, std::fmaf(-t, a, a));
}

/** specialization */
inline double lerp( float t, double a, double b )
{
    return std::fma( static_cast<double>(t), b, std::fma(-t, a, a));
}

/** Truncate a non-negative value. The function does assume a>=0, but does not check it. */
inline int truncate_unchecked(float a)
{
    return static_cast<int>(a);
}

/** dot product between two vectors. */
template<typename T>
float dot( const T a, const T b )
{
    return a.dot_product(b);
}

} /* namespace ml */