/**
 * ml - simple header-only mathematics library
 *
 * 4d vector implementation using ARM NEON intrinsics (ARMv8).
 *
 * \author Felix Lubbe
 * \copyright Copyright (c) 2026
 * \license Distributed under the MIT software license (see accompanying LICENSE.txt).
 */

#include <arm_neon.h>

namespace ml
{

namespace simd
{

/** 4-dimensional vector */
struct vec4
{
    union
    {
        float32x4_t data;
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
    : data{vdupq_n_f32(0.f)}
    {
    }

    explicit vec4(float32x4_t in_data)
    : data(in_data)
    {
    }

    explicit vec4(const vec3& v)
    {
        float elems[4] = {v.x, v.y, v.z, 1};
        data = vld1q_f32(elems);
    }

    vec4(const vec3& v, float in_w)
    {
        float elems[4] = {v.x, v.y, v.z, in_w};
        data = vld1q_f32(elems);
    }

    vec4(float in_x, float in_y, float in_z)
    {
        float elems[4] = {in_x, in_y, in_z, 1};
        data = vld1q_f32(elems);
    }

    vec4(float in_x, float in_y, float in_z, float in_w)
    {
        float elems[4] = {in_x, in_y, in_z, in_w};
        data = vld1q_f32(elems);
    }

    explicit vec4(const float (&v)[4])
    {
        data = vld1q_f32(v);
    }

    vec4(const vec4&) = default;
    vec4(vec4&&) = default;

    vec4& operator=(const vec4&) = default;
    vec4& operator=(vec4&&) = default;

    /** divide xyz by w and store 1/w in w */
    void divide_by_w()
    {
        assert(w != 0.f);

        const auto one_over_w = 1.0f / w;
        data = vmulq_f32(data, vdupq_n_f32(one_over_w));
        w = one_over_w;
    }

    bool is_zero() const
    {
        return vminvq_u32(vreinterpretq_u32_s32(vceqq_f32(data, vdupq_n_f32(0.0f)))) != 0;
    }

    float length_squared() const
    {
        return dot_product(*this);
    }

    float length() const
    {
        return sqrtf(length_squared());
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
        // NEON horizontal sum of the dot product.
        float32x4_t prod = vmulq_f32(data, v.data);
        float32x2_t lo = vget_low_f32(prod);
        float32x2_t hi = vget_high_f32(prod);
        return vaddvq_f32(prod);
    }

    vec4 hadamard_product(const vec4& v) const
    {
        return vec4{vmulq_f32(data, v.data)};
    }

    vec4 scale(float s) const
    {
        return vec4{vmulq_n_f32(data, s)};
    }

    void normalize()
    {
        /* one_over_length is safe to call on zero vectors - no check needed. */
        *this = scale(one_over_length());
    }

    vec4 normalized() const
    {
        return scale(one_over_length());
    }

    /* operators. */
    vec4 operator+(const vec4& v) const
    {
        return vec4{vaddq_f32(data, v.data)};
    }

    vec4 operator+(float s) const
    {
        return vec4{vaddq_f32(data, vdupq_n_f32(s))};
    }

    vec4 operator-(const vec4& v) const
    {
        return vec4{vsubq_f32(data, v.data)};
    }

    vec4 operator-(float s) const
    {
        return vec4{vsubq_f32(data, vdupq_n_f32(s))};
    }

    vec4 operator-() const
    {
        return vec4{vnegq_f32(data)};
    }

    vec4 operator*(const vec4& v) const
    {
        return hadamard_product(v);
    }

    vec4 operator*(float s) const
    {
        return scale(s);
    }

    vec4 operator/(float s) const
    {
        return scale(1.0f / s);
    }

    vec4 operator/(const vec4& other) const
    {
        return vec4{vdivq_f32(data, other.data)};
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

    vec4& operator*=(float s)
    {
        *this = scale(s);
        return *this;
    }

    vec4& operator/=(float s)
    {
        *this = scale(1.0f / s);
        return *this;
    }

    /* exact comparisons */
    bool operator==(const vec4& v) const
    {
        return vminvq_u32(vreinterpretq_u32_s32(vceqq_f32(data, v.data))) != 0;
    }

    bool operator!=(const vec4& v) const
    {
        return !(*this == v);
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
        return vec4{vdupq_n_f32(0.0f)};
    }

    static vec4 one()
    {
        return vec4{vdupq_n_f32(1.0f)};
    }
};

inline vec4 operator*(float s, const vec4& v)
{
    return v * s;
}

} /* namespace simd */

} /* namespace ml */
