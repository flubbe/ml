/**
 * ml - simple header-only mathematics library
 * 
 * function specializations for vec4.
 * 
 * \author Felix Lubbe
 * \copyright Copyright (c) 2021
 * \license Distributed under the MIT software license (see accompanying LICENSE.txt).
 */

namespace ml
{

/** Linear vector interpolation. */
inline const vec4 lerp(float t, vec4 v1, vec4 v2)
{
    return v1 * (1 - t) + v2 * t;
}

/** clamp a vector to the interval [0,1]. */
inline const vec4 clamp_to_unit_interval(vec4 v)
{
#if defined(ML_USE_SIMD)
    return {_mm_min_ps(_mm_max_ps(v.xyzw, _mm_set_ps1(0.0f)), _mm_set_ps1(1.0f))};
#elif !defined(ML_NO_BOOST)
    return {
      boost::algorithm::clamp(v.x, 0, 1),
      boost::algorithm::clamp(v.y, 0, 1),
      boost::algorithm::clamp(v.z, 0, 1),
      boost::algorithm::clamp(v.w, 0, 1)};
#else
    return {
      std::min(1.f, std::max(0.f, v.x)),
      std::min(1.f, std::max(0.f, v.y)),
      std::min(1.f, std::max(0.f, v.z)),
      std::min(1.f, std::max(0.f, v.w))};
#endif
}

} /* namespace ml */