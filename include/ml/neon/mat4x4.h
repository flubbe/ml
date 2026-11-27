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
        auto multiply_row = [](float32x4_t row,
                               const mat4x4& m) -> float32x4_t
        {
            const float32x4_t x = vmulq_n_f32(m.rows[0].data, vgetq_lane_f32(row, 0));
            const float32x4_t y = vmulq_n_f32(m.rows[1].data, vgetq_lane_f32(row, 1));
            const float32x4_t z = vmulq_n_f32(m.rows[2].data, vgetq_lane_f32(row, 2));
            const float32x4_t w = vmulq_n_f32(m.rows[3].data, vgetq_lane_f32(row, 3));

            return vaddq_f32(
              vaddq_f32(x, y),
              vaddq_f32(z, w));
        };

        mat4x4 res;
        for(std::size_t i = 0; i < 4; ++i)
        {
            res.rows[i].data = multiply_row(rows[i].data, m);
        }

        return res;
    }

    /* matrix-vector multiplication. */
    vec4 operator*(const vec4& v) const
    {
        return {rows[0].dot_product(v), rows[1].dot_product(v),
                rows[2].dot_product(v), rows[3].dot_product(v)};
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
    /**
     * Calculate the inverse and determinant of a matrix.
     * Uses Gauss-Jordan elimination with partial pivoting.
     */
    bool inverse(
      mat4x4* out,
      float* determinant = nullptr,
      float epsilon = ml::epsilon) const
    {
        // [ A | I ]
        float augmented[4][8]{};

        for(std::size_t i = 0; i < 4; ++i)
        {
            vst1q_f32(augmented[i], rows[i].data);
            augmented[i][4 + i] = 1.0f;
        }

        float det = 1.0f;

        for(std::size_t col = 0; col < 4; ++col)
        {
            // Find the largest pivot in this column.
            std::size_t pivot_row = col;
            float pivot_abs = std::abs(augmented[col][col]);

            for(std::size_t row = col + 1; row < 4; ++row)
            {
                const float candidate = std::abs(augmented[row][col]);
                if(candidate > pivot_abs)
                {
                    pivot_abs = candidate;
                    pivot_row = row;
                }
            }

            // A zero pivot means the matrix is singular.
            if(pivot_abs == 0.0f)
            {
                if(determinant)
                {
                    *determinant = 0.0f;
                }
                return false;
            }

            // Swap rows if necessary. A row swap changes the determinant sign.
            if(pivot_row != col)
            {
                for(std::size_t j = 0; j < 8; ++j)
                {
                    std::swap(augmented[col][j], augmented[pivot_row][j]);
                }

                det = -det;
            }

            const float pivot = augmented[col][col];
            det *= pivot;

            const float inv_pivot = 1.0f / pivot;

            // Normalize the pivot row.
            float32x4_t lhs = vmulq_n_f32(
              vld1q_f32(augmented[col]),
              inv_pivot);

            float32x4_t rhs = vmulq_n_f32(
              vld1q_f32(augmented[col] + 4),
              inv_pivot);

            vst1q_f32(augmented[col], lhs);
            vst1q_f32(augmented[col] + 4, rhs);

            // Eliminate this column from all other rows.
            for(std::size_t row = 0; row < 4; ++row)
            {
                if(row == col)
                {
                    continue;
                }

                const float factor = augmented[row][col];

                lhs = vld1q_f32(augmented[row]);
                rhs = vld1q_f32(augmented[row] + 4);

                lhs = vsubq_f32(
                  lhs,
                  vmulq_n_f32(vld1q_f32(augmented[col]), factor));

                rhs = vsubq_f32(
                  rhs,
                  vmulq_n_f32(vld1q_f32(augmented[col] + 4), factor));

                vst1q_f32(augmented[row], lhs);
                vst1q_f32(augmented[row] + 4, rhs);
            }
        }

        if(determinant)
        {
            *determinant = det;
        }

        // Apply the caller's singularity threshold to the determinant.
        if(std::abs(det) <= epsilon)
        {
            return false;
        }

        if(out)
        {
            for(std::size_t i = 0; i < 4; ++i)
            {
                out->rows[i].data = vld1q_f32(augmented[i] + 4);
            }
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
