/**
 * ml - simple header-only mathematics library
 * 
 * 2d vector implementation.
 * 
 * \author Felix Lubbe
 * \copyright Copyright (c) 2021
 * \license Distributed under the MIT software license (see accompanying LICENSE.txt).
 */

namespace ml
{

/** 2-dimensional vector */
struct vec2
{
    union
    {
        struct
        {
            float x{0}, y{0};
        };
        struct
        {
            float u, v;
        };
    };

    vec2()
    {
    }

    vec2(const vec2& other)
    : x(other.x)
    , y(other.y)
    {
    }

    vec2(float in_x, float in_y)
    : x(in_x)
    , y(in_y)
    {
    }

    /*
     * Vector operations.
     */
    float dot_product(vec2 other) const
    {
        return x * other.x + y * other.y;
    }

    float length_squared() const
    {
        return dot_product(*this);
    }

    float length() const
    {
#ifdef __GNUC__
        return sqrtf(length_squared());
#else
        return std::sqrtf(length_squared());
#endif
    }

    float area(vec2 other) const
    {
        return x * other.y - y * other.x;
    }

    int area_sign(vec2 other) const
    {
#ifdef ML_NO_BOOST
        const auto a = area(other);
        return (a == 0) ? 0 : ((a > 0) ? 1 : -1);
#else
        return boost::math::sign(area(other));
#endif
    }

    /* operators. */
    const vec2 operator+(vec2 other) const
    {
        return {x + other.x, y + other.y};
    }
    const vec2 operator-(vec2 other) const
    {
        return {x - other.x, y - other.y};
    }
    const vec2 operator-() const
    {
        return {-x, -y};
    }
    const vec2 operator*(float s) const
    {
        return {x * s, y * s};
    }
    const vec2 operator/(float s) const
    {
        return {x / s, y / s};
    }

    /* assignment */
    vec2& operator=(vec2 other)
    {
        x = other.x;
        y = other.y;
        return *this;
    }
    vec2& operator+=(vec2 other)
    {
        *this = *this + other;
        return *this;
    }
    vec2& operator-=(vec2 other)
    {
        *this = *this - other;
        return *this;
    }
    vec2& operator*=(float s)
    {
        *this = *this * s;
        return *this;
    }
    vec2& operator/=(float s)
    {
        *this = *this / s;
        return *this;
    }

    /* exact comparisons */
    bool operator==(vec2 other) const
    {
        return x == other.x && y == other.y;
    }
    bool operator!=(vec2 other) const
    {
        return x != other.x || y != other.y;
    }

    /* access. */
    float& operator[](int i)
    {
        assert(i >= 0 && i < 2);
        return (&x)[i];
    }
    float operator[](int i) const
    {
        assert(i >= 0 && i < 2);
        return (&x)[i];
    }
};

} /* namespace ml */