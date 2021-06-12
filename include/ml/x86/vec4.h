/**
 * ml - simple header-only mathematics library
 * 
 * 4d vector implementation.
 * 
 * \author Felix Lubbe
 * \copyright Copyright (c) 2021
 * \license Distributed under the MIT software license (see accompanying LICENSE.txt).
 */

namespace ml
{

/** 4-dimensional vector */
struct vec4
{
    union
    {
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

    vec4()
    {
    }

    vec4(const vec3 v)
    : x(v.x)
    , y(v.y)
    , z(v.z)
    , w(1)
    {
    }

    vec4(const vec3 v, float in_w)
    : x(v.x)
    , y(v.y)
    , z(v.z)
    , w(in_w)
    {
    }

    vec4(float in_x, float in_y, float in_z)
    : x(in_x)
    , y(in_y)
    , z(in_z)
    , w(1)
    {
    }

    vec4(float in_x, float in_y, float in_z, float in_w)
    : x(in_x)
    , y(in_y)
    , z(in_z)
    , w(in_w)
    {
    }

    vec4(float v[4])
    {
        x = v[0];
        y = v[1];
        z = v[2];
        w = v[3];
    }

    float divide_by_w()
    {
        assert(w != 0.f);

        const auto one_over_w = 1.0f / w;
        x *= one_over_w;
        y *= one_over_w;
        z *= one_over_w;
        w = 1;

        return one_over_w;
    }

    bool is_zero() const
    {
        return x == 0 && y == 0 && z == 0 && w == 0;
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

    float dot_product(const vec4& Other) const
    {
        return x * Other.x + y * Other.y + z * Other.z + w * Other.w;
    }

    const vec4 scale(const float S) const
    {
        return {x * S, y * S, z * S, w * S};
    }

    void normalize()
    {
        /* OneOverLength is safe to call on zero vectors - no check needed. */
        *this = *this * one_over_length();
    }
    const vec4 normalized() const
    {
        return *this * one_over_length();
    }

    /* operations. */
    const vec4 operator+(const vec4& Other) const
    {
        return {x + Other.x, y + Other.y, z + Other.z, w + Other.w};
    }
    const vec4 operator+(float f) const
    {
        return {x + f, y + f, z + f, w + f};
    }
    const vec4 operator-(const vec4& Other) const
    {
        return {x - Other.x, y - Other.y, z - Other.z, w - Other.w};
    }
    const vec4 operator-(float f) const
    {
        return {x - f, y - f, z - f, w - f};
    }
    const vec4 operator-() const
    {
        return {-x, -y, -z, -w};
    }
    const vec4 operator*(const vec4& Other) const
    {
        return {x * Other.x, y * Other.y, z * Other.z, w * Other.w};
    }
    const vec4 operator*(float S) const
    {
        return scale(S);
    }
    const vec4 operator/(float S) const
    {
        return scale(1.0f / S);
    }
    const vec4 operator/(const vec4 other) const
    {
        return {x / other.x, y / other.y, z / other.z, w / other.w};
    }

    vec4& operator+=(const vec4& Other)
    {
        *this = *this + Other;
        return *this;
    }
    vec4& operator-=(const vec4& Other)
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
        *this = *this * S;
        return *this;
    }
    vec4& operator/=(float S)
    {
        *this = *this / S;
        return *this;
    }

    /* exact comparisons */
    bool operator==(const vec4& Other) const
    {
        return x == Other.x && y == Other.y && z == Other.z && w == Other.w;
    }
    bool operator!=(const vec4& Other) const
    {
        return x != Other.x || y != Other.y || z != Other.z || w != Other.w;
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

    /* projection onto first components */
    const vec3 xyz() const
    {
        return {x, y, z};
    }
    const vec2 xy() const
    {
        return {x, y};
    }

    const vec3 rgb() const
    {
        return {r, g, b};
    }

    const vec2 st() const
    {
        return {s, t};
    }

    /* special vectors. */
    static const vec4 zero()
    {
        // note that by default W is initialized to 1, so we initialize the vector explicitely.
        return {0, 0, 0, 0};
    }

    static const vec4 one()
    {
        return {1, 1, 1, 1};
    }
};

} /* namespace ml */