/**
 * ml - simple header-only mathematics library
 *
 * vec4 include dispatcher.
 *
 * \author Felix Lubbe
 * \copyright Copyright (c) 2026
 * \license Distributed under the MIT software license (see accompanying LICENSE.txt).
 */

#include "scalar/vec4.h"

#ifndef ML_NO_SIMD
#    if defined(ML_SIMD_NEON)
#        include "neon/vec4.h"
#    elif defined(ML_SIMD_SSE)
#        include "sse/vec4.h"
#    else
#        error Unknown ML_SIMD switch
#    endif
#endif

#if defined(ML_USE_SIMD)

namespace ml
{
using vec4 = simd::vec4;
};

#else /* ML_NO_SIMD */

namespace ml
{
using vec4 = scalar::vec4;
}

#endif /* ML_NO_SIMD */
