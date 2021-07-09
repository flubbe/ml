/**
 * ml - simple header-only mathematics library
 * 
 * fixed-point 2d vector type using CNL.
 * 
 * \author Felix Lubbe
 * \copyright Copyright (c) 2021
 * \license Distributed under the MIT software license (see accompanying LICENSE.txt).
 */

namespace ml
{

/** 2-dimensional fixed-point vector */
template<int F>
struct vec2_fixed
{
    using type = cnl::scaled_integer<int32_t, cnl::power<-F>>;

    union
    {
        struct
        {
            type x{0}, y{0};
        };
        struct
        {
            type u, v;
        };
    };

    vec2_fixed() = default;

    vec2_fixed(const type& in_x, const type& in_y)
    : x{in_x}
    , y{in_y}
    {
    }

    vec2_fixed(float in_x, float in_y)
    : x{in_x}
    , y{in_y}
    {
    }

    vec2_fixed(const vec2_fixed&) = default;
    vec2_fixed(vec2_fixed&&) = default;

    vec2_fixed<F>& operator=(const vec2_fixed<F>&) = default;

    /*
     * Vector operations.
     */

    auto dot_product(const vec2_fixed<F>& v) const -> const decltype(x * v.x + y * v.y)
    {
        return x * v.x + y * v.y;
    }

    const type length_squared() const
    {
        return dot_product(*this);
    }

    auto area(const vec2_fixed<F>& v) const -> const decltype(x * v.y - y * v.x)
    {
        return x * v.y - y * v.x;
    }

    int area_sign(const vec2_fixed<F>& v) const
    {
#ifdef ML_NO_BOOST
        const auto a = area(other);
        return (a == 0) ? 0 : ((a > 0) ? 1 : -1);
#else
        return boost::math::sign(area(v));
#endif
    }

    /*
     * operators. 
     */

    const vec2_fixed<F> operator+(const vec2_fixed<F>& v) const
    {
        return {x + v.x, y + v.y};
    }
    const vec2_fixed<F> operator-(const vec2_fixed<F>& v) const
    {
        return {x - v.x, y - v.y};
    }
    const vec2_fixed<F> operator-() const
    {
        return {-x, -y};
    }
    const vec2_fixed<F> operator*(float s) const
    {
        return {x * s, y * s};
    }
    const vec2_fixed<F> operator/(float s) const
    {
        return {x / s, y / s};
    }

    /*
     * assignments.
     */

    vec2_fixed<F>& operator+=(const vec2_fixed<F>& v)
    {
        *this = *this + v;
        return *this;
    }
    vec2_fixed<F>& operator-=(const vec2_fixed<F>& v)
    {
        *this = *this - v;
        return *this;
    }

    /* 
     * exact comparisons.
     */

    bool operator==(const vec2_fixed<F>& v) const
    {
        return x == v.x && y == v.y;
    }
    bool operator!=(const vec2_fixed<F>& v) const
    {
        return x != v.x || y != v.y;
    }

    /* 
     * element access. 
     */

    type& operator[](int c)
    {
        assert(c >= 0 && c < 2);
        return (&x)[c];
    }
    const type operator[](int c) const
    {
        assert(c >= 0 && c < 2);
        return (&x)[c];
    }
};

} /* namespace ml */
