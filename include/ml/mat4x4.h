/**
 * ml - simple header-only mathematics library
 *
 * mat4x4 include dispatcher.
 *
 * \author Felix Lubbe
 * \copyright Copyright (c) 2026
 * \license Distributed under the MIT software license (see accompanying LICENSE.txt).
 */

#ifndef ML_MAT4X4_H
#define ML_MAT4X4_H

#include "scalar/mat4x4.h"

#ifndef ML_NO_SIMD
#    if defined(ML_SIMD_NEON)
#        include "neon/mat4x4.h"
#    elif defined(ML_SIMD_SSE)
#        include "sse/mat4x4.h"
#    else
#        error Unknown ML_SIMD switch
#    endif
#endif

#if defined(ML_USE_SIMD)

/* ml::mat4x4 is the SIMD version */
namespace ml
{
using mat4x4 = simd::mat4x4;
};

#else

namespace ml
{
using mat4x4 = scalar::mat4x4;
};

#endif

#endif /* ML_MAT4X4_H */
