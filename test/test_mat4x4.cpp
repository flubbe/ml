/* C++ headers */
#include <random>

/* boost test framework. */
#define BOOST_TEST_MAIN
#define BOOST_TEST_ALTERNATIVE_INIT_API
#define BOOST_TEST_MODULE math library test
#include <boost/test/unit_test.hpp>

// enable loading of SIMD types.
#define ML_INCLUDE_SIMD

/* user headers. */
#include "ml/all.h"
#include "utils.h"

BOOST_AUTO_TEST_SUITE(mat4x4)

BOOST_AUTO_TEST_CASE(mat4x4_determinant)
{
    ml::mat4x4 m = ml::mat4x4::zero();
    BOOST_CHECK_EQUAL(m.determinant(), 0.f);

    m = ml::mat4x4::one();
    BOOST_CHECK_EQUAL(m.determinant(), 0.f);

    m = ml::mat4x4::identity();
    BOOST_CHECK_EQUAL(m.determinant(), 1.f);

    m[1][1] = 2;
    m[1][2] = 1;
    m[2][2] = 3;
    m[2][3] = 1;
    m[3][3] = 4;
    BOOST_CHECK_EQUAL(m.determinant(), 24.f);
}

BOOST_AUTO_TEST_CASE(mat4x4_inverse)
{
    BOOST_TEST(!ml::mat4x4::zero().inverse().has_value());
    BOOST_TEST(!ml::mat4x4::one().inverse().has_value());
    BOOST_TEST(ml::mat4x4::identity().inverse().has_value());

    BOOST_CHECK_EQUAL(ml::mat4x4::identity().inverse(), ml::mat4x4::identity());

    auto m = ml::mat4x4{
      {0, 1, 0, 0},
      {1, 0, 0, 0},
      {0, 0, 0, 1},
      {0, 0, 1, 0}};
    BOOST_CHECK_EQUAL(m.inverse(), m);

    m *= 2.f;
    BOOST_CHECK_EQUAL(m.inverse(), 0.25f * m);
}

BOOST_AUTO_TEST_SUITE_END()
