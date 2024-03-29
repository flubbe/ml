/**
 * ml - simple header-only mathematics library
 * 
 * 4d vector implementation.
 * 
 * \author Felix Lubbe
 * \copyright Copyright (c) 2021
 * \license Distributed under the MIT software license (see accompanying LICENSE.txt).
 */

namespace ml
{

/** 4-dimensional vector */
struct vec4
{
    union
    {
        struct
        {
            float x, y, z, w;
        };
        struct
        {
            float r, g, b, a;
        };
        struct
        {
            float s, t, p, q;
        };
    };
    
    vec4()
    : x{0}
    , y{0}
    , z{0}
    , w{1}
    {
    }

    vec4(const vec3& v)
    : x{v.x}
    , y{v.y}
    , z{v.z}
    , w{1}
    {
    }

    vec4(const vec3& v, float in_w)
    : x{v.x}
    , y{v.y}
    , z{v.z}
    , w{in_w}
    {
    }

    vec4(float in_x, float in_y, float in_z)
    : x{in_x}
    , y{in_y}
    , z{in_z}
    , w{1}
    {
    }

    vec4(float in_x, float in_y, float in_z, float in_w)
    : x{in_x}
    , y{in_y}
    , z{in_z}
    , w{in_w}
    {
    }

    vec4(float v[4])
    {
        x = v[0];
        y = v[1];
        z = v[2];
        w = v[3];
    }

    vec4(const vec4&) = default;
    vec4(vec4&&) = default;

    vec4& operator=(const vec4&) = default;

    void divide_by_w()
    {
        assert(w != 0.f);

        const auto one_over_w = 1.0f / w;
        x *= one_over_w;
        y *= one_over_w;
        z *= one_over_w;
        w = one_over_w;
    }

    bool is_zero() const
    {
        return x == 0 && y == 0 && z == 0 && w == 0;
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

    float dot_product(const vec4& v) const
    {
        return x * v.x + y * v.y + z * v.z + w * v.w;
    }

    vec4 scale(float s) const
    {
        return {x * s, y * s, z * s, w * s};
    }

    void normalize()
    {
        /* one_over_length is safe to call on zero vectors - no check needed. */
        *this = *this * one_over_length();
    }
    vec4 normalized() const
    {
        return *this * one_over_length();
    }

    /* operators. */
    vec4 operator+(const vec4& v) const
    {
        return {x + v.x, y + v.y, z + v.z, w + v.w};
    }
    vec4 operator+(float s) const
    {
        return {x + s, y + s, z + s, w + s};
    }
    vec4 operator-(const vec4& v) const
    {
        return {x - v.x, y - v.y, z - v.z, w - v.w};
    }
    vec4 operator-(float s) const
    {
        return {x - s, y - s, z - s, w - s};
    }
    vec4 operator-() const
    {
        return {-x, -y, -z, -w};
    }
    vec4 operator*(float s) const
    {
        return scale(s);
    }
    vec4 operator*(const vec4& v) const
    {
        return {x * v.x, y * v.y, z * v.z, w * v.w};
    }
    vec4 operator/(float s) const
    {
        return scale(1.0f / s);
    }
    vec4 operator/(const vec4& v) const
    {
        return {x / v.x, y / v.y, z / v.z, w / v.w};
    }

    vec4& operator+=(const vec4& v)
    {
        *this = *this + v;
        return *this;
    }
    vec4& operator-=(const vec4& v)
    {
        *this = *this - v;
        return *this;
    }

    vec4& operator*=(const vec4& v)
    {
        *this = *this * v;
        return *this;
    }

    vec4& operator/=(const vec4& v)
    {
        *this = *this / v;
        return *this;
    }

    vec4& operator*=(float s)
    {
        *this = *this * s;
        return *this;
    }
    vec4& operator/=(float s)
    {
        *this = *this / s;
        return *this;
    }

    /* exact comparisons */
    bool operator==(const vec4& v) const
    {
        return x == v.x && y == v.y && z == v.z && w == v.w;
    }
    bool operator!=(const vec4& v) const
    {
        return x != v.x || y != v.y || z != v.z || w != v.w;
    }

    /* access. */
    float& operator[](int c)
    {
        assert(c >= 0 && c < 4);
        return (&x)[c];
    }
    float operator[](int c) const
    {
        assert(c >= 0 && c < 4);
        return (&x)[c];
    }

#if defined(ML_DEFINE_SWIZZLE_FUNCTIONS)
#    define ML_SWIZZLE_COMPONENTS 4
#    define ML_SWIZZLE_VEC4_TYPE  vec4
#    include "../swizzle.inl"
#    undef ML_SWIZZLE_VEC4_TYPE
#    undef ML_SWIZZLE_COMPONENTS
#else /* defined(ML_DEFINE_SWIZZLE_FUNCTIONS) */
    vec2 xy() const
    {
        return {x, y};
    }

    vec3 xyz() const
    {
        return {x, y, z};
    }
#endif

    /* special vectors. */
    static vec4 zero()
    {
        // note that by default w is initialized to 1, so we initialize the vector explicitely.
        return {0, 0, 0, 0};
    }

    static vec4 one()
    {
        return {1, 1, 1, 1};
    }
};

} /* namespace ml */
