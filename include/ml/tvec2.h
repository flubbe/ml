/**
 * ml - simple header-only mathematics library
 * 
 * 2d vector template.
 * 
 * \author Felix Lubbe
 * \copyright Copyright (c) 2021
 * \license Distributed under the MIT software license (see accompanying LICENSE.txt).
 */

namespace ml
{

/** 2-dimensional vector */
template<typename T>
struct tvec2
{
    using type = T;

    T x, y;

    tvec2() = default;

    tvec2(const T& in_x, const T& in_y)
    : x(in_x)
    , y(in_y)
    {
    }

    tvec2(const tvec2&) = default;
    tvec2(tvec2&&) = default;

    tvec2& operator=(const tvec2&) = default;

    /* operators. */
    tvec2 operator+(const tvec2& v) const
    {
        return {x + v.x, y + v.y};
    }
    tvec2 operator-(const tvec2& v) const
    {
        return {x - v.x, y - v.y};
    }
    tvec2 operator-() const
    {
        return {-x, -y};
    }
    tvec2 operator*(float s) const
    {
        return {x * s, y * s};
    }
    tvec2 operator/(float s) const
    {
        return {x / s, y / s};
    }

    /* assignment */
    tvec2& operator+=(const tvec2& v)
    {
        *this = *this + v;
        return *this;
    }
    tvec2& operator-=(const tvec2& v)
    {
        *this = *this - v;
        return *this;
    }
    tvec2& operator*=(float s)
    {
        *this = *this * s;
        return *this;
    }
    tvec2& operator/=(float s)
    {
        *this = *this / s;
        return *this;
    }

    /* access. */
    type& operator[](int c)
    {
        assert(c >= 0 && c < 2);
        return (&x)[c];
    }
    type operator[](int c) const
    {
        assert(c >= 0 && c < 2);
        return (&x)[c];
    }
};

/** conversion from vec2. */
template<typename T>
inline tvec2<T> to_tvec2(const vec2& v)
{
    return { static_cast<T>(v.x), static_cast<T>(v.y) };
}

} /* namespace ml */