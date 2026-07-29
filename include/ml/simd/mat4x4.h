/**
 * ml - simple header-only mathematics library
 *
 * 4d matrix implementation using SSE intrinsics.
 *
 * \author Felix Lubbe
 * \copyright Copyright (c) 2021
 * \license Distributed under the MIT software license (see accompanying LICENSE.txt).
 */

#include <optional>

namespace ml
{

namespace simd
{

/* validate vec4 definition. */
static_assert(std::is_same<vec4, ml::simd::vec4>::value, "ml::simd::mat4x4 needs vec4 to match ml::simd::vec4");

/** 4x4 matrix */
struct mat4x4
{
    vec4 rows[4];

    mat4x4()
    {
        *this = zero();
    }
    mat4x4(const vec4& row0, const vec4& row1, const vec4& row2, const vec4& row3)
    : rows{row0, row1, row2, row3}
    {
    }

    mat4x4(const mat4x4&) = default;
    mat4x4(mat4x4&&) = default;

    mat4x4& operator=(const mat4x4&) = default;

    /* matrix-matrix operations. */
    mat4x4 operator+(const mat4x4& m) const
    {
        return {
          rows[0] + m.rows[0],
          rows[1] + m.rows[1],
          rows[2] + m.rows[2],
          rows[3] + m.rows[3],
        };
    }
    mat4x4 operator-(const mat4x4& m) const
    {
        return {
          rows[0] - m.rows[0],
          rows[1] - m.rows[1],
          rows[2] - m.rows[2],
          rows[3] - m.rows[3],
        };
    }
    mat4x4 operator-() const
    {
        return {-rows[0], -rows[1], -rows[2], -rows[3]};
    }
    mat4x4 operator*(const mat4x4& m) const
    {
        // reference: https://github.com/microsoft/DirectXMath/blob/master/Inc/DirectXMathMatrix.inl
        mat4x4 res;

        // Use vW to hold the original row
        __m128 vW = rows[0].data;
        __m128 vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
        __m128 vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
        __m128 vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
        vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));

        // Perform the operation on the first row
        vX = _mm_mul_ps(vX, m.rows[0].data);
        vY = _mm_mul_ps(vY, m.rows[1].data);
        vZ = _mm_mul_ps(vZ, m.rows[2].data);
        vW = _mm_mul_ps(vW, m.rows[3].data);
        // Perform a binary add to reduce cumulative errors
        vX = _mm_add_ps(vX, vZ);
        vY = _mm_add_ps(vY, vW);
        vX = _mm_add_ps(vX, vY);
        res.rows[0].data = vX;

        vW = rows[1].data;
        vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
        vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
        vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
        vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));

        vX = _mm_mul_ps(vX, m.rows[0].data);
        vY = _mm_mul_ps(vY, m.rows[1].data);
        vZ = _mm_mul_ps(vZ, m.rows[2].data);
        vW = _mm_mul_ps(vW, m.rows[3].data);
        vX = _mm_add_ps(vX, vZ);
        vY = _mm_add_ps(vY, vW);
        vX = _mm_add_ps(vX, vY);
        res.rows[1].data = vX;

        vW = rows[2].data;
        vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
        vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
        vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
        vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));

        vX = _mm_mul_ps(vX, m.rows[0].data);
        vY = _mm_mul_ps(vY, m.rows[1].data);
        vZ = _mm_mul_ps(vZ, m.rows[2].data);
        vW = _mm_mul_ps(vW, m.rows[3].data);
        vX = _mm_add_ps(vX, vZ);
        vY = _mm_add_ps(vY, vW);
        vX = _mm_add_ps(vX, vY);
        res.rows[2].data = vX;

        vW = rows[3].data;
        vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
        vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
        vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
        vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));

        vX = _mm_mul_ps(vX, m.rows[0].data);
        vY = _mm_mul_ps(vY, m.rows[1].data);
        vZ = _mm_mul_ps(vZ, m.rows[2].data);
        vW = _mm_mul_ps(vW, m.rows[3].data);
        vX = _mm_add_ps(vX, vZ);
        vY = _mm_add_ps(vY, vW);
        vX = _mm_add_ps(vX, vY);
        res.rows[3].data = vX;

        return res;
    }

    /* matrix-vector multiplication. */
    vec4 operator*(const vec4& v) const
    {
        return {rows[0].dot_product(v), rows[1].dot_product(v), rows[2].dot_product(v), rows[3].dot_product(v)};
    }

    /* scaling */
    mat4x4 operator*(float s) const
    {
        return {rows[0] * s, rows[1] * s, rows[2] * s, rows[3] * s};
    }

    /* assignments */
    mat4x4& operator*=(const mat4x4 m)
    {
        *this = *this * m;
        return *this;
    }

    mat4x4 operator*=(float s)
    {
        *this = *this * s;
        return *this;
    }
    mat4x4 operator/=(float s)
    {
        *this = *this * (1.0f / s);
        return *this;
    }

    /* exact comparisons */
    bool operator==(const mat4x4& m) const
    {
        return rows[0] == m.rows[0] && rows[1] == m.rows[1] && rows[2] == m.rows[2] && rows[3] == m.rows[3];
    }
    bool operator!=(const mat4x4& m) const
    {
        return rows[0] != m.rows[0] || rows[1] != m.rows[1] || rows[2] != m.rows[2] || rows[3] != m.rows[3];
    }

    /* matrix transformations */
    void transpose()
    {
        _MM_TRANSPOSE4_PS(rows[0].data, rows[1].data, rows[2].data, rows[3].data);
    }

    mat4x4 transposed() const
    {
        mat4x4 m{*this};
        m.transpose();
        return m;
    }

    float determinant() const
    {
        float det;
        inverse(nullptr, &det);
        return det;
    }

    bool invert()
    {
        mat4x4 inv;
        float det;

        if(!inverse(&inv, &det))
        {
            return false;
        }

        *this = inv;
        return true;
    }

    std::optional<mat4x4> inverse() const
    {
        mat4x4 result{*this};

        if(!result.invert())
        {
            return std::nullopt;
        }

        return result;
    }

    /* access. */
    vec4& operator[](int c)
    {
        assert(c >= 0 && c < 4);
        return rows[c];
    }
    vec4 operator[](int c) const
    {
        assert(c >= 0 && c < 4);
        return rows[c];
    }

    /* special matrices. */
    static mat4x4 identity()
    {
        return {
          {1.0f, 0.0f, 0.0f, 0.0f},
          {0.0f, 1.0f, 0.0f, 0.0f},
          {0.0f, 0.0f, 1.0f, 0.0f},
          {0.0f, 0.0f, 0.0f, 1.0f},
        };
    }

    static mat4x4 one()
    {
        return mat4x4{vec4::one(), vec4::one(), vec4::one(), vec4::one()};
    }

    static mat4x4 zero()
    {
        return mat4x4{vec4::zero(), vec4::zero(), vec4::zero(), vec4::zero()};
    }

private:
    /**
     * Calculate the inverse and determinant of a matrix.
     * Based on the Intel paper "Streaming SIMD Extensions - Inverse of 4x4 Matrix"
     */
    bool inverse(
      mat4x4* out,
      float* determinant = nullptr,
      float epsilon = ml::epsilon) const
    {
        __m128 row0 = rows[0].data;
        __m128 row1 = rows[1].data;
        __m128 row2 = rows[2].data;
        __m128 row3 = rows[3].data;

        __m128 cofactor0, cofactor1, cofactor2, cofactor3;
        __m128 det, tmp1;

        _MM_TRANSPOSE4_PS(row0, row1, row2, row3);

        // Block 1
        tmp1 = _mm_mul_ps(row2, row3);
        tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
        cofactor0 = _mm_mul_ps(row1, tmp1);
        cofactor1 = _mm_mul_ps(row0, tmp1);
        tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
        cofactor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp1), cofactor0);
        cofactor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), cofactor1);
        cofactor1 = _mm_shuffle_ps(cofactor1, cofactor1, 0x4E);

        // Block 2
        tmp1 = _mm_mul_ps(row1, row2);
        tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
        cofactor0 = _mm_add_ps(_mm_mul_ps(row3, tmp1), cofactor0);
        cofactor3 = _mm_mul_ps(row0, tmp1);
        tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
        cofactor0 = _mm_sub_ps(cofactor0, _mm_mul_ps(row3, tmp1));
        cofactor3 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), cofactor3);
        cofactor3 = _mm_shuffle_ps(cofactor3, cofactor3, 0x4E);

        // Block 3
        tmp1 = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
        tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
        const __m128 row2_shuffled = _mm_shuffle_ps(row2, row2, 0x4E);
        cofactor0 = _mm_add_ps(_mm_mul_ps(row2_shuffled, tmp1), cofactor0);
        cofactor2 = _mm_mul_ps(row0, tmp1);
        tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
        cofactor0 = _mm_sub_ps(cofactor0, _mm_mul_ps(row2_shuffled, tmp1));
        cofactor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), cofactor2);
        cofactor2 = _mm_shuffle_ps(cofactor2, cofactor2, 0x4E);

        // Block 4
        tmp1 = _mm_mul_ps(row0, row1);
        tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
        cofactor2 = _mm_add_ps(_mm_mul_ps(row3, tmp1), cofactor2);
        cofactor3 = _mm_sub_ps(_mm_mul_ps(row2_shuffled, tmp1), cofactor3);
        tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
        cofactor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp1), cofactor2);
        cofactor3 = _mm_sub_ps(cofactor3, _mm_mul_ps(row2_shuffled, tmp1));

        // Block 5
        tmp1 = _mm_mul_ps(row0, row3);
        tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
        cofactor1 = _mm_sub_ps(cofactor1, _mm_mul_ps(row2_shuffled, tmp1));
        cofactor2 = _mm_add_ps(_mm_mul_ps(row1, tmp1), cofactor2);
        tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
        cofactor1 = _mm_add_ps(_mm_mul_ps(row2_shuffled, tmp1), cofactor1);
        cofactor2 = _mm_sub_ps(cofactor2, _mm_mul_ps(row1, tmp1));

        // Block 6
        tmp1 = _mm_mul_ps(row0, row2_shuffled);
        tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
        cofactor1 = _mm_add_ps(_mm_mul_ps(row3, tmp1), cofactor1);
        cofactor3 = _mm_sub_ps(cofactor3, _mm_mul_ps(row1, tmp1));
        tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
        cofactor1 = _mm_sub_ps(cofactor1, _mm_mul_ps(row3, tmp1));
        cofactor3 = _mm_add_ps(_mm_mul_ps(row1, tmp1), cofactor3);

        // Determinant calculation
        det = _mm_mul_ps(row0, cofactor0);
        det = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
        det = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);

        float d = _mm_cvtss_f32(det);
        if(determinant != nullptr)
        {
            *determinant = d;
        }

        const bool invertible = std::abs(d) >= epsilon;
        if(!invertible)
        {
            return false;
        }

        if(out != nullptr)
        {
            row0 = cofactor0;
            row1 = cofactor3;
            row2 = cofactor2;
            row3 = cofactor1;

            _MM_TRANSPOSE4_PS(row0, row1, row2, row3);

            __m128 invDet = _mm_set1_ps(1.0f / d);
            out->rows[0].data = _mm_mul_ps(row0, invDet);
            out->rows[1].data = _mm_mul_ps(row1, invDet);
            out->rows[2].data = _mm_mul_ps(row2, invDet);
            out->rows[3].data = _mm_mul_ps(row3, invDet);
        }

        return true;
    }
};

inline mat4x4 operator*(float s, const mat4x4& m)
{
    return m * s;
}

} /* namespace simd */

} /* namespace ml */
