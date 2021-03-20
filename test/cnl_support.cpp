/* C++ headers */
#include <random>

/* boost test framework. */
#define BOOST_TEST_MAIN
#define BOOST_TEST_ALTERNATIVE_INIT_API
#define BOOST_TEST_MODULE cnl support functions test
#include <boost/test/unit_test.hpp>

/* user headers. */
#include "ml/all.h"

using namespace ml;
using namespace std;

BOOST_AUTO_TEST_SUITE(cnl_support)

/*
 * rounding.
 */

BOOST_AUTO_TEST_CASE(rounding)
{
    fixed_t fp1{0}, fp2{-1}, fp3{-2.9}, fp4{-10.1}, fp5{-14.5}, fp6{3.5}, fp7{0.5}, fp8{0.7}, fp9{10}, fp10{23.4};

    BOOST_CHECK(round(fixed_t{0}) == std::round(0));
    BOOST_CHECK(round(fixed_t{-1}) == std::round(-1));
    BOOST_CHECK(round(fixed_t{-2.9}) == std::round(-2.9));
    BOOST_CHECK(round(fixed_t{-10.1}) == std::round(-10.1));
    BOOST_CHECK(round(fixed_t{-14.5}) == std::round(-14.5));
    BOOST_CHECK(round(fixed_t{3.5}) == std::round(3.5));
    BOOST_CHECK(round(fixed_t{0.5}) == std::round(0.5));
    BOOST_CHECK(round(fixed_t{0.7}) == std::round(0.7));
    BOOST_CHECK(round(fixed_t{10}) == std::round(10));
    BOOST_CHECK(round(fixed_t{23.4}) == std::round(23.4));
}

BOOST_AUTO_TEST_CASE(randomized_rounding)
{
    std::random_device rnd_device;
    std::mt19937 mersenne_engine{rnd_device()};
    std::uniform_real_distribution<float> dist{-1000, 1000};

    auto gen = [&dist, &mersenne_engine]() { return dist(mersenne_engine); };

    for(int i = 0; i < 10000; ++i)
    {
        auto f = gen();
        BOOST_CHECK(round(fixed_t{f}) == std::round(f));
    }
}

/*
 * to_float conversion.
 */

/* 2^(-16) */
#define TOLERANCE (0.0000152587890625f)

BOOST_AUTO_TEST_CASE(to_float, *boost::unit_test::tolerance(TOLERANCE))
{
    fixed_t fp1{0}, fp2{-1}, fp3{-2.9}, fp4{-10.1}, fp5{-14.5}, fp6{3.5}, fp7{0.5}, fp8{0.7}, fp9{10}, fp10{23.4};

    BOOST_TEST(ml::to_float(fixed_t{0}) == 0.0f);
    BOOST_TEST(ml::to_float(fixed_t{-1}) == -1.0f);
    BOOST_TEST(ml::to_float(fixed_t{-2.9}) == -2.9f);
    BOOST_TEST(ml::to_float(fixed_t{-10.1}) == -10.1f);
    BOOST_TEST(ml::to_float(fixed_t{-14.5}) == -14.5f);
    BOOST_TEST(ml::to_float(fixed_t{3.5}) == 3.5f);
    BOOST_TEST(ml::to_float(fixed_t{0.5}) == 0.5f);
    BOOST_TEST(ml::to_float(fixed_t{0.7}) == 0.7f);
    BOOST_TEST(ml::to_float(fixed_t{10}) == 10.0f);
    BOOST_TEST(ml::to_float(fixed_t{23.4}) == 23.4f);
}

#undef TOLERANCE

/*
 * integral part extraction.
 */

BOOST_AUTO_TEST_CASE(integral_part)
{
    fixed_t fp1{0}, fp2{-1}, fp3{-2.9}, fp4{-10.1}, fp5{-14.5}, fp6{3.5}, fp7{0.5}, fp8{0.7}, fp9{10}, fp10{23.4};

    BOOST_TEST(ml::integral_part(fixed_t{0}) == 0);
    BOOST_TEST(ml::integral_part(fixed_t{-1}) == -1);
    BOOST_TEST(ml::integral_part(fixed_t{-2.9}) == -2);
    BOOST_TEST(ml::integral_part(fixed_t{-10.1}) == -10);
    BOOST_TEST(ml::integral_part(fixed_t{-14.5}) == -14);
    BOOST_TEST(ml::integral_part(fixed_t{3.5}) == 3);
    BOOST_TEST(ml::integral_part(fixed_t{0.5}) == 0);
    BOOST_TEST(ml::integral_part(fixed_t{0.7}) == 0);
    BOOST_TEST(ml::integral_part(fixed_t{10}) == 10);
    BOOST_TEST(ml::integral_part(fixed_t{23.4}) == 23);
}

/*
 * number truncation.
 */

BOOST_AUTO_TEST_CASE(fixed_point_truncate)
{
    fixed_24_8_t fp1{1.2345};
    fixed_24_8_t fp2{ml::truncate_unchecked<4>(fp1)};

    BOOST_TEST((cnl::unwrap(fp1) & 0xF) != 0);
    BOOST_TEST((cnl::unwrap(fp2) & 0xF) == 0);
}

/*
 * fixed-point to fixed-point type conversions.
 */

BOOST_AUTO_TEST_CASE(fixed_point_conversion)
{
    fixed_28_4_t fp1{1.2345};
    fixed_24_8_t fp2{fp1};

    BOOST_TEST(ml::to_float(fp1) == ml::to_float(fp2));
}

BOOST_AUTO_TEST_CASE(fixed_point_vec2_conversions)
{
    ml::vec2_fixed<4> v1{1.2345f, 2.3456f};
    ml::vec2_fixed<8> v2{v1.x, v1.y};

    BOOST_TEST(ml::to_float(v1.x) == ml::to_float(v2.x));
    BOOST_TEST(ml::to_float(v1.y) == ml::to_float(v2.y));
}

/*
 * fixed-point comparisons.
 */

BOOST_AUTO_TEST_CASE(fixed_point_comparison)
{
    BOOST_TEST(fixed_28_4_t{1.4832} > 0);
    BOOST_TEST(fixed_24_8_t{1.4832} > 0);
    BOOST_TEST(fixed_t{1.4832} > 0);

    BOOST_TEST(fixed_28_4_t{-10.22} < 0);
    BOOST_TEST(fixed_24_8_t{-12.92} < 0);
    BOOST_TEST(fixed_t{-8289.23} < 0);
}

/*
 * check type ranges
 */

BOOST_AUTO_TEST_CASE(type_ranges)
{
    /* note: need to take care of automatic type conversion */
    BOOST_TEST(ml::fixed_24_8_t{ml::fixed_24_8_t{300} * ml::fixed_24_8_t{200}} == 60000);
    BOOST_TEST(ml::fixed_24_8_t{ml::fixed_24_8_t{-300} * ml::fixed_24_8_t{200}} == -60000);
    BOOST_TEST(ml::fixed_24_8_t{ml::fixed_24_8_t{300} * ml::fixed_24_8_t{-200}} == -60000);
    BOOST_TEST(ml::fixed_24_8_t{ml::fixed_24_8_t{-300} * ml::fixed_24_8_t{-200}} == 60000);
}

BOOST_AUTO_TEST_SUITE_END()
