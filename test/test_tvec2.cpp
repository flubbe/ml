/* boost test framework. */
#define BOOST_TEST_MAIN
#define BOOST_TEST_ALTERNATIVE_INIT_API
#define BOOST_TEST_MODULE math library test
#include <boost/test/unit_test.hpp>

/* user headers. */
#include "ml/all.h"

BOOST_AUTO_TEST_SUITE(tvec2)

/*
 * tvec2 tests.
 */

BOOST_AUTO_TEST_CASE(to_float)
{
    ml::tvec2<ml::fixed_24_8_t> v = {1.23, 2.24};
    BOOST_TEST(ml::to_float(v.x) == 1.23f, boost::test_tools::tolerance(4e-3f));
    BOOST_TEST(ml::to_float(v.y) == 2.24f, boost::test_tools::tolerance(4e-3f));
}

BOOST_AUTO_TEST_SUITE_END()
