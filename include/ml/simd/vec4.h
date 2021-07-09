/**
 * ml - simple header-only mathematics library
 * 
 * 4d vector implementation using SSE intrinsics.
 * 
 * \author Felix Lubbe
 * \copyright Copyright (c) 2021
 * \license Distributed under the MIT software license (see accompanying LICENSE.txt).
 */

namespace ml
{

namespace simd
{

/** 4-dimensional vector */
struct vec4
{
    union
    {
        __m128 data;
        struct
        {
            float x{0}, y{0}, z{0}, w{1};
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

    vec4() = default;

    vec4(const __m128 in_data)
    : data(in_data)
    {
    }

    vec4(const vec3 v)
    {
        //!!fixme: is there a way to just copy v?
        data = _mm_set_ps(1, v.z, v.y, v.x);
    }

    vec4(const vec3 v, float in_w)
    {
        //!!fixme: is there a way to just copy v?
        data = _mm_set_ps(in_w, v.z, v.y, v.x);
    }

    vec4(float in_x, float in_y, float in_z)
    {
        data = _mm_set_ps(1, in_z, in_y, in_x);
    }

    vec4(float in_x, float in_y, float in_z, float in_w)
    {
        data = _mm_set_ps(in_w, in_z, in_y, in_x);
    }

    vec4(float v[4])
    {
        data = _mm_load_ps(v);
    }

    vec4(const vec4&) = default;
    vec4(vec4&&) = default;

    vec4& operator=(const vec4&) = default;

    /** divide xyz by w and store 1/w in w */
    void divide_by_w()
    {
        assert(w != 0.f);

        const auto one_over_w = 1.0f / w;
        data = _mm_mul_ps(data, _mm_set_ps1(one_over_w));
        w = one_over_w;
    }

    bool is_zero() const
    {
        return _mm_movemask_ps(_mm_cmpeq_ps(data, _mm_set_ps1(0.0f))) == 0xF;
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
        // see answer here: https://stackoverflow.com/questions/6996764/fastest-way-to-do-horizontal-sse-vector-sum-or-other-reduction

#if defined(ML_USE_SSE41)
        return _mm_cvtss_f32(_mm_dp_ps(data, v.data, 0xff));
#elif defined(ML_USE_SSE3)
        // multiply entries
        __m128 r1 = _mm_mul_ps(data, v.data);

        __m128 shuf = _mm_movehdup_ps(r1);    // broadcast elements 3,1 to 2,0
        __m128 sums = _mm_add_ps(r1, shuf);
        shuf = _mm_movehl_ps(shuf, sums);    // high half -> low half
        sums = _mm_add_ss(sums, shuf);
        return _mm_cvtss_f32(sums);
#else /* otherwise use SSE1 */
        // multiply entries
        __m128 r1 = _mm_mul_ps(data, v.data);

        // horizontal sum.
        __m128 shuf = _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(2, 3, 0, 1));
        __m128 sums = _mm_add_ps(r1, shuf);
        shuf = _mm_movehl_ps(shuf, sums);
        sums = _mm_add_ss(sums, shuf);

        return _mm_cvtss_f32(sums);
#endif
    }

    const vec4 scale(float s) const
    {
        return {_mm_mul_ps(data, _mm_set1_ps(s))};
    }

    void normalize()
    {
        /* one_over_length is safe to call on zero vectors - no check needed. */
        *this = scale(one_over_length());
    }
    const vec4 normalized() const
    {
        return scale(one_over_length());
    }

    /* operators. */
    const vec4 operator+(const vec4& v) const
    {
        return {_mm_add_ps(data, v.data)};
    }
    const vec4 operator+(float s) const
    {
        return {_mm_add_ps(data, _mm_set1_ps(s))};
    }
    const vec4 operator-(const vec4& v) const
    {
        return {_mm_sub_ps(data, v.data)};
    }
    const vec4 operator-(float s) const
    {
        return {_mm_sub_ps(data, _mm_set1_ps(s))};
    }
    const vec4 operator-() const
    {
        return {_mm_sub_ps(_mm_set1_ps(0.0f), data)};
    }
    const vec4 operator*(const vec4& v) const
    {
        return {_mm_mul_ps(data, v.data)};
    }
    const vec4 operator*(float s) const
    {
        return scale(s);
    }
    const vec4 operator/(float s) const
    {
        return scale(1.0f / s);
    }
    const vec4 operator/(const vec4 other) const
    {
        return {_mm_div_ps(data, other.data)};
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
        return _mm_movemask_ps(_mm_cmpeq_ps(data, v.data)) == 0xF;
    }
    bool operator!=(const vec4& v) const
    {
        return _mm_movemask_ps(_mm_cmpneq_ps(data, v.data)) != 0;
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
    const vec2 xy() const
    {
        return {x, y};
    }

    const vec3 xyz() const
    {
        return {x, y, z};
    }
#endif

    /*  special vectors. */
    static const vec4 zero()
    {
        // note that by default w is initialized to 1, so we initialize the vector explicitely.
        return {_mm_set_ps1(0.0)};
    }

    static const vec4 one()
    {
        return {_mm_set_ps1(1.0)};
    }
};

} /* namespace simd */

} /* namespace ml */