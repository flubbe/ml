/**
 * ml - simple header-only mathematics library
 * 
 * component access for vec2/vec3/vec4.
 * 
 * \author Felix Lubbe
 * \copyright Copyright (c) 2021
 * \license Distributed under the MIT software license (see accompanying LICENSE.txt).
 */

#ifndef ML_SWIZZLE_COMPONENTS
#    error ML_SWIZZLE_COMPONENTS not set.
#endif

#if ML_SWIZZLE_COMPONENTS != 2 && ML_SWIZZLE_COMPONENTS != 3 && ML_SWIZZLE_COMPONENTS != 4
#    error ML_SWIZZLE_COMPONENTS must be either 2, 3 or 4
#endif

#if defined(ML_DEFINE_SWIZZLE_FUNCTIONS) && defined(ML_IMPLEMENT_SWIZZLE_FUNCTIONS)
#    if !defined ML_SWIZZLE_TYPE
#        error ML_SWIZZLE_TYPE undefined.
#    endif /* ML_SWIZZLE_TYPE */
#endif

/*
 * vec2 access. 
 */

#ifdef ML_DEFINE_SWIZZLE_FUNCTIONS
#    ifdef ML_IMPLEMENT_SWIZZLE_FUNCTIONS
#        define ML_SWIZZLE2(a, b)                               \
            inline const ml::vec2 ML_SWIZZLE_TYPE::a##b() const \
            {                                                   \
                return {a, b};                                  \
            }
#    else
#        define ML_SWIZZLE2(a, b) \
            const vec2 a##b() const;
#    endif
#else
#    define ML_SWIZZLE2(a, b)
#endif

ML_SWIZZLE2(x, x)
ML_SWIZZLE2(x, y)

#if ML_SWIZZLE_COMPONENTS >= 3
ML_SWIZZLE2(x, z)
#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE2(x, w)
#    endif /* ML_SWIZZLE_COMPONENTS >= 4 */
#endif     /* ML_SWIZZLE_COMPONENTS >= 3 */

ML_SWIZZLE2(y, x)
ML_SWIZZLE2(y, y)

#if ML_SWIZZLE_COMPONENTS >= 3
ML_SWIZZLE2(y, z)
#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE2(y, w)
#    endif /* ML_SWIZZLE_COMPONENTS >= 4 */
#endif     /* ML_SWIZZLE_COMPONENTS >= 3 */

#if ML_SWIZZLE_COMPONENTS >= 3

ML_SWIZZLE2(z, x)
ML_SWIZZLE2(z, y)
ML_SWIZZLE2(z, z)

#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE2(z, w)
#    endif /* ML_SWIZZLE_COMPONENTS >= 4 */

#endif /* ML_SWIZZLE_COMPONENTS >= 3 */

#if ML_SWIZZLE_COMPONENTS == 4

ML_SWIZZLE2(w, x)
ML_SWIZZLE2(w, y)
ML_SWIZZLE2(w, z)
ML_SWIZZLE2(w, w)

#endif /* ML_SWIZZLE_COMPONENTS == 4 */

#undef ML_SWIZZLE2

/*
 * vec3 access.
 */

#ifdef ML_DEFINE_SWIZZLE_FUNCTIONS
#    ifdef ML_IMPLEMENT_SWIZZLE_FUNCTIONS
#        define ML_SWIZZLE3(a, b, c)                               \
            inline const ml::vec3 ML_SWIZZLE_TYPE::a##b##c() const \
            {                                                      \
                return {a, b, c};                                  \
            }
#    else
#        define ML_SWIZZLE3(a, b, c) \
            const struct vec3 a##b##c() const;
#    endif
#else
#    define ML_SWIZZLE3(a, b, c)
#endif

/* x** */

ML_SWIZZLE3(x, x, x)
ML_SWIZZLE3(x, x, y)

#if ML_SWIZZLE_COMPONENTS >= 3
ML_SWIZZLE3(x, x, z)
#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE3(x, x, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */
#endif     /* ML_SWIZZLE_COMPONENTS >= 3 */

ML_SWIZZLE3(x, y, x)
ML_SWIZZLE3(x, y, y)

#if ML_SWIZZLE_COMPONENTS >= 3
ML_SWIZZLE3(x, y, z)
#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE3(x, y, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */
#endif     /* ML_SWIZZLE_COMPONENTS >= 3 */

#if ML_SWIZZLE_COMPONENTS >= 3

ML_SWIZZLE3(x, z, x)
ML_SWIZZLE3(x, z, y)

ML_SWIZZLE3(x, z, z)
#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE3(x, z, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */

#endif /* ML_SWIZZLE_COMPONENTS >= 3 */

#if ML_SWIZZLE_COMPONENTS == 4

ML_SWIZZLE3(x, w, x)
ML_SWIZZLE3(x, w, y)
ML_SWIZZLE3(x, w, z)
ML_SWIZZLE3(x, w, w)

#endif /* ML_SWIZZLE_COMPONENTS == 4 */

/* y** */

ML_SWIZZLE3(y, x, x)
ML_SWIZZLE3(y, x, y)

#if ML_SWIZZLE_COMPONENTS >= 3
ML_SWIZZLE3(y, x, z)
#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE3(y, x, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */
#endif     /* ML_SWIZZLE_COMPONENTS >= 3 */

ML_SWIZZLE3(y, y, x)
ML_SWIZZLE3(y, y, y)

#if ML_SWIZZLE_COMPONENTS >= 3
ML_SWIZZLE3(y, y, z)
#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE3(y, y, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */
#endif     /* ML_SWIZZLE_COMPONENTS >= 3 */

#if ML_SWIZZLE_COMPONENTS >= 3

ML_SWIZZLE3(y, z, x)
ML_SWIZZLE3(y, z, y)

ML_SWIZZLE3(y, z, z)
#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE3(y, z, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */

#endif /* ML_SWIZZLE_COMPONENTS >= 3 */

#if ML_SWIZZLE_COMPONENTS == 4

ML_SWIZZLE3(y, w, x)
ML_SWIZZLE3(y, w, y)
ML_SWIZZLE3(y, w, z)
ML_SWIZZLE3(y, w, w)

#endif /* ML_SWIZZLE_COMPONENTS == 4 */

/* z** */

#if ML_SWIZZLE_COMPONENTS >= 3

ML_SWIZZLE3(z, x, x)
ML_SWIZZLE3(z, x, y)
ML_SWIZZLE3(z, x, z)

#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE3(z, x, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */

ML_SWIZZLE3(z, y, x)
ML_SWIZZLE3(z, y, y)
ML_SWIZZLE3(z, y, z)

#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE3(z, y, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */

ML_SWIZZLE3(z, z, x)
ML_SWIZZLE3(z, z, y)
ML_SWIZZLE3(z, z, z)

#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE3(z, z, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */

#endif /* ML_SWIZZLE_COMPONENTS >= 3 */

#if ML_SWIZZLE_COMPONENTS == 4

ML_SWIZZLE3(z, w, x)
ML_SWIZZLE3(z, w, y)
ML_SWIZZLE3(z, w, z)
ML_SWIZZLE3(z, w, w)

#endif /* ML_SWIZZLE_COMPONENTS == 4 */

/* w** */

#if ML_SWIZZLE_COMPONENTS == 4

ML_SWIZZLE3(w, x, x)
ML_SWIZZLE3(w, x, y)
ML_SWIZZLE3(w, x, z)
ML_SWIZZLE3(w, x, w)

ML_SWIZZLE3(w, y, x)
ML_SWIZZLE3(w, y, y)
ML_SWIZZLE3(w, y, z)
ML_SWIZZLE3(w, y, w)

ML_SWIZZLE3(w, z, x)
ML_SWIZZLE3(w, z, y)
ML_SWIZZLE3(w, z, z)
ML_SWIZZLE3(w, z, w)

ML_SWIZZLE3(w, w, x)
ML_SWIZZLE3(w, w, y)
ML_SWIZZLE3(w, w, z)
ML_SWIZZLE3(w, w, w)

#endif /* ML_SWIZZLE_COMPONENTS == 4 */

#undef ML_SWIZZLE3

/*
 * vec4 access.
 */

#ifdef ML_DEFINE_SWIZZLE_FUNCTIONS
#    ifdef ML_IMPLEMENT_SWIZZLE_FUNCTIONS
#        define ML_SWIZZLE4(a, b, c, d)                               \
            inline const ml::vec4 ML_SWIZZLE_TYPE::a##b##c##d() const \
            {                                                         \
                return {a, b, c, d};                                  \
            }
#    else
#        define ML_SWIZZLE4(a, b, c, d) \
            const ML_SWIZZLE_VEC4_TYPE a##b##c##d() const;
#    endif
#else
#    define ML_SWIZZLE4(a, b, c, d)
#endif

/* xx** */

ML_SWIZZLE4(x, x, x, x)
ML_SWIZZLE4(x, x, x, y)

#if ML_SWIZZLE_COMPONENTS >= 3
ML_SWIZZLE4(x, x, x, z)
#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE4(x, x, x, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */
#endif     /* ML_SWIZZLE_COMPONENTS >= 3 */

ML_SWIZZLE4(x, x, y, x)
ML_SWIZZLE4(x, x, y, y)

#if ML_SWIZZLE_COMPONENTS >= 3
ML_SWIZZLE4(x, x, y, z)
#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE4(x, x, y, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */
#endif     /* ML_SWIZZLE_COMPONENTS >= 3 */

#if ML_SWIZZLE_COMPONENTS >= 3

ML_SWIZZLE4(x, x, z, x)
ML_SWIZZLE4(x, x, z, y)
ML_SWIZZLE4(x, x, z, z)

#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE4(x, x, z, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */

#endif /* ML_SWIZZLE_COMPONENTS >= 3 */

#if ML_SWIZZLE_COMPONENTS == 4

ML_SWIZZLE4(x, x, w, x)
ML_SWIZZLE4(x, x, w, y)
ML_SWIZZLE4(x, x, w, z)
ML_SWIZZLE4(x, x, w, w)

#endif /* ML_SWIZZLE_COMPONENTS == 4 */

/* xy** */

ML_SWIZZLE4(x, y, x, x)
ML_SWIZZLE4(x, y, x, y)

#if ML_SWIZZLE_COMPONENTS >= 3
ML_SWIZZLE4(x, y, x, z)
#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE4(x, y, x, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */
#endif     /* ML_SWIZZLE_COMPONENTS >= 3 */

ML_SWIZZLE4(x, y, y, x)
ML_SWIZZLE4(x, y, y, y)

#if ML_SWIZZLE_COMPONENTS >= 3
ML_SWIZZLE4(x, y, y, z)
#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE4(x, y, y, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */
#endif     /* ML_SWIZZLE_COMPONENTS >= 3 */

#if ML_SWIZZLE_COMPONENTS >= 3

ML_SWIZZLE4(x, y, z, x)
ML_SWIZZLE4(x, y, z, y)
ML_SWIZZLE4(x, y, z, z)

#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE4(x, y, z, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */

#endif /* ML_SWIZZLE_COMPONENTS >= 3 */

#if ML_SWIZZLE_COMPONENTS == 4

ML_SWIZZLE4(x, y, w, x)
ML_SWIZZLE4(x, y, w, y)
ML_SWIZZLE4(x, y, w, z)
ML_SWIZZLE4(x, y, w, w)

#endif /* ML_SWIZZLE_COMPONENTS == 4 */

/* xz** */

#if ML_SWIZZLE_COMPONENTS >= 3

ML_SWIZZLE4(x, z, x, x)
ML_SWIZZLE4(x, z, x, y)
ML_SWIZZLE4(x, z, x, z)

#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE4(x, z, x, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */

ML_SWIZZLE4(x, z, y, x)
ML_SWIZZLE4(x, z, y, y)
ML_SWIZZLE4(x, z, y, z)

#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE4(x, z, y, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */

ML_SWIZZLE4(x, z, z, x)
ML_SWIZZLE4(x, z, z, y)
ML_SWIZZLE4(x, z, z, z)

#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE4(x, z, z, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */

#    if ML_SWIZZLE_COMPONENTS == 4

ML_SWIZZLE4(x, z, w, x)
ML_SWIZZLE4(x, z, w, y)
ML_SWIZZLE4(x, z, w, z)
ML_SWIZZLE4(x, z, w, w)

#    endif /* ML_SWIZZLE_COMPONENTS == 4 */

#endif /* ML_SWIZZLE_COMPONENTS >= 3 */

/* xw** */

#if ML_SWIZZLE_COMPONENTS == 4

ML_SWIZZLE4(x, w, x, x)
ML_SWIZZLE4(x, w, x, y)
ML_SWIZZLE4(x, w, x, z)
ML_SWIZZLE4(x, w, x, w)

ML_SWIZZLE4(x, w, y, x)
ML_SWIZZLE4(x, w, y, y)
ML_SWIZZLE4(x, w, y, z)
ML_SWIZZLE4(x, w, y, w)

ML_SWIZZLE4(x, w, z, x)
ML_SWIZZLE4(x, w, z, y)
ML_SWIZZLE4(x, w, z, z)
ML_SWIZZLE4(x, w, z, w)

ML_SWIZZLE4(x, w, w, x)
ML_SWIZZLE4(x, w, w, y)
ML_SWIZZLE4(x, w, w, z)
ML_SWIZZLE4(x, w, w, w)

#endif /* ML_SWIZZLE_COMPONENTS == 4 */

/* yx** */

ML_SWIZZLE4(y, x, x, x)
ML_SWIZZLE4(y, x, x, y)

#if ML_SWIZZLE_COMPONENTS >= 3
ML_SWIZZLE4(y, x, x, z)
#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE4(y, x, x, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */
#endif     /* ML_SWIZZLE_COMPONENTS >= 3 */

ML_SWIZZLE4(y, x, y, x)
ML_SWIZZLE4(y, x, y, y)

#if ML_SWIZZLE_COMPONENTS >= 3
ML_SWIZZLE4(y, x, y, z)
#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE4(y, x, y, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */
#endif     /* ML_SWIZZLE_COMPONENTS >= 3 */

#if ML_SWIZZLE_COMPONENTS >= 3

ML_SWIZZLE4(y, x, z, x)
ML_SWIZZLE4(y, x, z, y)
ML_SWIZZLE4(y, x, z, z)

#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE4(y, x, z, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */

#endif /* ML_SWIZZLE_COMPONENTS >= 3 */

#if ML_SWIZZLE_COMPONENTS == 4

ML_SWIZZLE4(y, x, w, x)
ML_SWIZZLE4(y, x, w, y)
ML_SWIZZLE4(y, x, w, z)
ML_SWIZZLE4(y, x, w, w)

#endif /* ML_SWIZZLE_COMPONENTS == 4 */

/* yy** */

ML_SWIZZLE4(y, y, x, x)
ML_SWIZZLE4(y, y, x, y)

#if ML_SWIZZLE_COMPONENTS >= 3
ML_SWIZZLE4(y, y, x, z)
#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE4(y, y, x, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */
#endif     /* ML_SWIZZLE_COMPONENTS >= 3 */

ML_SWIZZLE4(y, y, y, x)
ML_SWIZZLE4(y, y, y, y)

#if ML_SWIZZLE_COMPONENTS >= 3
ML_SWIZZLE4(y, y, y, z)
#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE4(y, y, y, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */
#endif     /* ML_SWIZZLE_COMPONENTS >= 3 */

#if ML_SWIZZLE_COMPONENTS >= 3

ML_SWIZZLE4(y, y, z, x)
ML_SWIZZLE4(y, y, z, y)
ML_SWIZZLE4(y, y, z, z)

#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE4(y, y, z, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */

#endif /* ML_SWIZZLE_COMPONENTS >= 3 */

#if ML_SWIZZLE_COMPONENTS == 4

ML_SWIZZLE4(y, y, w, x)
ML_SWIZZLE4(y, y, w, y)
ML_SWIZZLE4(y, y, w, z)
ML_SWIZZLE4(y, y, w, w)

#endif /* ML_SWIZZLE_COMPONENTS == 4 */

/* yz** */

#if ML_SWIZZLE_COMPONENTS >= 3

ML_SWIZZLE4(y, z, x, x)
ML_SWIZZLE4(y, z, x, y)
ML_SWIZZLE4(y, z, x, z)

#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE4(y, z, x, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */

ML_SWIZZLE4(y, z, y, x)
ML_SWIZZLE4(y, z, y, y)
ML_SWIZZLE4(y, z, y, z)

#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE4(y, z, y, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */

ML_SWIZZLE4(y, z, z, x)
ML_SWIZZLE4(y, z, z, y)
ML_SWIZZLE4(y, z, z, z)

#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE4(y, z, z, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */

#    if ML_SWIZZLE_COMPONENTS == 4

ML_SWIZZLE4(y, z, w, x)
ML_SWIZZLE4(y, z, w, y)
ML_SWIZZLE4(y, z, w, z)
ML_SWIZZLE4(y, z, w, w)

#    endif /* ML_SWIZZLE_COMPONENTS == 4 */

#endif /* ML_SWIZZLE_COMPONENTS >= 3 */

/* yw** */

#if ML_SWIZZLE_COMPONENTS == 4

ML_SWIZZLE4(y, w, x, x)
ML_SWIZZLE4(y, w, x, y)
ML_SWIZZLE4(y, w, x, z)
ML_SWIZZLE4(y, w, x, w)

ML_SWIZZLE4(y, w, y, x)
ML_SWIZZLE4(y, w, y, y)
ML_SWIZZLE4(y, w, y, z)
ML_SWIZZLE4(y, w, y, w)

ML_SWIZZLE4(y, w, z, x)
ML_SWIZZLE4(y, w, z, y)
ML_SWIZZLE4(y, w, z, z)
ML_SWIZZLE4(y, w, z, w)

ML_SWIZZLE4(y, w, w, x)
ML_SWIZZLE4(y, w, w, y)
ML_SWIZZLE4(y, w, w, z)
ML_SWIZZLE4(y, w, w, w)

#endif /* ML_SWIZZLE_COMPONENTS == 4 */

/* zx** */

#if ML_SWIZZLE_COMPONENTS >= 3

ML_SWIZZLE4(z, x, x, x)
ML_SWIZZLE4(z, x, x, y)
ML_SWIZZLE4(z, x, x, z)

#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE4(z, x, x, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */

ML_SWIZZLE4(z, x, y, x)
ML_SWIZZLE4(z, x, y, y)
ML_SWIZZLE4(z, x, y, z)

#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE4(z, x, y, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */

ML_SWIZZLE4(z, x, z, x)
ML_SWIZZLE4(z, x, z, y)
ML_SWIZZLE4(z, x, z, z)

#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE4(z, x, z, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */

#    if ML_SWIZZLE_COMPONENTS == 4

ML_SWIZZLE4(z, x, w, x)
ML_SWIZZLE4(z, x, w, y)
ML_SWIZZLE4(z, x, w, z)
ML_SWIZZLE4(z, x, w, w)

#    endif /* ML_SWIZZLE_COMPONENTS == 4 */

/* zy** */

ML_SWIZZLE4(z, y, x, x)
ML_SWIZZLE4(z, y, x, y)
ML_SWIZZLE4(z, y, x, z)

#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE4(z, y, x, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */

ML_SWIZZLE4(z, y, y, x)
ML_SWIZZLE4(z, y, y, y)
ML_SWIZZLE4(z, y, y, z)

#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE4(z, y, y, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */

ML_SWIZZLE4(z, y, z, x)
ML_SWIZZLE4(z, y, z, y)
ML_SWIZZLE4(z, y, z, z)

#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE4(z, y, z, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */

#    if ML_SWIZZLE_COMPONENTS == 4

ML_SWIZZLE4(z, y, w, x)
ML_SWIZZLE4(z, y, w, y)
ML_SWIZZLE4(z, y, w, z)
ML_SWIZZLE4(z, y, w, w)

#    endif /* ML_SWIZZLE_COMPONENTS == 4 */

/* zz** */

ML_SWIZZLE4(z, z, x, x)
ML_SWIZZLE4(z, z, x, y)
ML_SWIZZLE4(z, z, x, z)

#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE4(z, z, x, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */

ML_SWIZZLE4(z, z, y, x)
ML_SWIZZLE4(z, z, y, y)
ML_SWIZZLE4(z, z, y, z)

#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE4(z, z, y, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */

ML_SWIZZLE4(z, z, z, x)
ML_SWIZZLE4(z, z, z, y)
ML_SWIZZLE4(z, z, z, z)

#    if ML_SWIZZLE_COMPONENTS == 4
ML_SWIZZLE4(z, z, z, w)
#    endif /* ML_SWIZZLE_COMPONENTS == 4 */

#    if ML_SWIZZLE_COMPONENTS == 4

ML_SWIZZLE4(z, z, w, x)
ML_SWIZZLE4(z, z, w, y)
ML_SWIZZLE4(z, z, w, z)
ML_SWIZZLE4(z, z, w, w)

#    endif /* ML_SWIZZLE_COMPONENTS == 4 */

/* zw** */

#    if ML_SWIZZLE_COMPONENTS == 4

ML_SWIZZLE4(z, w, x, x)
ML_SWIZZLE4(z, w, x, y)
ML_SWIZZLE4(z, w, x, z)
ML_SWIZZLE4(z, w, x, w)

ML_SWIZZLE4(z, w, y, x)
ML_SWIZZLE4(z, w, y, y)
ML_SWIZZLE4(z, w, y, z)
ML_SWIZZLE4(z, w, y, w)

ML_SWIZZLE4(z, w, z, x)
ML_SWIZZLE4(z, w, z, y)
ML_SWIZZLE4(z, w, z, z)
ML_SWIZZLE4(z, w, z, w)

ML_SWIZZLE4(z, w, w, x)
ML_SWIZZLE4(z, w, w, y)
ML_SWIZZLE4(z, w, w, z)
ML_SWIZZLE4(z, w, w, w)

#    endif /* ML_SWIZZLE_COMPONENTS == 4 */

#endif /* ML_SWIZZLE_COMPONENTS >= 3 */

#if ML_SWIZZLE_COMPONENTS == 4

/* wx** */

ML_SWIZZLE4(w, x, x, x)
ML_SWIZZLE4(w, x, x, y)
ML_SWIZZLE4(w, x, x, z)
ML_SWIZZLE4(w, x, x, w)

ML_SWIZZLE4(w, x, y, x)
ML_SWIZZLE4(w, x, y, y)
ML_SWIZZLE4(w, x, y, z)
ML_SWIZZLE4(w, x, y, w)

ML_SWIZZLE4(w, x, z, x)
ML_SWIZZLE4(w, x, z, y)
ML_SWIZZLE4(w, x, z, z)
ML_SWIZZLE4(w, x, z, w)

ML_SWIZZLE4(w, x, w, x)
ML_SWIZZLE4(w, x, w, y)
ML_SWIZZLE4(w, x, w, z)
ML_SWIZZLE4(w, x, w, w)

/* wy** */

ML_SWIZZLE4(w, y, x, x)
ML_SWIZZLE4(w, y, x, y)
ML_SWIZZLE4(w, y, x, z)
ML_SWIZZLE4(w, y, x, w)

ML_SWIZZLE4(w, y, y, x)
ML_SWIZZLE4(w, y, y, y)
ML_SWIZZLE4(w, y, y, z)
ML_SWIZZLE4(w, y, y, w)

ML_SWIZZLE4(w, y, z, x)
ML_SWIZZLE4(w, y, z, y)
ML_SWIZZLE4(w, y, z, z)
ML_SWIZZLE4(w, y, z, w)

ML_SWIZZLE4(w, y, w, x)
ML_SWIZZLE4(w, y, w, y)
ML_SWIZZLE4(w, y, w, z)
ML_SWIZZLE4(w, y, w, w)

/* wz** */

ML_SWIZZLE4(w, z, x, x)
ML_SWIZZLE4(w, z, x, y)
ML_SWIZZLE4(w, z, x, z)
ML_SWIZZLE4(w, z, x, w)

ML_SWIZZLE4(w, z, y, x)
ML_SWIZZLE4(w, z, y, y)
ML_SWIZZLE4(w, z, y, z)
ML_SWIZZLE4(w, z, y, w)

ML_SWIZZLE4(w, z, z, x)
ML_SWIZZLE4(w, z, z, y)
ML_SWIZZLE4(w, z, z, z)
ML_SWIZZLE4(w, z, z, w)

ML_SWIZZLE4(w, z, w, x)
ML_SWIZZLE4(w, z, w, y)
ML_SWIZZLE4(w, z, w, z)
ML_SWIZZLE4(w, z, w, w)

/* ww** */

ML_SWIZZLE4(w, w, x, x)
ML_SWIZZLE4(w, w, x, y)
ML_SWIZZLE4(w, w, x, z)
ML_SWIZZLE4(w, w, x, w)

ML_SWIZZLE4(w, w, y, x)
ML_SWIZZLE4(w, w, y, y)
ML_SWIZZLE4(w, w, y, z)
ML_SWIZZLE4(w, w, y, w)

ML_SWIZZLE4(w, w, z, x)
ML_SWIZZLE4(w, w, z, y)
ML_SWIZZLE4(w, w, z, z)
ML_SWIZZLE4(w, w, z, w)

ML_SWIZZLE4(w, w, w, x)
ML_SWIZZLE4(w, w, w, y)
ML_SWIZZLE4(w, w, w, z)
ML_SWIZZLE4(w, w, w, w)

#endif /* ML_SWIZZLE_COMPONENTS == 4 */

#undef ML_SWIZZLE4
