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
        __m128 xyzw;
        struct
        {
            float x{0}, y{0}, z{0}, w{1};
        };
        struct {
            float r, g, b, a;
        };
        struct {
            float s, t, p, q;
        };
    };

    vec4()
	{}

    vec4(const __m128 in_xyzw)
    : xyzw(in_xyzw)
    {}

    vec4(const vec3 v)
	{
        //!!fixme: is there a way to just copy v?
        xyzw = _mm_set_ps( 1, v.z, v.y, v.x );
    }

    vec4(const vec3 v, float in_w)
	{
        //!!fixme: is there a way to just copy v?
        xyzw = _mm_set_ps( in_w, v.z, v.y, v.x );
    }

    vec4(float in_x, float in_y, float in_z)
	{
        xyzw = _mm_set_ps( 1, in_z, in_y, in_x );
    }

    vec4(float in_x, float in_y, float in_z, float in_w)
	{
        xyzw = _mm_set_ps( in_w, in_z, in_y, in_x );
    }

    /** divide xyz by w and store 1/w in w */    
    void divide_by_w()
	{
		assert(w != 0.f);

        const auto one_over_w = 1.0f / w;
        xyzw = _mm_mul_ps( xyzw, _mm_set_ps1(one_over_w) );
        w = one_over_w;
	}

    bool is_zero() const
    {
        return _mm_movemask_ps(_mm_cmpeq_ps(xyzw, _mm_set_ps1(0.0f))) != 0;
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
        if( is_zero() )
        {
            return 1.0f;
        }

        return 1.0f / length();
    }

    float dot_product(const vec4 Other) const
    {
        // see answer here: https://stackoverflow.com/questions/6996764/fastest-way-to-do-horizontal-sse-vector-sum-or-other-reduction

#if defined(ML_USE_SSE41)
        return _mm_cvtss_f32( _mm_dp_ps( xyzw, Other.xyzw, 0xff ) );
#elif defined(ML_USE_SSE3)
        // multiply entries
        __m128 r1 = _mm_mul_ps( xyzw, Other.xyzw );

        __m128 shuf = _mm_movehdup_ps(r1);        // broadcast elements 3,1 to 2,0
        __m128 sums = _mm_add_ps(r1, shuf);
        shuf = _mm_movehl_ps(shuf, sums); // high half -> low half
        sums = _mm_add_ss(sums, shuf);
        return _mm_cvtss_f32(sums);
#else /* otherwise use SSE1 */
        // multiply entries
        __m128 r1 = _mm_mul_ps( xyzw, Other.xyzw );

        // horizontal sum.
        __m128 shuf = _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(2, 3, 0, 1));
        __m128 sums = _mm_add_ps(r1, shuf);
        shuf = _mm_movehl_ps(shuf, sums);
        sums = _mm_add_ss(sums, shuf);

        return _mm_cvtss_f32(sums);
#endif
    }

    const vec4 scale(const float& S) const
    {
        return {_mm_mul_ps(xyzw, _mm_set1_ps(S))};
    }

    void normalize()
    {
        /* OneOverLength is safe to call on zero vectors - no check needed. */
        *this = scale(one_over_length());
    }
    const vec4 normalized() const
    {
        return scale(one_over_length());
    }

	/* operations. */
    const vec4 operator+(const vec4 Other) const
	{
        return {_mm_add_ps(xyzw, Other.xyzw)};
	}
    const vec4 operator-(const vec4 Other) const
	{
        return {_mm_sub_ps(xyzw, Other.xyzw)};
	}
    const vec4 operator-() const
	{
        return { _mm_sub_ps(_mm_set1_ps(0.0f), xyzw) };
	}
    const vec4 operator*(const vec4 Other) const
	{
        return { _mm_mul_ps(xyzw, Other.xyzw) };
	}
    const vec4 operator*(float S) const
	{
        return scale(S);
	}
    const vec4 operator/(float S) const
    {
        return scale(1.0f/S);
    }
    const vec4 operator/(const vec4 other) const
    {
        return {_mm_div_ps(xyzw,other.xyzw)};
    }

	vec4& operator+=(const vec4 Other)
	{
		*this = *this + Other;
		return *this;
	}
	vec4& operator-=(const vec4 Other)
	{
		*this = *this - Other;
		return *this;
	}

    vec4& operator*=(const vec4 other)
    {
        *this = *this * other;
        return *this;
    }    

	vec4& operator*=(float S)
	{
		*this = scale(S);
		return *this;
	}
	vec4& operator/=(float S)
	{
		*this = scale(1.0f/S);
		return *this;
	}

	/* exact comparisons */
    bool operator==(const vec4 Other) const
	{
        return _mm_movemask_ps(_mm_cmpeq_ps(xyzw, Other.xyzw)) == 0xF;
	}
    bool operator!=(const vec4 Other) const
	{
        return _mm_movemask_ps(_mm_cmpneq_ps(xyzw, Other.xyzw)) != 0;
	}

	/* access. */
    float& operator[](int c)
    {
        assert(c>=0 && c<4);
        return (&x)[c];
    }
    float operator[](int c) const
    {
        assert(c>=0 && c<4);
        return (&x)[c];
    }

	/* projection onto first components */
    const vec3 xyz() const
	{
        return { x, y, z };
	}
    const vec2 xy() const
	{
        return { x, y };
	}

    const vec3 rgb() const
    {
        return { r, g, b };
    }

    const vec2 st() const
    {
        return { s, t };
    }

	/* special vectors. */
    static const vec4 zero()
	{
        // note that by default w is initialized to 1, so we initialize the vector explicitely.
        return { _mm_set_ps1(0.0) };
	}
};

} /* namespace simd */

} /* namespace ml */