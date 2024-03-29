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
        vec4 col1{dot(rows[0], v), dot(rows[1], v), dot(rows[2], v), dot(rows[3], v)};

        v = {m.rows[0].y, m.rows[1].y, m.rows[2].y, m.rows[3].y};
        vec4 col2{dot(rows[0], v), dot(rows[1], v), dot(rows[2], v), dot(rows[3], v)};

        v = {m.rows[0].z, m.rows[1].z, m.rows[2].z, m.rows[3].z};
        vec4 col3{dot(rows[0], v), dot(rows[1], v), dot(rows[2], v), dot(rows[3], v)};

        v = {m.rows[0].w, m.rows[1].w, m.rows[2].w, m.rows[3].w};
        vec4 col4{dot(rows[0], v), dot(rows[1], v), dot(rows[2], v), dot(rows[3], v)};

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
};

} /* namespace ml */