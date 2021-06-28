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

    tvec2()
    {
    }

    tvec2(const vec2& o)
    : x(o.x)
    , y(o.y)
    {
    }

    tvec2(const T& in_x, const T& in_y)
    : x(in_x)
    , y(in_y)
    {
    }

    tvec2(const tvec2&) = default;

    /* operators. */
    const tvec2 operator+(const tvec2& o) const
    {
        return {x + o.x, y + o.y};
    }
    const tvec2 operator-(const tvec2& o) const
    {
        return {x - o.x, y - o.y};
    }
    const tvec2 operator-() const
    {
        return {-x, -y};
    }
    const tvec2 operator*(float S) const
    {
        return {x * S, y * S};
    }
    const tvec2 operator/(float S) const
    {
        return {x / S, y / S};
    }

    /* assignment */
    tvec2& operator=(const tvec2& o)
    {
        x = o.x;
        y = o.y;
        return *this;
    }
    tvec2& operator+=(const tvec2& o)
    {
        *this = *this + o;
        return *this;
    }
    tvec2& operator-=(const tvec2& o)
    {
        *this = *this - o;
        return *this;
    }
    tvec2& operator*=(float S)
    {
        *this = *this * S;
        return *this;
    }
    tvec2& operator/=(float S)
    {
        *this = *this / S;
        return *this;
    }

    /* access. */
    type& operator[](int c)
    {
        assert(c >= 0 && c < 2);
        return (&x)[c];
    }
    const type operator[](int c) const
    {
        assert(c >= 0 && c < 2);
        return (&x)[c];
    }
};

} /* namespace ml */