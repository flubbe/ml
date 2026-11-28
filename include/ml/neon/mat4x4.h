/**
 * ml - simple header-only mathematics library
 *
 * 4d matrix implementation using ARM NEON intrinsics (ARMv8).
 *
 * \author Felix Lubbe
 * \copyright Copyright (c) 2026
 * \license Distributed under the MIT software license (see accompanying LICENSE.txt).
 */

#include <optional>

namespace ml
{

namespace simd
{

namespace detail
{

inline void transpose4x4(
  float32x4_t& row0,
  float32x4_t& row1,
  float32x4_t& row2,
  float32x4_t& row3)
{
    const float32x4x2_t t0 = vtrnq_f32(row0, row1);
    const float32x4x2_t t1 = vtrnq_f32(row2, row3);

    row0 = vcombine_f32(vget_low_f32(t0.val[0]), vget_low_f32(t1.val[0]));
    row1 = vcombine_f32(vget_low_f32(t0.val[1]), vget_low_f32(t1.val[1]));
    row2 = vcombine_f32(vget_high_f32(t0.val[0]), vget_high_f32(t1.val[0]));
    row3 = vcombine_f32(vget_high_f32(t0.val[1]), vget_high_f32(t1.val[1]));
}

}    // namespace detail

/* validate vec4 definition. */
static_assert(std::is_same<vec4, ml::simd::vec4>::value,
              "ml::simd::mat4x4 needs vec4 to match ml::simd::vec4");

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
          rows[3] + m.rows[3]};
    }
    mat4x4 operator-(const mat4x4& m) const
    {
        return {
          rows[0] - m.rows[0],
          rows[1] - m.rows[1],
          rows[2] - m.rows[2],
          rows[3] - m.rows[3]};
    }
    mat4x4 operator-() const
    {
        return {-rows[0], -rows[1], -rows[2], -rows[3]};
    }
    mat4x4 operator*(const mat4x4& m) const
    {
        mat4x4 res;
        for(int i = 0; i < 4; ++i)
        {
            res.rows[i].data = matmul_row(rows[i], m);
        }
        return res;
    }

    /* matrix-vector multiplication. */
    vec4 operator*(const vec4& v) const
    {
        vec4 res;
        for(int i = 0; i < 4; ++i)
        {
            res[i] = matmul_row_dot(rows[i], v);
        }
        return res;
    }

    /* scaling */
    mat4x4 operator*(float s) const
    {
        return {rows[0] * s, rows[1] * s, rows[2] * s, rows[3] * s};
    }

    /* assignments */
    mat4x4& operator+=(const mat4x4& m)
    {
        *this = *this + m;
        return *this;
    }

    mat4x4& operator-=(const mat4x4& m)
    {
        *this = *this - m;
        return *this;
    }

    mat4x4& operator*=(const mat4x4& m)
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
        detail::transpose4x4(rows[0].data, rows[1].data, rows[2].data, rows[3].data);
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
          {0.0f, 0.0f, 0.0f, 1.0f}};
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
    /* Multiply a row by matrix m: result = row * m (as SIMD). */
    static float32x4_t matmul_row(vec4 row, const mat4x4& m)
    {
        const float32x4_t mat0 = m.rows[0].data;
        const float32x4_t mat1 = m.rows[1].data;
        const float32x4_t mat2 = m.rows[2].data;
        const float32x4_t mat3 = m.rows[3].data;

        float32x4_t result = vmulq_n_f32(mat0, vgetq_lane_f32(row.data, 0));
        result = vmlaq_n_f32(result, mat1, vgetq_lane_f32(row.data, 1));
        result = vmlaq_n_f32(result, mat2, vgetq_lane_f32(row.data, 2));
        result = vmlaq_n_f32(result, mat3, vgetq_lane_f32(row.data, 3));

        return result;
    }

    /* Compute dot-product of row with a column of the other matrix. */
    static float matmul_row_dot(const vec4& row, const vec4& col)
    {
        const float32x4_t prod = vmulq_f32(row.data, col.data);
        const float32x2_t lo = vget_low_f32(prod);
        const float32x2_t hi = vget_high_f32(prod);
        return vget_lane_f32(vadd_f32(lo, hi), 0);
    }

    /**
     * Calculate the inverse and determinant of a matrix.
     * Based on the Intel paper "Streaming SIMD Extensions - Inverse of 4x4 Matrix".
     * Adapted for ARM NEON.
     */
    bool inverse(
      mat4x4* out,
      float* determinant = nullptr,
      float epsilon = ml::epsilon) const
    {
        float32x4_t row0 = rows[0].data;
        float32x4_t row1 = rows[1].data;
        float32x4_t row2 = rows[2].data;
        float32x4_t row3 = rows[3].data;

        float32x4_t cofactor0, cofactor1, cofactor2, cofactor3;
        float32x4_t det, tmp1;

        // Transpose the input matrix.
        float32x4x2_t t0 = vtrnq_f32(row0, row1);
        float32x4x2_t t1 = vtrnq_f32(row2, row3);

        row0 = vcombine_f32(vget_low_f32(t0.val[0]), vget_low_f32(t1.val[0]));
        row1 = vcombine_f32(vget_low_f32(t0.val[1]), vget_low_f32(t1.val[1]));
        row2 = vcombine_f32(vget_high_f32(t0.val[0]), vget_high_f32(t1.val[0]));
        row3 = vcombine_f32(vget_high_f32(t0.val[1]), vget_high_f32(t1.val[1]));

        // Block 1
        tmp1 = vmulq_f32(row2, row3);
        tmp1 = vrev64q_f32(tmp1);
        cofactor0 = vmulq_f32(row1, tmp1);
        cofactor1 = vmulq_f32(row0, tmp1);
        tmp1 = vextq_f32(tmp1, tmp1, 2);
        cofactor0 = vsubq_f32(vmulq_f32(row1, tmp1), cofactor0);
        cofactor1 = vsubq_f32(vmulq_f32(row0, tmp1), cofactor1);
        cofactor1 = vextq_f32(cofactor1, cofactor1, 2);

        // Block 2
        tmp1 = vmulq_f32(row1, row2);
        tmp1 = vrev64q_f32(tmp1);
        cofactor0 = vaddq_f32(vmulq_f32(row3, tmp1), cofactor0);
        cofactor3 = vmulq_f32(row0, tmp1);
        tmp1 = vextq_f32(tmp1, tmp1, 2);
        cofactor0 = vsubq_f32(cofactor0, vmulq_f32(row3, tmp1));
        cofactor3 = vsubq_f32(vmulq_f32(row0, tmp1), cofactor3);
        cofactor3 = vextq_f32(cofactor3, cofactor3, 2);

        // Block 3
        tmp1 = vmulq_f32(vextq_f32(row1, row1, 2), row3);
        tmp1 = vrev64q_f32(tmp1);
        const float32x4_t row2_shuffled = vextq_f32(row2, row2, 2);
        cofactor0 = vaddq_f32(vmulq_f32(row2_shuffled, tmp1), cofactor0);
        cofactor2 = vmulq_f32(row0, tmp1);
        tmp1 = vextq_f32(tmp1, tmp1, 2);
        cofactor0 = vsubq_f32(cofactor0, vmulq_f32(row2_shuffled, tmp1));
        cofactor2 = vsubq_f32(vmulq_f32(row0, tmp1), cofactor2);
        cofactor2 = vextq_f32(cofactor2, cofactor2, 2);

        // Block 4
        tmp1 = vmulq_f32(row0, row1);
        tmp1 = vrev64q_f32(tmp1);
        cofactor2 = vaddq_f32(vmulq_f32(row3, tmp1), cofactor2);
        cofactor3 = vsubq_f32(vmulq_f32(row2_shuffled, tmp1), cofactor3);
        tmp1 = vextq_f32(tmp1, tmp1, 2);
        cofactor2 = vsubq_f32(vmulq_f32(row3, tmp1), cofactor2);
        cofactor3 = vsubq_f32(cofactor3, vmulq_f32(row2_shuffled, tmp1));

        // Block 5
        tmp1 = vmulq_f32(row0, row3);
        tmp1 = vrev64q_f32(tmp1);
        cofactor1 = vsubq_f32(cofactor1, vmulq_f32(row2_shuffled, tmp1));
        cofactor2 = vaddq_f32(vmulq_f32(row1, tmp1), cofactor2);
        tmp1 = vextq_f32(tmp1, tmp1, 2);
        cofactor1 = vaddq_f32(vmulq_f32(row2_shuffled, tmp1), cofactor1);
        cofactor2 = vsubq_f32(cofactor2, vmulq_f32(row1, tmp1));

        // Block 6
        tmp1 = vmulq_f32(row0, row2_shuffled);
        tmp1 = vrev64q_f32(tmp1);
        cofactor1 = vaddq_f32(vmulq_f32(row3, tmp1), cofactor1);
        cofactor3 = vsubq_f32(cofactor3, vmulq_f32(row1, tmp1));
        tmp1 = vextq_f32(tmp1, tmp1, 2);
        cofactor1 = vsubq_f32(cofactor1, vmulq_f32(row3, tmp1));
        cofactor3 = vaddq_f32(vmulq_f32(row1, tmp1), cofactor3);

        // Determinant calculation
        det = vmulq_f32(row0, cofactor0);
        det = vaddq_f32(vextq_f32(det, det, 2), det);
        det = vaddq_f32(vrev64q_f32(det), det);

        const float d = vgetq_lane_f32(det, 0);

        if(determinant != nullptr)
        {
            // Negated to account for the initial matrix transpose and cofactor signs.
            *determinant = -d;
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

            // Transpose the adjugate.
            t0 = vtrnq_f32(row0, row1);
            t1 = vtrnq_f32(row2, row3);

            row0 = vcombine_f32(vget_low_f32(t0.val[0]), vget_low_f32(t1.val[0]));
            row1 = vcombine_f32(vget_low_f32(t0.val[1]), vget_low_f32(t1.val[1]));
            row2 = vcombine_f32(vget_high_f32(t0.val[0]), vget_high_f32(t1.val[0]));
            row3 = vcombine_f32(vget_high_f32(t0.val[1]), vget_high_f32(t1.val[1]));

            const float inv_det = 1.0f / d;

            out->rows[0].data = vmulq_n_f32(row0, inv_det);
            out->rows[1].data = vmulq_n_f32(row1, inv_det);
            out->rows[2].data = vmulq_n_f32(row2, inv_det);
            out->rows[3].data = vmulq_n_f32(row3, inv_det);
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
