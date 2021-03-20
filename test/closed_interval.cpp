/* boost test framework. */
#define BOOST_TEST_MAIN
#define BOOST_TEST_ALTERNATIVE_INIT_API
#define BOOST_TEST_MODULE closed interval fixed - point class test
#include <boost/test/unit_test.hpp>

/* user headers. */
#include "ml/all.h"

/*
 * Helpers.
 */

using namespace ml;
using namespace std;

BOOST_AUTO_TEST_SUITE(fp_closed_interval)

/*
 * number representations.
 */

const closed_unit_interval<uint32_t> zero{0}, half{0.5}, one{1};

BOOST_AUTO_TEST_CASE(representation)
{
    BOOST_CHECK(unwrap(zero) == 0);
    BOOST_CHECK(unwrap(half) == std::numeric_limits<uint32_t>::max() / 2);
    BOOST_CHECK(unwrap(one) == std::numeric_limits<uint32_t>::max());
}

/*
 * arithmetic.
 */

BOOST_AUTO_TEST_CASE(arithmetic)
{
    BOOST_CHECK(to_float(half + half) == to_float(one));
}

/*
 * comparisons.
 */

BOOST_AUTO_TEST_CASE(comparisons)
{
    BOOST_CHECK(zero < half);
    BOOST_CHECK(zero < one);
    BOOST_CHECK(half < one);

    BOOST_CHECK(!(zero > zero));
    BOOST_CHECK(zero >= zero);

    BOOST_CHECK(!(half > half));
    BOOST_CHECK(half >= half);
    BOOST_CHECK(one >= half);

    BOOST_CHECK(!(one > one));
    BOOST_CHECK(one >= one);

    BOOST_CHECK(half > zero);
    BOOST_CHECK(one > zero);
    BOOST_CHECK(one > half);

    BOOST_CHECK(!(zero > zero));
    BOOST_CHECK(zero >= zero);

    BOOST_CHECK(!(half > half));
    BOOST_CHECK(half >= half);
    BOOST_CHECK(one >= half);

    BOOST_CHECK(!(one > one));
    BOOST_CHECK(one >= one);
}

BOOST_AUTO_TEST_SUITE_END()
