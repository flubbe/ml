/**
 * ml - simple header-only mathematics library
 * 
 * 4d matrix implementation using SSE intrinsics.
 * 
 * \author Felix Lubbe
 * \copyright Copyright (c) 2021
 * \license Distributed under the MIT software license (see accompanying LICENSE.txt).
 */

namespace ml
{

namespace simd
{

/* validate vec4 definition. */
static_assert(std::is_same<vec4,ml::simd::vec4>::value, "ml::simd::mat4x4 needs vec4 to match ml::simd::vec4");

/** 4x4 matrix */
struct mat4x4
{
    vec4 rows[4];

    mat4x4()
	{
		*this = zero();
	}
    mat4x4(vec4 row0, vec4 row1, vec4 row2, vec4 row3)
    : rows { row0, row1, row2, row3 }
	{}

	/* matrix-matrix operations. */
    const mat4x4 operator+(const mat4x4 other) const
	{
        return {
            rows[0] + other.rows[0],
            rows[1] + other.rows[1],
            rows[2] + other.rows[2],
            rows[3] + other.rows[3],
        };
	}
    const mat4x4 operator-(const mat4x4 other) const
	{
        return {
            rows[0] - other.rows[0],
            rows[1] - other.rows[1],
            rows[2] - other.rows[2],
            rows[3] - other.rows[3],
        };
	}
    const mat4x4 operator-() const
	{
        return { -rows[0], -rows[1], -rows[2], -rows[3] };
	}
    const mat4x4 operator*(const mat4x4 m) const
	{
        // reference: https://github.com/microsoft/DirectXMath/blob/master/Inc/DirectXMathMatrix.inl
        mat4x4 res;

        // Use vW to hold the original row
        __m128 vW = rows[0].xyzw;
        __m128 vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
        __m128 vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
        __m128 vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
        vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));        

        // Perform the operation on the first row
        vX = _mm_mul_ps(vX, m.rows[0].xyzw);
        vY = _mm_mul_ps(vY, m.rows[1].xyzw);
        vZ = _mm_mul_ps(vZ, m.rows[2].xyzw);
        vW = _mm_mul_ps(vW, m.rows[3].xyzw);
        // Perform a binary add to reduce cumulative errors
        vX = _mm_add_ps(vX, vZ);
        vY = _mm_add_ps(vY, vW);
        vX = _mm_add_ps(vX, vY);
        res.rows[0].xyzw = vX;

        vW = rows[1].xyzw;
        vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
        vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
        vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
        vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));

        vX = _mm_mul_ps(vX, m.rows[0].xyzw);
        vY = _mm_mul_ps(vY, m.rows[1].xyzw);
        vZ = _mm_mul_ps(vZ, m.rows[2].xyzw);
        vW = _mm_mul_ps(vW, m.rows[3].xyzw);
        vX = _mm_add_ps(vX, vZ);
        vY = _mm_add_ps(vY, vW);
        vX = _mm_add_ps(vX, vY);
        res.rows[1].xyzw = vX;

        vW = rows[2].xyzw;
        vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
        vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
        vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
        vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));

        vX = _mm_mul_ps(vX, m.rows[0].xyzw);
        vY = _mm_mul_ps(vY, m.rows[1].xyzw);
        vZ = _mm_mul_ps(vZ, m.rows[2].xyzw);
        vW = _mm_mul_ps(vW, m.rows[3].xyzw);
        vX = _mm_add_ps(vX, vZ);
        vY = _mm_add_ps(vY, vW);
        vX = _mm_add_ps(vX, vY);
        res.rows[2].xyzw = vX;

        vW = rows[3].xyzw;
        vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
        vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
        vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
        vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));

        vX = _mm_mul_ps(vX, m.rows[0].xyzw);
        vY = _mm_mul_ps(vY, m.rows[1].xyzw);
        vZ = _mm_mul_ps(vZ, m.rows[2].xyzw);
        vW = _mm_mul_ps(vW, m.rows[3].xyzw);
        vX = _mm_add_ps(vX, vZ);
        vY = _mm_add_ps(vY, vW);
        vX = _mm_add_ps(vX, vY);
        res.rows[3].xyzw = vX;

        return res;
	}
    mat4x4& operator*=( const mat4x4 Other )
    {
        *this = *this * Other;
        return *this;
    }

    /* scaling */
    const mat4x4 operator*(const float s) const
    {
        return { rows[0]*s, rows[1]*s, rows[2]*s, rows[3]*s };
    }
    const mat4x4 operator*=(const float s)
    {
        *this = *this * s;
        return *this;
    }

	/* exact comparisons */
    bool operator==(const mat4x4 other) const 
	{
		return 
            rows[0] == other.rows[0] && 
            rows[1] == other.rows[1] && 
            rows[2] == other.rows[2] && 
            rows[3] == other.rows[3];
	}
    bool operator!=(const mat4x4 other) const
	{
		return 
            rows[0] != other.rows[0] || 
            rows[1] != other.rows[1] || 
            rows[2] != other.rows[2] || 
            rows[3] != other.rows[3];
	}

	/* matrix-vector operations. */
    const vec4 operator*(const vec4 v) const
	{
        return { rows[0].dot_product(v), rows[1].dot_product(v), rows[2].dot_product(v), rows[3].dot_product(v) };
	}

    void transpose()
	{
        _MM_TRANSPOSE4_PS(rows[0].xyzw, rows[1].xyzw, rows[2].xyzw, rows[3].xyzw);
	}

    const mat4x4 transposed() const
    {
        mat4x4 m{*this};
        m.transpose();
        return m;
    }

	/* access. */
    vec4& operator[](int c)
	{
        assert(c>=0 && c<4);
        return rows[c];
	}
    vec4 operator[](int c) const
    {
        assert(c>=0 && c<4);
        return rows[c];
    }

	/* special matrices. */
    static mat4x4 identity()
	{
        return {
            { 1.0f, 0.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f },
        };
	}
    static mat4x4 zero()
	{
        return mat4x4( vec4::zero(), vec4::zero(), vec4::zero(), vec4::zero() );
	}
};

} /* namespace simd */

} /* namespace ml */