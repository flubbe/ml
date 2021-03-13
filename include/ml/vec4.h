/**
 * ml - simple header-only mathematics library
 * 
 * dummy header to include the correct vec4 implementation.
 * 
 * \author Felix Lubbe
 * \copyright Copyright (c) 2021
 * \license Distributed under the MIT software license (see accompanying LICENSE.txt).
 */

#if defined(ML_USE_SIMD)
#   include "simd/vec4.h"
namespace ml
{
    using vec4 = simd::vec4;
}; /* namespace ml */
#elif defined(ML_INCLUDE_SIMD)
#   include "x86/vec4.h"
#   include "simd/vec4.h"
#else
#   include "x86/vec4.h"
#endif

