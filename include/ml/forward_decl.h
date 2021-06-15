/**
 * ml - simple header-only mathematics library
 * 
 * Forward declarations of vector types.
 * 
 * \author Felix Lubbe
 * \copyright Copyright (c) 2021
 * \license Distributed under the MIT software license (see accompanying LICENSE.txt).
 */

namespace ml
{

struct vec2;
struct vec3;
#if defined(ML_USE_SIMD)
namespace simd
{
struct vec4;
} /* namespace simd */
using vec4 = simd::vec4;
#elif defined(ML_INCLUDE_SIMD)
namespace simd
{
struct vec4;
} /* namespace simd */
struct vec4;
#else
struct vec4;
#endif /* defined(ML_USE_SIMD) */

} /* namespace ml */
