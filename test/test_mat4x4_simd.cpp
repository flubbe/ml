/* C++ headers */
#include <random>

/* boost test framework. */
#define BOOST_TEST_MAIN
#define BOOST_TEST_ALTERNATIVE_INIT_API
#define BOOST_TEST_MODULE math library test
#include <boost/test/unit_test.hpp>

/* user headers. */
#include "ml/all.h"
#include "utils.h"

BOOST_AUTO_TEST_SUITE(mat4x4_simd)

// compare non-simd and simd vec4
bool operator==(
  const ml::scalar::vec4& v1,
  const ml::simd::vec4& v2)
{
    return v1.x == v2.x
           && v1.y == v2.y
           && v1.z == v2.z
           && v1.w == v2.w;
}

// compare non-simd and simd mat4x4
bool operator==(
  const ml::scalar::mat4x4& m1,
  const ml::simd::mat4x4& m2)
{
    return m1.rows[0] == m2.rows[0]
           && m1.rows[1] == m2.rows[1]
           && m1.rows[2] == m2.rows[2]
           && m1.rows[3] == m2.rows[3];
}

// initialize simd vec4 from non-simd vec4
ml::simd::vec4 vec_simd_init(
  const ml::scalar::vec4& v)
{
    return {v.x, v.y, v.z, v.w};
}

// initialize simd mat4x4 from non-simd mat4x4
ml::simd::mat4x4 mat_simd_init(
  const ml::scalar::mat4x4& m)
{
    return {vec_simd_init(m.rows[0]),
            vec_simd_init(m.rows[1]),
            vec_simd_init(m.rows[2]),
            vec_simd_init(m.rows[3])};
}

BOOST_AUTO_TEST_CASE(multiplication)
{
    ml::scalar::mat4x4 m1{
      {1, 2, 3, 4},
      {2, 4, 3, 1},
      {3, 1, 4, 2},
      {4, 2, 1, 3}};
    m1 = m1 * m1;

    ml::simd::mat4x4 m2{
      {1, 2, 3, 4},
      {2, 4, 3, 1},
      {3, 1, 4, 2},
      {4, 2, 1, 3}};
    m2 = m2 * m2;

    BOOST_TEST((m1.rows[0] == m2.rows[0]));
    BOOST_TEST((m1.rows[1] == m2.rows[1]));
    BOOST_TEST((m1.rows[2] == m2.rows[2]));
    BOOST_TEST((m1.rows[3] == m2.rows[3]));

    m1.transpose();
    m2.transpose();

    // for better tracking in case of test failure.
    auto m1_t = m1;
    auto m2_t = m2;
    BOOST_TEST((m1_t.rows[0] == m2_t.rows[0]));
    BOOST_TEST((m1_t.rows[1] == m2_t.rows[1]));
    BOOST_TEST((m1_t.rows[2] == m2_t.rows[2]));
    BOOST_TEST((m1_t.rows[3] == m2_t.rows[3]));
}

template<typename T>
T get_random_vec4()
{
    return {static_cast<float>(rand() % 9 - 5),
            static_cast<float>(rand() % 9 - 5),
            static_cast<float>(rand() % 9 - 5),
            static_cast<float>(rand() % 9 + 1)};
}

template<typename M, typename T>
M get_random_mat()
{
    return {
      get_random_vec4<T>(),
      get_random_vec4<T>(),
      get_random_vec4<T>(),
      get_random_vec4<T>()};
}

BOOST_AUTO_TEST_CASE(randomized_multiplication)
{
    // random multiplication tests.
    for(int i = 0; i < 1000; ++i)
    {
        ml::scalar::mat4x4 rm1 = get_random_mat<ml::scalar::mat4x4, ml::scalar::vec4>();
        ml::scalar::mat4x4 rm2 = get_random_mat<ml::scalar::mat4x4, ml::scalar::vec4>();

        ml::simd::mat4x4 rm1_simd = mat_simd_init(rm1), rm2_simd = mat_simd_init(rm2);

        ml::scalar::mat4x4 res = rm1 * rm2;
        ml::simd::mat4x4 res_simd = rm1_simd * rm2_simd;

        BOOST_REQUIRE(res.rows[0] == res_simd.rows[0]);
        BOOST_REQUIRE(res.rows[1] == res_simd.rows[1]);
        BOOST_REQUIRE(res.rows[2] == res_simd.rows[2]);
        BOOST_REQUIRE(res.rows[3] == res_simd.rows[3]);
    }
}

BOOST_AUTO_TEST_CASE(determinant)
{
    ml::simd::mat4x4 m = ml::simd::mat4x4::zero();
    BOOST_CHECK_EQUAL(m.determinant(), 0.f);

    m = ml::simd::mat4x4::one();
    BOOST_CHECK_EQUAL(m.determinant(), 0.f);

    m = ml::simd::mat4x4::identity();
    BOOST_CHECK_EQUAL(m.determinant(), 1.f);

    m[1][1] = 2;
    m[1][2] = 1;
    m[2][2] = 3;
    m[2][3] = 1;
    m[3][3] = 4;
    BOOST_CHECK_EQUAL(m.determinant(), 24.f);
}

BOOST_AUTO_TEST_CASE(inverse)
{
    BOOST_TEST(!ml::simd::mat4x4::zero().inverse().has_value());
    BOOST_TEST(!ml::simd::mat4x4::one().inverse().has_value());
    BOOST_TEST(ml::simd::mat4x4::identity().inverse().has_value());

    BOOST_CHECK_EQUAL(ml::simd::mat4x4::identity().inverse(), ml::simd::mat4x4::identity());

    auto m = ml::simd::mat4x4{
      {0, 1, 0, 0},
      {1, 0, 0, 0},
      {0, 0, 0, 1},
      {0, 0, 1, 0}};
    BOOST_CHECK_EQUAL(m.inverse(), m);

    m *= 2.f;
    BOOST_CHECK_EQUAL(m.inverse(), 0.25f * m);
}

BOOST_AUTO_TEST_SUITE_END()
