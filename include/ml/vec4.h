/**
 * ml - simple header-only mathematics library
 *
 * vec4 include dispatcher.
 *
 * \author Felix Lubbe
 * \copyright Copyright (c) 2026
 * \license Distributed under the MIT software license (see accompanying LICENSE.txt).
 */

#ifndef ML_VEC4_H
#define ML_VEC4_H

#ifndef ML_NO_SIMD
#    if defined(ML_SIMD_NEON)
#        include "neon/vec4.h"
#    elif defined(ML_SIMD_SSE)
#        include "sse/vec4.h"
#    else
#        error Unknown ML_SIMD switch
#    endif
#endif

/* Check if we should define vector swizzle functions. */
#if !defined(ML_NO_SWIZZLE)
#    define ML_DEFINE_SWIZZLE_FUNCTIONS
#endif

#if defined(ML_USE_SIMD)
/* ml::vec4 is the SIMD version */
namespace ml
{
using vec4 = simd::vec4;
};

#elif defined(ML_INCLUDE_SIMD)
/* Both SIMD version (ml::simd::vec4) and scalar version */
#    include "scalar/vec4.h"
#else
/* Scalar-only */
namespace ml
{
using vec4 = simd::vec4;
};
#endif

#endif /* ML_VEC4_H */
