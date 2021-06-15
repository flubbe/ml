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
            float x{0}, y{0}, z{0};
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
    : x(in_x)
    , y(in_y)
    , z(in_z)
    {
    }

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

    float dot_product(vec3 Other) const
    {
        return x * Other.x + y * Other.y + z * Other.z;
    }
    const vec3 cross_product(vec3 Other) const
    {
        return {y * Other.z - z * Other.y, z * Other.x - x * Other.z, x * Other.y - y * Other.x};
    }

    const vec3 scale(float S) const
    {
        return {x * S, y * S, z * S};
    }

    void normalize()
    {
        /* one_over_length is safe to call on zero vectors - no check needed. */
        *this = scale(one_over_length());
    }
    const vec3 normalized() const
    {
        return scale(one_over_length());
    }

    /* operators. */
    const vec3 operator+(vec3 Other) const
    {
        return {x + Other.x, y + Other.y, z + Other.z};
    }
    const vec3 operator+(float f) const
    {
        return {x + f, y + f, z + f};
    }
    const vec3 operator-(vec3 Other) const
    {
        return {x - Other.x, y - Other.y, z - Other.z};
    }
    const vec3 operator-(float f) const
    {
        return {x - f, y - f, z - f};
    }
    const vec3 operator-() const
    {
        return {-x, -y, -z};
    }
    const vec3 operator*(float S) const
    {
        return scale(S);
    }
    const vec3 operator/(float S) const
    {
        return scale(1.0f / S);
    }
    float operator*(vec3 Other) const
    {
        return dot_product(Other);
    }
    const vec3 operator^(vec3 Other) const
    {
        return cross_product(Other);
    }

    vec3& operator+=(const vec3 other)
    {
        *this = *this + other;
        return *this;
    }
    vec3& operator-=(const vec3 other)
    {
        *this = *this - other;
        return *this;
    }

    /* exact comparisons */
    bool operator==(vec3 Other) const
    {
        return x == Other.x && y == Other.y && z == Other.z;
    }
    bool operator!=(vec3 Other) const
    {
        return x != Other.x || y != Other.y || z != Other.z;
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
    const vec2 xy() const
    {
        return {x, y};
    }
#endif /* defined(ML_DEFINE_SWIZZLE_FUNCTIONS) */

    /* special vectors. */
    static const vec3 zero()
    {
        // vec3 is initialized to zero by default.
        return {};
    }

    static const vec3 one()
    {
        return {1, 1, 1};
    }
};

} /* namespace ml */