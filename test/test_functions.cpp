/* boost test framework. */
#define BOOST_TEST_MAIN
#define BOOST_TEST_ALTERNATIVE_INIT_API
#define BOOST_TEST_MODULE math library test
#include <boost/test/unit_test.hpp>

/* user headers. */
#include "ml/all.h"

BOOST_AUTO_TEST_SUITE(functions)

BOOST_AUTO_TEST_CASE(lerp)
{
    BOOST_TEST(ml::lerp(0.3f, 1.f, 2.f) == static_cast<float>(1.f * (1.f - 0.3f) + 2.f * 0.3f), boost::test_tools::tolerance(1e-8f));
    BOOST_TEST(ml::lerp(0.3, 1.0, 2.0) == static_cast<double>(1.0 * (1.0 - 0.3) + 2.0 * 0.3), boost::test_tools::tolerance(1e-8));

    ml::vec4 a{1.1, 2.2, 3.3, 4.4};
    ml::vec4 b{-9.3, -10.4, -11.5, -12.6};
    ml::vec4 l{ml::lerp(0.4f, a, b)};
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

BOOST_AUTO_TEST_CASE(radians_degrees)
{
    /* these are just linear transformations.... */

    BOOST_TEST(ml::to_radians(0) == 0);
    BOOST_TEST(ml::to_radians(90) == static_cast<float>(M_PI_2), boost::test_tools::tolerance(1e-9f));
    BOOST_TEST(ml::to_radians(180) == static_cast<float>(M_PI), boost::test_tools::tolerance(1e-9f));
    BOOST_TEST(ml::to_radians(270) == static_cast<float>(3 * M_PI_2), boost::test_tools::tolerance(1e-9f));
    BOOST_TEST(ml::to_radians(360) == static_cast<float>(2 * M_PI), boost::test_tools::tolerance(1e-9f));
    BOOST_TEST(ml::to_radians(-90) == static_cast<float>(-M_PI_2), boost::test_tools::tolerance(1e-9f));
    BOOST_TEST(ml::to_radians(-180) == static_cast<float>(-M_PI), boost::test_tools::tolerance(1e-9f));
    BOOST_TEST(ml::to_radians(-270) == static_cast<float>(-3 * M_PI_2), boost::test_tools::tolerance(1e-9f));
    BOOST_TEST(ml::to_radians(-360) == static_cast<float>(-2 * M_PI), boost::test_tools::tolerance(1e-9f));

    BOOST_TEST(ml::to_degrees(0) == 0);
    BOOST_TEST(ml::to_degrees(static_cast<float>(M_PI_2)) == static_cast<float>(90), boost::test_tools::tolerance(1e-9f));
    BOOST_TEST(ml::to_degrees(static_cast<float>(M_PI)) == static_cast<float>(180), boost::test_tools::tolerance(1e-9f));
    BOOST_TEST(ml::to_degrees(static_cast<float>(3 * M_PI_2)) == static_cast<float>(270), boost::test_tools::tolerance(1e-9f));
    BOOST_TEST(ml::to_degrees(static_cast<float>(2 * M_PI)) == static_cast<float>(360), boost::test_tools::tolerance(1e-9f));
    BOOST_TEST(ml::to_degrees(static_cast<float>(-M_PI_2)) == static_cast<float>(-90), boost::test_tools::tolerance(1e-9f));
    BOOST_TEST(ml::to_degrees(static_cast<float>(-M_PI)) == static_cast<float>(-180), boost::test_tools::tolerance(1e-9f));
    BOOST_TEST(ml::to_degrees(static_cast<float>(-3 * M_PI_2)) == static_cast<float>(-270), boost::test_tools::tolerance(1e-9f));
    BOOST_TEST(ml::to_degrees(static_cast<float>(-2 * M_PI)) == static_cast<float>(-360), boost::test_tools::tolerance(1e-9f));

    for(int i = 0; i < 100; ++i)
    {
        BOOST_TEST(ml::to_radians(ml::to_degrees(static_cast<float>(i))) == static_cast<float>(i), boost::test_tools::tolerance(1e-6f));
        BOOST_TEST(ml::to_degrees(ml::to_radians(static_cast<float>(i))) == static_cast<float>(i), boost::test_tools::tolerance(1e-6f));
    }
}

BOOST_AUTO_TEST_SUITE_END()
