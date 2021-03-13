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
template <size_t F>
struct vec2_fixed
{
    using type=cnl::scaled_integer<int32_t,cnl::power<-F>>;

    union
    {
        struct
        {
            type x, y;
        };
        struct
        {
            type u, v;
        };
    };

    vec2_fixed()
    {}
    
    vec2_fixed(const vec2_fixed& Other)
        : x(Other.x), y(Other.y)
    {}

    vec2_fixed( const type& in_x, const type& in_y )
        : x(in_x), y(in_y)
    {}

    vec2_fixed( float in_x, float in_y )
        : x(in_x), y(in_y)
    {}

    /*
     * Vector operations.
     */
    const type dot_product(vec2_fixed<F> Other) const
    {
        return type{x * Other.x} + type{y * Other.y};
    }
    
    const type length_squared() const
    {
        return dot_product(*this);
    }
    
    const type length() const
    {
#ifdef __GNUC__
        return sqrtf(ml::to_float(length_squared()));
#else
        return std::sqrtf(ml::to_float(length_squared()));
#endif
    }
    
    const type area(vec2_fixed<F> Other) const
    {
        return type{x * Other.y} - type{y * Other.x};
    }
    
    int area_sign(vec2_fixed<F> Other) const
    {
        return boost::math::sign(area(Other));
    }
    
    /* operators. */
    const vec2_fixed<F> operator+(const vec2_fixed<F>& Other) const
	{
        return {x + Other.x, y + Other.y};
	}
    const vec2_fixed<F> operator-(const vec2_fixed<F>& Other) const
	{
        return {x - Other.x, y - Other.y};
	}
    const vec2_fixed<F> operator-() const
	{
        return {-x,-y};
	}
    const vec2_fixed<F> operator*(float S) const
	{
        return {x * S, y * S};
	}
    const vec2_fixed<F> operator/(float S) const
    {
        return {x / S, y / S};
    }
    
    /* assignment */
    vec2_fixed<F>& operator=(vec2_fixed<F> Other)
    {
        x = Other.x;
        y = Other.y;
        return *this;
    }
    vec2_fixed<F>& operator+=(vec2_fixed<F> Other)
    {
        *this = *this + Other;
        return *this;
    }
    vec2_fixed<F>& operator-=(vec2_fixed<F> Other)
    {
        *this = *this - Other;
        return *this;
    }
    vec2_fixed<F>& operator*=(type S)
    {
        *this = *this * S;
        return *this;
    }
    vec2_fixed<F>& operator/=(type S)
    {
        *this = *this / S;
        return *this;
    }
 
    /* exact comparisons */
    bool operator==(vec2_fixed<F> Other) const
    {
        return x==Other.x && y==Other.y;
    }
    bool operator!=(vec2_fixed<F> Other) const
    {
        return x!=Other.x || y!=Other.y;
    }

    /* access. */
    type& operator[](int c)
    {
        assert(c>=0 && c<2);
        return (&x)[c];
    }
    const type operator[](int c) const
    {
        assert(c>=0 && c<2);
        return (&x)[c];
    }
};

} /* namespace ml */