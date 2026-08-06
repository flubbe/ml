/**
 * ml - simple header-only mathematics library
 *
 * 3d matrix implementation.
 *
 * \author Felix Lubbe
 * \copyright Copyright (c) 2021
 * \license Distributed under the MIT software license (see accompanying LICENSE.txt).
 */

#include <optional>

namespace ml
{

/** 3x3 matrix */
struct mat3x3
{
    vec3 rows[3];

    mat3x3()
    {
        *this = zero();
    }
    mat3x3(const vec3& row0, const vec3& row1, const vec3& row2)
    : rows{row0, row1, row2}
    {
    }

    mat3x3(const mat3x3&) = default;
    mat3x3(mat3x3&&) = default;

    mat3x3& operator=(const mat3x3&) = default;

    /* matrix-matrix operations. */
    mat3x3 operator+(const mat3x3& m) const
    {
        return {
          rows[0] + m.rows[0],
          rows[1] + m.rows[1],
          rows[2] + m.rows[2]};
    }
    mat3x3 operator-(const mat3x3& m) const
    {
        return {
          rows[0] - m.rows[0],
          rows[1] - m.rows[1],
          rows[2] - m.rows[2]};
    }
    mat3x3 operator-() const
    {
        return {-rows[0], -rows[1], -rows[2]};
    }
    mat3x3 operator*(const mat3x3& m) const
    {
        vec3 v{m.rows[0].x, m.rows[1].x, m.rows[2].x};
        vec3 col1{
          rows[0].dot_product(v),
          rows[1].dot_product(v),
          rows[2].dot_product(v)};

        v = {m.rows[0].y, m.rows[1].y, m.rows[2].y};
        vec3 col2{
          rows[0].dot_product(v),
          rows[1].dot_product(v),
          rows[2].dot_product(v)};

        v = {m.rows[0].z, m.rows[1].z, m.rows[2].z};
        vec3 col3{
          rows[0].dot_product(v),
          rows[1].dot_product(v),
          rows[2].dot_product(v)};

        return mat3x3(col1, col2, col3).transposed();
    }

    /* matrix-vector multiplication. */
    vec3 operator*(const vec3& v) const
    {
        return {rows[0].dot_product(v), rows[1].dot_product(v), rows[2].dot_product(v)};
    }

    /* scaling */
    mat3x3 operator*(float s) const
    {
        return {rows[0] * s, rows[1] * s, rows[2] * s};
    }

    /* assignments */
    mat3x3& operator*=(const mat3x3& m)
    {
        *this = *this * m;
        return *this;
    }

    mat3x3 operator*=(float s)
    {
        *this = *this * s;
        return *this;
    }
    mat3x3 operator/=(float s)
    {
        *this = *this * (1.0f / s);
        return *this;
    }

    /* exact comparisons */
    bool operator==(const mat3x3& m) const
    {
        return rows[0] == m.rows[0] && rows[1] == m.rows[1] && rows[2] == m.rows[2];
    }
    bool operator!=(const mat3x3& m) const
    {
        return rows[0] != m.rows[0] || rows[1] != m.rows[1] || rows[2] != m.rows[2];
    }

    /* matrix transformations. */
    void transpose()
    {
        *this = {
          {rows[0].x, rows[1].x, rows[2].x},
          {rows[0].y, rows[1].y, rows[2].y},
          {rows[0].z, rows[1].z, rows[2].z}};
    }

    mat3x3 transposed() const
    {
        mat3x3 m{*this};
        m.transpose();
        return m;
    }

    float determinant() const
    {
        const float a = rows[0].x;
        const float b = rows[0].y;
        const float c = rows[0].z;

        const float d = rows[1].x;
        const float e = rows[1].y;
        const float f = rows[1].z;

        const float g = rows[2].x;
        const float h = rows[2].y;
        const float i = rows[2].z;

        return a * (e * i - f * h)
               - b * (d * i - f * g)
               + c * (d * h - e * g);
    }

    bool invert(
      float epsilon = ml::epsilon)
    {
        const float a = rows[0].x;
        const float b = rows[0].y;
        const float c = rows[0].z;

        const float d = rows[1].x;
        const float e = rows[1].y;
        const float f = rows[1].z;

        const float g = rows[2].x;
        const float h = rows[2].y;
        const float i = rows[2].z;

        const float A = e * i - f * h;
        const float B = f * g - d * i;
        const float C = d * h - e * g;

        const float det = a * A + b * B + c * C;

        if(std::abs(det) <= epsilon)
        {
            return false;
        }

        const float inv_det = 1.0f / det;

        rows[0] = {
          A * inv_det,
          (c * h - b * i) * inv_det,
          (b * f - c * e) * inv_det};

        rows[1] = {
          B * inv_det,
          (a * i - c * g) * inv_det,
          (c * d - a * f) * inv_det};

        rows[2] = {
          C * inv_det,
          (b * g - a * h) * inv_det,
          (a * e - b * d) * inv_det};

        return true;
    }

    std::optional<mat3x3> inverse(
      float epsilon = ml::epsilon) const
    {
        mat3x3 result{*this};

        if(!result.invert(epsilon))
        {
            return std::nullopt;
        }

        return result;
    }

    /* access. */
    vec3& operator[](int c)
    {
        assert(c >= 0 && c < 3);
        return rows[c];
    }
    vec3 operator[](int c) const
    {
        assert(c >= 0 && c < 3);
        return rows[c];
    }

    /* special matrices. */
    static mat3x3 identity()
    {
        return {
          {1.0f, 0.0f, 0.0f},
          {0.0f, 1.0f, 0.0f},
          {0.0f, 0.0f, 1.0f}};
    }

    static mat3x3 one()
    {
        return {vec3::one(), vec3::one(), vec3::one()};
    }

    static mat3x3 zero()
    {
        return {vec3::zero(), vec3::zero(), vec3::zero()};
    }
};

inline mat3x3 operator*(float s, const mat3x3& m)
{
    return m * s;
}

}    // namespace ml
