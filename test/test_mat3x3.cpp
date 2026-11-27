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

BOOST_AUTO_TEST_SUITE(mat3x3)

/*
 * mat3x3 tests.
 */

BOOST_AUTO_TEST_CASE(initialization)
{
    auto m = ml::mat3x3::zero();
    BOOST_TEST((
      m.rows[0].x == 0 && m.rows[0].y == 0 && m.rows[0].z == 0
      && m.rows[1].x == 0 && m.rows[1].y == 0 && m.rows[1].z == 0
      && m.rows[2].x == 0 && m.rows[2].y == 0 && m.rows[2].z == 0));

    m = ml::mat3x3::one();
    BOOST_TEST((
      m.rows[0].x == 1 && m.rows[0].y == 1 && m.rows[0].z == 1
      && m.rows[1].x == 1 && m.rows[1].y == 1 && m.rows[1].z == 1
      && m.rows[2].x == 1 && m.rows[2].y == 1 && m.rows[2].z == 1));

    m = ml::mat3x3::identity();
    BOOST_TEST((
      m.rows[0].x == 1 && m.rows[0].y == 0 && m.rows[0].z == 0
      && m.rows[1].x == 0 && m.rows[1].y == 1 && m.rows[1].z == 0
      && m.rows[2].x == 0 && m.rows[2].y == 0 && m.rows[2].z == 1));
}

BOOST_AUTO_TEST_CASE(transpose)
{
    auto m = ml::mat3x3{
      {1, 2, 3},
      {4, 5, 6},
      {7, 8, 9}}
               .transposed();
    BOOST_TEST((
      m.rows[0].x == 1 && m.rows[0].y == 4 && m.rows[0].z == 7
      && m.rows[1].x == 2 && m.rows[1].y == 5 && m.rows[1].z == 8
      && m.rows[2].x == 3 && m.rows[2].y == 6 && m.rows[2].z == 9));

    m = ml::mat3x3{
      {1, 2, 3},
      {4, 5, 6},
      {7, 8, 9}};
    m.transpose();
    BOOST_TEST((
      m.rows[0].x == 1 && m.rows[0].y == 4 && m.rows[0].z == 7
      && m.rows[1].x == 2 && m.rows[1].y == 5 && m.rows[1].z == 8
      && m.rows[2].x == 3 && m.rows[2].y == 6 && m.rows[2].z == 9));
}

BOOST_AUTO_TEST_CASE(multiplication)
{
    auto m = ml::mat3x3{
               {1, 2, 3},
               {4, 5, 6},
               {7, 8, 9}}
             * ml::mat3x3::zero();
    BOOST_TEST((
      m.rows[0].x == 0 && m.rows[0].y == 0 && m.rows[0].z == 0
      && m.rows[1].x == 0 && m.rows[1].y == 0 && m.rows[1].z == 0
      && m.rows[2].x == 0 && m.rows[2].y == 0 && m.rows[2].z == 0));

    m = ml::mat3x3{
          {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}}
        * ml::mat3x3::identity();
    BOOST_TEST((
      m.rows[0].x == 1 && m.rows[0].y == 2 && m.rows[0].z == 3
      && m.rows[1].x == 4 && m.rows[1].y == 5 && m.rows[1].z == 6
      && m.rows[2].x == 7 && m.rows[2].y == 8 && m.rows[2].z == 9));
}

BOOST_AUTO_TEST_CASE(determinant)
{
    BOOST_CHECK_CLOSE(ml::mat3x3::zero().determinant(), 0, 1e-6f);
    BOOST_CHECK_CLOSE(ml::mat3x3::one().determinant(), 0, 1e-6f);
    BOOST_CHECK_CLOSE(ml::mat3x3::identity().determinant(), 1, 1e-6f);

    auto m = ml::mat3x3{
      {0, 1, 0},
      {1, 0, 0},
      {0, 0, 1}};
    BOOST_CHECK_CLOSE(m.determinant(), -1, 1e-6f);

    m = ml::mat3x3{
      {1, 2, 3},
      {4, 5, 6},
      {7, 8, 9}};
    BOOST_CHECK_CLOSE(m.determinant(), 0, 1e-6f);
}

BOOST_AUTO_TEST_CASE(inverse)
{
    BOOST_TEST(!ml::mat3x3::zero().inverse().has_value());
    BOOST_TEST(!ml::mat3x3::one().inverse().has_value());
    BOOST_TEST(ml::mat3x3::identity().inverse().has_value());

    BOOST_CHECK_EQUAL(ml::mat3x3::identity().inverse(), ml::mat3x3::identity());

    auto m = ml::mat3x3{
      {0, 1, 0},
      {1, 0, 0},
      {0, 0, 1}};
    BOOST_CHECK_EQUAL(m.inverse(), m);
}

BOOST_AUTO_TEST_SUITE_END()
