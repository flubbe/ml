/**
 * ml - simple header-only mathematics library
 * 
 * Main header, includes the whole library.
 * 
 * The following preprocessor definitions can be used to configure the library:
 * 
 *   ML_NO_DEPS:      don't include any dependencies and support functions.
 *                    equivalent to defining ML_NO_CPP, ML_NO_BOOST and ML_NO_CNL
 *   ML_NO_CPP:       don't include standard C++-headers.
 *   ML_NO_BOOST:     don't include boost headers.
 *   ML_NO_CNL:       don't include CNL support and headers.
 * 
 * Further:
 * 
 *   ML_NO_SIMD:      don't use SSE versions of vec4 and mat4x4
 *   ML_INCLUDE_SIMD: provide SSE and non-SSE versions of vec4 and mat4x4
 * 
 * \author Felix Lubbe
 * \copyright Copyright (c) 2021
 * \license Distributed under the MIT software license (see accompanying LICENSE.txt).
 */

#pragma once

#ifdef ML_NO_DEPS

#define ML_NO_CPP
#define ML_NO_BOOST
#define ML_NO_CNL

#endif /* ML_NO_DEPS */

#ifndef ML_NO_CPP

/* C++ headers */
#include <algorithm>
#include <cmath>

#endif /* ML_NO_CPP */

#ifndef ML_NO_BOOST

/* boost */
#include <boost/math/special_functions/sign.hpp>
#include <boost/algorithm/clamp.hpp>

#endif /* ML_NO_BOOST */

#ifndef ML_NO_CNL

/* CNL for most fixed-point types. */
#include "cnl/static_number.h"
#include "cnl/num_traits.h"

/* CNL support functions. */
#include "cnl_support.h"

#endif /* ML_NO_CNL */

/* fixed-point unit interval. */
#include "closed_unit_interval.h"

/* enable SIMD (if not requested to disable or just include it) */
#if !defined(ML_NO_SIMD) && !defined(ML_INCLUDE_SIMD)
#   define ML_USE_SIMD
#endif /* IML_INCLUDE_SIMD */

/*
 * SIMD.
 */
#if defined(ML_USE_SIMD) || defined(ML_INCLUDE_SIMD)

#define ML_USE_SSE41
#define ML_USE_SSE3

/* SSE intrinsics */
#include <mmintrin.h>  /* MMX */
#include <xmmintrin.h> /* SSE */
#include <emmintrin.h> /* SSE2 */
#include <pmmintrin.h> /* SSE3 */
/*
#include <tmmintrin.h> SSSE3
*/
#include <smmintrin.h> /* SSE4.1 */
/*
#include <nmmintrin.h> SSE4.2
#include <ammintrin.h> SSE4A
#include <wmmintrin.h> AES
#include <immintrin.h> AVX, AVX2, FMA
*/

#endif /* defined(ML_USE_SIMD) || defined(ML_INCLUDE_SIMD) */

/*
 * include libarary headers.
 */

/* some mathematical constants. */
#include "constants.h"

/* fixed point types */
#include "fixed_point.h"

/* mathematical functions that do not depend on the types included below. */
#include "functions.h"
  
/* vectors and matrices */
#include "vec2.h"
#ifndef ML_NO_CNL
#include "vec2_fix.h"
#endif /* ML_NO_CNL */
#include "vec3.h"
#include "vec4.h"
#include "mat4x4.h"

/* templated 2d vector class for easy handling of 2d composite types. */
#include "tvec2.h"

/* special matrices. */
#include "matrices.h"

/* mathematical functions. */
#include "functions_vec4.h"

/* geometric objects and helper functions. */
#include "geometry.h"
