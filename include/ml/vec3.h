/**
 * ml - simple header-only mathematics library
 * 
 * 3d vector implementation.
 * 
 * \author Felix Lubbe
 * \copyright Copyright (c) 2021
 * \license Distributed under the MIT software license (see accompanying LICENSE.txt).
 */

namespace ml
{

/** 3-dimensional vector */
struct vec3
{
    union
    {
        struct
        {
            float x, y, z;
        };
        struct
        {
            float u, v, w;
        };
    };

    vec3()
    : x{0}
    , y{0}
    , z{0}
    {
    }

    vec3(float in_x, float in_y, float in_z)
    : x{in_x}
    , y{in_y}
    , z{in_z}
    {
    }

    vec3(const vec3&) = default;
    vec3(vec3&&) = default;

    vec3& operator=(const vec3&) = default;

    bool is_zero() const
    {
        return x == 0.f && y == 0.f && z == 0.f;
    }

    float length_squared() const
    {
        return dot_product(*this);
    }

    float length() const
    {
        // for 3-component vectors: https://fastcpp.blogspot.com/2012/02/calculating-length-of-3d-vector-using.html
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

    float dot_product(const vec3& v) const
    {
        return x * v.x + y * v.y + z * v.z;
    }
    vec3 cross_product(const vec3& v) const
    {
        return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
    }

    vec3 scale(float s) const
    {
        return {x * s, y * s, z * s};
    }

    void normalize()
    {
        /* one_over_length is safe to call on zero vectors - no check needed. */
        *this = scale(one_over_length());
    }
    vec3 normalized() const
    {
        return scale(one_over_length());
    }

    /* operators. */
    vec3 operator+(const vec3& v) const
    {
        return {x + v.x, y + v.y, z + v.z};
    }
    vec3 operator+(float s) const
    {
        return {x + s, y + s, z + s};
    }
    vec3 operator-(const vec3& v) const
    {
        return {x - v.x, y - v.y, z - v.z};
    }
    vec3 operator-(float s) const
    {
        return {x - s, y - s, z - s};
    }
    vec3 operator-() const
    {
        return {-x, -y, -z};
    }
    vec3 operator*(float s) const
    {
        return scale(s);
    }
    vec3 operator*(const vec3& v) const
    {
        return {x * v.x, y * v.y, z * v.z};
    }
    vec3 operator/(float s) const
    {
        return scale(1.0f / s);
    }
    vec3 operator/(const vec3& v) const
    {
        return {x / v.x, y / v.y, z / v.z};
    }
    vec3 operator^(const vec3& v) const
    {
        return cross_product(v);
    }

    vec3& operator+=(const vec3& v)
    {
        *this = *this + v;
        return *this;
    }
    vec3& operator-=(const vec3& v)
    {
        *this = *this - v;
        return *this;
    }

    vec3& operator*=(const vec3& v)
    {
        *this = *this * v;
        return *this;
    }
    vec3& operator/=(const vec3& v)
    {
        *this = *this / v;
        return *this;
    }

    vec3& operator*=(float s)
    {
        *this = *this * s;
        return *this;
    }
    vec3& operator/=(float s)
    {
        *this = *this / s;
        return *this;
    }

    /* exact comparisons */
    bool operator==(const vec3& v) const
    {
        return x == v.x && y == v.y && z == v.z;
    }
    bool operator!=(const vec3& v) const
    {
        return x != v.x || y != v.y || z != v.z;
    }

    /* access. */
    float& operator[](int c)
    {
        assert(c >= 0 && c < 3);
        return (&x)[c];
    }
    float operator[](int c) const
    {
        assert(c >= 0 && c < 3);
        return (&x)[c];
    }

#if defined(ML_DEFINE_SWIZZLE_FUNCTIONS)
#    define ML_SWIZZLE_COMPONENTS 3
#    define ML_SWIZZLE_VEC4_TYPE  vec4
#    include "swizzle.inl"
#    undef ML_SWIZZLE_VEC4_TYPE
#    undef ML_SWIZZLE_COMPONENTS
#else  /* defined(ML_DEFINE_SWIZZLE_FUNCTIONS) */
    /* projection onto first components */
    vec2 xy() const
    {
        return {x, y};
    }
#endif /* defined(ML_DEFINE_SWIZZLE_FUNCTIONS) */

    /* special vectors. */
    static vec3 zero()
    {
        // vec3 is initialized to zero by default.
        return {};
    }

    static vec3 one()
    {
        return {1, 1, 1};
    }
};

} /* namespace ml */
