/**
 * ml - simple header-only mathematics library
 * 
 * dummy header to include the correct mat4x4 implementation.
 * 
 * \author Felix Lubbe
 * \copyright Copyright (c) 2021
 * \license Distributed under the MIT software license (see accompanying LICENSE.txt).
 */

#if defined(ML_USE_SIMD)
#    include "simd/mat4x4.h"
namespace ml
{
using mat4x4 = simd::mat4x4;
}; /* namespace ml */
#elif defined(ML_INCLUDE_SIMD)
#    include "x86/mat4x4.h"
#    include "simd/mat4x4.h"
#else
#    include "x86/mat4x4.h"
#endif
