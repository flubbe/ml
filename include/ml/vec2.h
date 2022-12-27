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
            float x, y;
        };
        struct
        {
            float u, v;
        };
    };

    vec2()
    : x{0}
    , y{0}
    {
    }

    vec2(float in_x, float in_y)
    : x{in_x}
    , y{in_y}
    {
    }

    vec2(const vec2&) = default;
    vec2(vec2&&) = default;

    vec2& operator=(const vec2&) = default;

    bool is_zero() const
    {
        return x == 0 && y == 0;
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

    float one_over_length() const
    {
        if(is_zero())
        {
            return 1.0f;
        }

        return 1.0f / length();
    }

    float dot_product(const vec2& v) const
    {
        return x * v.x + y * v.y;
    }

    vec2 scale(float s) const
    {
        return {x * s, y * s};
    }

    void normalize()
    {
        /* one_over_length is safe to call on zero vectors - no check needed. */
        *this = *this * one_over_length();
    }
    vec2 normalized() const
    {
        return *this * one_over_length();
    }

    float area(const vec2& v) const
    {
        return x * v.y - y * v.x;
    }

    int area_sign(const vec2& v) const
    {
#ifdef ML_NO_BOOST
        const auto a = area(other);
        return (a == 0) ? 0 : ((a > 0) ? 1 : -1);
#else
        return boost::math::sign(area(v));
#endif
    }

    /* operators. */
    vec2 operator+(const vec2& v) const
    {
        return {x + v.x, y + v.y};
    }
    vec2 operator+(float s) const
    {
        return {x + s, y + s};
    }
    vec2 operator-(vec2 other) const
    {
        return {x - other.x, y - other.y};
    }
    vec2 operator-(float s) const
    {
        return {x - s, y - s};
    }
    vec2 operator-() const
    {
        return {-x, -y};
    }
    vec2 operator*(float s) const
    {
        return scale(s);
    }
    vec2 operator*(const vec2& v) const
    {
        return {x * v.x, y * v.y};
    }
    vec2 operator/(float s) const
    {
        return scale(1.0f / s);
    }
    vec2 operator/(const vec2& v) const
    {
        return {x / v.x, y / v.y};
    }

    vec2& operator+=(const vec2& v)
    {
        *this = *this + v;
        return *this;
    }
    vec2& operator-=(const vec2& v)
    {
        *this = *this - v;
        return *this;
    }

    vec2& operator*=(const vec2& v)
    {
        *this = *this * v;
        return *this;
    }
    vec2& operator/=(const vec2& v)
    {
        *this = *this / v;
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

#if defined(ML_DEFINE_SWIZZLE_FUNCTIONS)
#    define ML_SWIZZLE_COMPONENTS 2
#    define ML_SWIZZLE_VEC4_TYPE  vec4
#    include "swizzle.inl"
#    undef ML_SWIZZLE_VEC4_TYPE
#    undef ML_SWIZZLE_COMPONENTS
#endif /* defined(ML_DEFINE_SWIZZLE_FUNCTIONS) */

    /* special vectors. */
    static vec2 zero()
    {
        return {0, 0};
    }

    static vec2 one()
    {
        return {1, 1};
    }
};

} /* namespace ml */
