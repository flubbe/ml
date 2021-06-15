/**
 * ml - simple header-only mathematics library
 * 
 * helper to implement vector swizzle notation.
 * 
 * \author Felix Lubbe
 * \copyright Copyright (c) 2021
 * \license Distributed under the MIT software license (see accompanying LICENSE.txt).
 */

#define ML_IMPLEMENT_SWIZZLE_FUNCTIONS
/* vec2 */
#define ML_SWIZZLE_COMPONENTS 2
#define ML_SWIZZLE_TYPE       ml::vec2
#include "swizzle.inl"
#undef ML_SWIZZLE_TYPE
#undef ML_SWIZZLE_COMPONENTS
/* vec3 */
#define ML_SWIZZLE_COMPONENTS 3
#define ML_SWIZZLE_TYPE       ml::vec3
#include "swizzle.inl"
#undef ML_SWIZZLE_TYPE
#undef ML_SWIZZLE_COMPONENTS
/* vec4 */
#define ML_SWIZZLE_COMPONENTS 4
#define ML_SWIZZLE_TYPE       ml::vec4
#include "swizzle.inl"
#undef ML_SWIZZLE_TYPE
#undef ML_SWIZZLE_COMPONENTS
#undef ML_IMPLEMENT_SWIZZLE_FUNCTIONS
