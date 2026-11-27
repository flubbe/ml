/**
 * ml - simple header-only mathematics library
 *
 * 4d matrix implementation.
 *
 * \author Felix Lubbe
 * \copyright Copyright (c) 2021
 * \license Distributed under the MIT software license (see accompanying LICENSE.txt).
 */

namespace ml
{

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
        vec4 v{m.rows[0].x, m.rows[1].x, m.rows[2].x, m.rows[3].x};
        vec4 col1{
          rows[0].dot_product(v),
          rows[1].dot_product(v),
          rows[2].dot_product(v),
          rows[3].dot_product(v)};

        v = {m.rows[0].y, m.rows[1].y, m.rows[2].y, m.rows[3].y};
        vec4 col2{
          rows[0].dot_product(v),
          rows[1].dot_product(v),
          rows[2].dot_product(v),
          rows[3].dot_product(v)};

        v = {m.rows[0].z, m.rows[1].z, m.rows[2].z, m.rows[3].z};
        vec4 col3{
          rows[0].dot_product(v),
          rows[1].dot_product(v),
          rows[2].dot_product(v),
          rows[3].dot_product(v)};

        v = {m.rows[0].w, m.rows[1].w, m.rows[2].w, m.rows[3].w};
        vec4 col4{
          rows[0].dot_product(v),
          rows[1].dot_product(v),
          rows[2].dot_product(v),
          rows[3].dot_product(v)};

        return mat4x4(col1, col2, col3, col4).transposed();
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

    /* matrix transformations. */
    void transpose()
    {
        *this = {
          {rows[0].x, rows[1].x, rows[2].x, rows[3].x},
          {rows[0].y, rows[1].y, rows[2].y, rows[3].y},
          {rows[0].z, rows[1].z, rows[2].z, rows[3].z},
          {rows[0].w, rows[1].w, rows[2].w, rows[3].w}};
    }

    mat4x4 transposed() const
    {
        mat4x4 m{*this};
        m.transpose();
        return m;
    }

    float determinant() const
    {
        const float m00 = rows[0].x, m01 = rows[0].y, m02 = rows[0].z, m03 = rows[0].w;
        const float m10 = rows[1].x, m11 = rows[1].y, m12 = rows[1].z, m13 = rows[1].w;
        const float m20 = rows[2].x, m21 = rows[2].y, m22 = rows[2].z, m23 = rows[2].w;
        const float m30 = rows[3].x, m31 = rows[3].y, m32 = rows[3].z, m33 = rows[3].w;

        const float s0 = m00 * m11 - m10 * m01;
        const float s1 = m00 * m12 - m10 * m02;
        const float s2 = m00 * m13 - m10 * m03;
        const float s3 = m01 * m12 - m11 * m02;
        const float s4 = m01 * m13 - m11 * m03;
        const float s5 = m02 * m13 - m12 * m03;

        const float c5 = m22 * m33 - m32 * m23;
        const float c4 = m21 * m33 - m31 * m23;
        const float c3 = m21 * m32 - m31 * m22;
        const float c2 = m20 * m33 - m30 * m23;
        const float c1 = m20 * m32 - m30 * m22;
        const float c0 = m20 * m31 - m30 * m21;

        return s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;
    }

    bool invert(
      float epsilon = ml::epsilon)
    {
        const float m00 = rows[0].x, m01 = rows[0].y, m02 = rows[0].z, m03 = rows[0].w;
        const float m10 = rows[1].x, m11 = rows[1].y, m12 = rows[1].z, m13 = rows[1].w;
        const float m20 = rows[2].x, m21 = rows[2].y, m22 = rows[2].z, m23 = rows[2].w;
        const float m30 = rows[3].x, m31 = rows[3].y, m32 = rows[3].z, m33 = rows[3].w;

        const float s0 = m00 * m11 - m10 * m01;
        const float s1 = m00 * m12 - m10 * m02;
        const float s2 = m00 * m13 - m10 * m03;
        const float s3 = m01 * m12 - m11 * m02;
        const float s4 = m01 * m13 - m11 * m03;
        const float s5 = m02 * m13 - m12 * m03;

        const float c5 = m22 * m33 - m32 * m23;
        const float c4 = m21 * m33 - m31 * m23;
        const float c3 = m21 * m32 - m31 * m22;
        const float c2 = m20 * m33 - m30 * m23;
        const float c1 = m20 * m32 - m30 * m22;
        const float c0 = m20 * m31 - m30 * m21;

        const float det =
          s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;

        if(std::abs(det) < epsilon)
        {
            return false;
        }

        const float inv_det = 1.0f / det;

        rows[0].x = (m11 * c5 - m12 * c4 + m13 * c3) * inv_det;
        rows[0].y = (-m01 * c5 + m02 * c4 - m03 * c3) * inv_det;
        rows[0].z = (m31 * s5 - m32 * s4 + m33 * s3) * inv_det;
        rows[0].w = (-m21 * s5 + m22 * s4 - m23 * s3) * inv_det;

        rows[1].x = (-m10 * c5 + m12 * c2 - m13 * c1) * inv_det;
        rows[1].y = (m00 * c5 - m02 * c2 + m03 * c1) * inv_det;
        rows[1].z = (-m30 * s5 + m32 * s2 - m33 * s1) * inv_det;
        rows[1].w = (m20 * s5 - m22 * s2 + m23 * s1) * inv_det;

        rows[2].x = (m10 * c4 - m11 * c2 + m13 * c0) * inv_det;
        rows[2].y = (-m00 * c4 + m01 * c2 - m03 * c0) * inv_det;
        rows[2].z = (m30 * s4 - m31 * s2 + m33 * s0) * inv_det;
        rows[2].w = (-m20 * s4 + m21 * s2 - m23 * s0) * inv_det;

        rows[3].x = (-m10 * c3 + m11 * c1 - m12 * c0) * inv_det;
        rows[3].y = (m00 * c3 - m01 * c1 + m02 * c0) * inv_det;
        rows[3].z = (-m30 * s3 + m31 * s1 - m32 * s0) * inv_det;
        rows[3].w = (m20 * s3 - m21 * s1 + m22 * s0) * inv_det;

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
        return {vec4::one(), vec4::one(), vec4::one(), vec4::one()};
    }

    static mat4x4 zero()
    {
        return {vec4::zero(), vec4::zero(), vec4::zero(), vec4::zero()};
    }
};

inline mat4x4 operator*(float s, const mat4x4& m)
{
    return m * s;
}

} /* namespace ml */
