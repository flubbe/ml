/* boost test framework. */
#define BOOST_TEST_MAIN
#define BOOST_TEST_ALTERNATIVE_INIT_API
#define BOOST_TEST_MODULE math library test
#include <boost/test/unit_test.hpp>

/* user headers. */
#include "ml/all.h"

BOOST_AUTO_TEST_SUITE(vec2)

BOOST_AUTO_TEST_CASE(area)
{
    ml::vec2 v1{1, 0}, v2{0, 1};
    BOOST_TEST(v1.area(v2) == 1.0f);

    ml::vec2 v3{std::cosf(M_PI / 4), std::sinf(M_PI / 4)};
    ml::vec2 v4{std::cosf(M_PI / 4 + M_PI / 2), std::sinf(M_PI / 4 + M_PI / 2)};
    BOOST_TEST(v3.area(v4) == 1.0f, boost::test_tools::tolerance(1e-6f));
}

BOOST_AUTO_TEST_SUITE_END()
