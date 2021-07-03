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

/*
 * Helpers.
 */

using namespace ml;

void random_initialize_real_std_vector(std::size_t n, std::vector<vec4>& v)
{
    std::random_device rnd_device;
    std::mt19937 mersenne_engine{rnd_device()};
    std::uniform_real_distribution<float> dist{-1, 1};

    auto gen = [&dist, &mersenne_engine]() {
        return vec4{dist(mersenne_engine), dist(mersenne_engine), dist(mersenne_engine), dist(mersenne_engine)};
    };

    v.resize(n);
    generate(std::begin(v), std::end(v), gen);
}

/*
 * memory alignment.
 */
inline bool
  is_aligned(const void* ptr, std::uintptr_t alignment) noexcept
{
    auto iptr = reinterpret_cast<std::uintptr_t>(ptr);
    return !(iptr % alignment);
}

BOOST_AUTO_TEST_SUITE(math)

/*
 * vec2 tests.
 */

BOOST_AUTO_TEST_CASE(vec2)
{
    ml::vec2 v1{1, 0}, v2{0, 1};
    BOOST_TEST(v1.area(v2) == 1.0f);

    ml::vec2 v3{std::cos(M_PI / 4), std::sin(M_PI / 4)};
    ml::vec2 v4{std::cos(M_PI / 4 + M_PI / 2), std::sin(M_PI / 4 + M_PI / 2)};
    BOOST_TEST(v3.area(v4) == 1.0f, boost::test_tools::tolerance(1e-6f));
}

/*
 * tvec2 tests.
 */

BOOST_AUTO_TEST_CASE(tvec2)
{
    ml::tvec2<ml::fixed_24_8_t> v = {1.23, 2.24};
    BOOST_TEST(ml::to_float(v.x) == 1.23f, boost::test_tools::tolerance(4e-3f));
    BOOST_TEST(ml::to_float(v.y) == 2.24f, boost::test_tools::tolerance(4e-3f));
}

/*
 * vec2_fixed tests.
 */

BOOST_AUTO_TEST_CASE(vec2_fixed)
{
    const ml::vec2_fixed<8> v1{1, 0}, v2{0, 1};
    BOOST_TEST(v1.area(v2) == ml::vec2_fixed<8>::type{1});
    BOOST_TEST(v2.area(v1) == ml::vec2_fixed<8>::type{-1});
    BOOST_TEST(v1.area(v1) == ml::vec2_fixed<8>::type{0});
    BOOST_TEST(v2.area(v2) == ml::vec2_fixed<8>::type{0});
}

/*
 * vec4 tests.
 */

// check that we actually using both non-simd and simd types.
static_assert(!std::is_same<ml::vec4, ml::simd::vec4>::value);
static_assert(!std::is_same<ml::mat4x4, ml::simd::mat4x4>::value);

// paranoid checks.
static_assert(sizeof(vec4) == 4 * sizeof(float));
static_assert(sizeof(vec4) == sizeof(__m128));

BOOST_AUTO_TEST_CASE(vec4_alignment)
{
    std::vector<vec4> v;
    random_initialize_real_std_vector(10, v);

    // check vector alignment while being paranoid.
    BOOST_TEST_MESSAGE("Checking 16-byte vector alignment");
    for(size_t i = 0; i < v.size(); ++i)
    {
        BOOST_CHECK(is_aligned(&v[i], 16));
    }
}

BOOST_AUTO_TEST_CASE(vec4_simd_initialization)
{
    ml::simd::vec4 v1{1, 2, 3, 4};
    ml::simd::vec4 v2{1, 2, 3};
    ml::simd::vec4 v3{ml::vec3{1, 2, 3}, 4};

    BOOST_TEST((v1.x == 1 && v1.y == 2 && v1.z == 3 && v1.w == 4));
    BOOST_TEST((v2.x == 1 && v2.y == 2 && v2.z == 3 && v2.w == 1)); /* w-component defaults to one if not specified. */
    BOOST_TEST((v3.x == 1 && v3.y == 2 && v3.z == 3 && v3.w == 4));

    float v[4] = {1, 2, 3, 4};
    ml::simd::vec4 v4{v};
    ml::vec4 v5{v};
    
    BOOST_TEST((v4.x == 1 && v4.y == 2 && v4.z == 3 && v4.w == 4));
    BOOST_TEST((v5.x == 1 && v5.y == 2 && v5.z == 3 && v5.w == 4));
}

BOOST_AUTO_TEST_CASE(vec4_simd_comparisons)
{
    BOOST_TEST((ml::simd::vec4(1, 2, 3, 4) == ml::simd::vec4(1, 2, 3, 4)));
    BOOST_TEST(!(ml::simd::vec4(1, 2, 3, 4) == ml::simd::vec4(1, 2, 3, 0)));
    BOOST_TEST(!(ml::simd::vec4(1, 2, 3, 4) != ml::simd::vec4(1, 2, 3, 4)));
    BOOST_TEST((ml::simd::vec4(1, 2, 3, 4) != ml::simd::vec4(1, 2, 3, 0)));
}

BOOST_AUTO_TEST_CASE(vec4_component_product)
{
    BOOST_TEST((ml::vec4(1, 2, 3, 4) * ml::vec4(4, 3, 2, 1) == ml::vec4(4, 6, 6, 4)));
    BOOST_TEST((ml::vec4(0, 1, 0, 1) * ml::vec4(-1, 0, -1, 0) == ml::vec4(0, 0, 0, 0)));
    BOOST_TEST((ml::vec4(-2, -3, 3, 2) * ml::vec4(-1, 0, 4, -4) == ml::vec4(2, 0, 12, -8)));
}

BOOST_AUTO_TEST_CASE(vec4_simd_component_product)
{
    BOOST_TEST((ml::simd::vec4(1, 2, 3, 4) * ml::simd::vec4(4, 3, 2, 1) == ml::simd::vec4(4, 6, 6, 4)));
    BOOST_TEST((ml::simd::vec4(0, 1, 0, 1) * ml::simd::vec4(-1, 0, -1, 0) == ml::simd::vec4(0, 0, 0, 0)));
    BOOST_TEST((ml::simd::vec4(-2, -3, 3, 2) * ml::simd::vec4(-1, 0, 4, -4) == ml::simd::vec4(2, 0, 12, -8)));
}

BOOST_AUTO_TEST_CASE(vec4_dot)
{
    BOOST_TEST(ml::dot(ml::vec4(1, -1, 1, -1), ml::vec4(1, -1, 1, -1)) == 4);
    BOOST_TEST(ml::dot(ml::vec4(1, 2, 3, 4), ml::vec4(4, 3, 2, 1)) == 20);
    BOOST_TEST(ml::dot(ml::vec4(1, 2, 3, 4), ml::vec4(-2, 1, -4, 3)) == 0);
}

BOOST_AUTO_TEST_CASE(vec4_simd_dot)
{
    BOOST_TEST(ml::dot(ml::simd::vec4(1, -1, 1, -1), ml::simd::vec4(1, -1, 1, -1)) == 4);
    BOOST_TEST(ml::dot(ml::simd::vec4(1, 2, 3, 4), ml::simd::vec4(4, 3, 2, 1)) == 20);
    BOOST_TEST(ml::dot(ml::simd::vec4(1, 2, 3, 4), ml::simd::vec4(-2, 1, -4, 3)) == 0);
}

BOOST_AUTO_TEST_CASE(vec4_is_zero)
{
    BOOST_TEST(!ml::vec4(0,0,0,1).is_zero());
    BOOST_TEST(ml::vec4(0,0,0,0).is_zero());
}

BOOST_AUTO_TEST_CASE(vec4_simd_is_zero)
{
    BOOST_TEST(!ml::simd::vec4(0,0,0,1).is_zero());
    BOOST_TEST(ml::simd::vec4(0,0,0,0).is_zero());
}

/*
 * mat4x4 tests.
 */

// compare non-simd and simd vec4
bool operator==(const ml::vec4& v1, const ml::simd::vec4& v2)
{
    return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w;
}

// compare non-simd and simd mat4x4
bool operator==(const ml::mat4x4& m1, const ml::simd::mat4x4& m2)
{
    return m1.rows[0] == m2.rows[0] && m1.rows[1] == m2.rows[1] && m1.rows[2] == m2.rows[2] && m1.rows[3] == m2.rows[3];
}

// initialize simd vec4 from non-simd vec4
ml::simd::vec4 vec_simd_init(const ml::vec4& v)
{
    return {v.x, v.y, v.z, v.w};
}

// initialize simd mat4x4 from non-simd mat4x4
ml::simd::mat4x4 mat_simd_init(const ml::mat4x4& m)
{
    return {vec_simd_init(m.rows[0]), vec_simd_init(m.rows[1]), vec_simd_init(m.rows[2]), vec_simd_init(m.rows[3])};
}

BOOST_AUTO_TEST_CASE(mat4x4_multiplication)
{
    ml::mat4x4 m1{
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
    return {static_cast<float>(rand() % 9 - 5), static_cast<float>(rand() % 9 - 5), static_cast<float>(rand() % 9 - 5), static_cast<float>(rand() % 9 + 1)};
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

BOOST_AUTO_TEST_CASE(mat4x4_randomized_multiplication)
{
    // random multiplication tests.
    for(int i = 0; i < 1000; ++i)
    {
        ml::mat4x4 rm1 = get_random_mat<ml::mat4x4, ml::vec4>();
        ml::mat4x4 rm2 = get_random_mat<ml::mat4x4, ml::vec4>();

        ml::simd::mat4x4 rm1_simd = mat_simd_init(rm1), rm2_simd = mat_simd_init(rm2);

        ml::mat4x4 res = rm1 * rm2;
        ml::simd::mat4x4 res_simd = rm1_simd * rm2_simd;

        BOOST_REQUIRE(res.rows[0] == res_simd.rows[0]);
        BOOST_REQUIRE(res.rows[1] == res_simd.rows[1]);
        BOOST_REQUIRE(res.rows[2] == res_simd.rows[2]);
        BOOST_REQUIRE(res.rows[3] == res_simd.rows[3]);
    }
}

/*
 * math functions.
 */

BOOST_AUTO_TEST_CASE(lerp)
{
    BOOST_TEST(ml::lerp<float>(0.3f, 1.f, 2.f) == static_cast<float>(1.f * (1.f - 0.3f) + 2.f * 0.3f), boost::test_tools::tolerance(1e-8f));
    BOOST_TEST(ml::lerp<double>(0.3, 1, 2) == static_cast<double>(1.0 * (1.0 - 0.3) + 2.0 * 0.3), boost::test_tools::tolerance(1e-8));

    ml::vec4 a{1.1, 2.2, 3.3, 4.4};
    ml::vec4 b{-9.3, -10.4, -11.5, -12.6};
    ml::vec4 l{ml::lerp<ml::vec4>(0.4, a, b)};
    BOOST_TEST(l.x == -3.06f, boost::test_tools::tolerance(1e-6f));
    BOOST_TEST(l.y == -2.84f, boost::test_tools::tolerance(1e-6f));
    BOOST_TEST(l.z == -2.62f, boost::test_tools::tolerance(1e-6f));
    BOOST_TEST(l.w == -2.4f, boost::test_tools::tolerance(1e-6f));

    for(int i = 0; i < 100; ++i)
    {
        float t = static_cast<float>(i) / 100.f;

        l = ml::lerp(t, a, b);
        BOOST_TEST(l.x == ml::lerp(t, a.x, b.x), boost::test_tools::tolerance(1e-5f));
        BOOST_TEST(l.y == ml::lerp(t, a.y, b.y), boost::test_tools::tolerance(1e-5f));
        BOOST_TEST(l.z == ml::lerp(t, a.z, b.z), boost::test_tools::tolerance(1e-5f));
        BOOST_TEST(l.w == ml::lerp(t, a.w, b.w), boost::test_tools::tolerance(1e-5f));
    }
}

BOOST_AUTO_TEST_SUITE_END()
