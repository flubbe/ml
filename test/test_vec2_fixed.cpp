/* boost test framework. */
#define BOOST_TEST_MAIN
#define BOOST_TEST_ALTERNATIVE_INIT_API
#define BOOST_TEST_MODULE math library test
#include <boost/test/unit_test.hpp>

/* user headers. */
#include "ml/all.h"

BOOST_AUTO_TEST_SUITE(vec2_fixed)

BOOST_AUTO_TEST_CASE(area)
{
    const ml::vec2_fixed<8> v1{1, 0}, v2{0, 1};
    BOOST_TEST(v1.area(v2) == ml::vec2_fixed<8>::type{1});
    BOOST_TEST(v2.area(v1) == ml::vec2_fixed<8>::type{-1});
    BOOST_TEST(v1.area(v1) == ml::vec2_fixed<8>::type{0});
    BOOST_TEST(v2.area(v2) == ml::vec2_fixed<8>::type{0});
}

BOOST_AUTO_TEST_SUITE_END()
