/**
 * ml - simple header-only mathematics library
 * 
 * planes and lines.
 * 
 * \author Felix Lubbe
 * \copyright Copyright (c) 2021
 * \license Distributed under the MIT software license (see accompanying LICENSE.txt).
 */

namespace ml
{

/** A 3-dimensional plane of the form A*x+B*y+C*z+D=0. */
struct plane : public vec4
{
    plane()
    : vec4(0, 0, 0, 1) /* invalid plane. */
	{}

    plane(vec3 n, float d)
    : vec4(n, d)
	{}

    plane(float a, float b, float c, float d)
    : vec4(a, b, c, d)
	{}

	plane(const plane& o)
		: vec4(o)
	{}

    float distance( vec3 p ) const
    {
        const auto proj = xyz();
        return (proj.dot_product(p) + w) / proj.length();
    }
};

/** A line. */
template<typename T>
struct line
{
    T pos, dir;

    line() = default;

    line( const T& in_pos, const T& in_dir )
    : pos(in_pos), dir(in_dir)
    {}

    const T evaluate_at( float param ) const
    {
        return pos + dir * param;
    }
};

/** create a line from two points. */
template<typename T>
const line<T> create_line( const T& p1, const T& p2 )
{
    return { p1, p2-p1 };
}

/** 3d line. */
typedef line<vec3> line3;

/** 4d line. */
typedef line<vec4> line4;

} /* namespace ml */