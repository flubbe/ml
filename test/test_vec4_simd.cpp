/* C++ headers */
#include <random>

/* boost test framework. */
#define BOOST_TEST_MAIN
#define BOOST_TEST_ALTERNATIVE_INIT_API
#define BOOST_TEST_MODULE math library test
#include <boost/test/unit_test.hpp>

// enable loading of SIMD types.
#define ML_USE_SIMD

/* user headers. */
#include "ml/all.h"

BOOST_AUTO_TEST_SUITE(vec4_simd)

#if !defined(ML_SIMD_NEON) && !defined(ML_SIMD_SSE)
#    error NEON or SSe needs to be enabled for these tests.
#endif

// paranoid checks.
static_assert(sizeof(ml::simd::vec4) == 4 * sizeof(float));

BOOST_AUTO_TEST_CASE(initialization)
{
    ml::simd::vec4 v1{1, 2, 3, 4};
    ml::simd::vec4 v2{1, 2, 3};
    ml::simd::vec4 v3{ml::vec3{1, 2, 3}, 4};

    BOOST_TEST((v1.x == 1 && v1.y == 2 && v1.z == 3 && v1.w == 4));
    BOOST_TEST((v2.x == 1 && v2.y == 2 && v2.z == 3 && v2.w == 1)); /* w-component defaults to one if not specified. */
    BOOST_TEST((v3.x == 1 && v3.y == 2 && v3.z == 3 && v3.w == 4));

    float v[4] = {1, 2, 3, 4};
    ml::simd::vec4 v4{v};
    ml::scalar::vec4 v5{v};

    BOOST_TEST((v4.x == 1 && v4.y == 2 && v4.z == 3 && v4.w == 4));
    BOOST_TEST((v5.x == 1 && v5.y == 2 && v5.z == 3 && v5.w == 4));
}

BOOST_AUTO_TEST_CASE(is_zero)
{
    BOOST_TEST(!ml::simd::vec4(0, 0, 0, 1).is_zero());
    BOOST_TEST(ml::simd::vec4(0, 0, 0, 0).is_zero());
}

BOOST_AUTO_TEST_CASE(comparisons)
{
    BOOST_TEST((ml::simd::vec4(1, 2, 3, 4) == ml::simd::vec4(1, 2, 3, 4)));
    BOOST_TEST(!(ml::simd::vec4(1, 2, 3, 4) == ml::simd::vec4(1, 2, 3, 0)));
    BOOST_TEST(!(ml::simd::vec4(1, 2, 3, 4) != ml::simd::vec4(1, 2, 3, 4)));
    BOOST_TEST((ml::simd::vec4(1, 2, 3, 4) != ml::simd::vec4(1, 2, 3, 0)));
}

BOOST_AUTO_TEST_CASE(component_product)
{
    BOOST_TEST((ml::simd::vec4(1, 2, 3, 4) * ml::simd::vec4(4, 3, 2, 1) == ml::simd::vec4(4, 6, 6, 4)));
    BOOST_TEST((ml::simd::vec4(0, 1, 0, 1) * ml::simd::vec4(-1, 0, -1, 0) == ml::simd::vec4(0, 0, 0, 0)));
    BOOST_TEST((ml::simd::vec4(-2, -3, 3, 2) * ml::simd::vec4(-1, 0, 4, -4) == ml::simd::vec4(2, 0, 12, -8)));
}

BOOST_AUTO_TEST_CASE(dot_product)
{
    BOOST_TEST(ml::dot(ml::simd::vec4(1, -1, 1, -1), ml::simd::vec4(1, -1, 1, -1)) == 4);
    BOOST_TEST(ml::dot(ml::simd::vec4(1, 2, 3, 4), ml::simd::vec4(4, 3, 2, 1)) == 20);
    BOOST_TEST(ml::dot(ml::simd::vec4(1, 2, 3, 4), ml::simd::vec4(-2, 1, -4, 3)) == 0);
}

BOOST_AUTO_TEST_SUITE_END()
