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

namespace ml
{

template<typename T>
std::ostream& operator<<(std::ostream& os, const closed_unit_interval<T>& v)
{
    return os << "closed_unit_interval{" << +v.data << "}";
}

}    // namespace ml

BOOST_AUTO_TEST_SUITE(fp_closed_interval)

/*
 * number representations.
 */

const closed_unit_interval<uint32_t> zero{0}, half{0.5}, one{1};

BOOST_AUTO_TEST_CASE(representation)
{
    BOOST_CHECK_EQUAL(unwrap(zero), 0);
    BOOST_CHECK_EQUAL(unwrap(half), 2147483648u);
    BOOST_CHECK_EQUAL(unwrap(one), 4294967295u);

    BOOST_CHECK_EQUAL(ml::closed_unit_interval<uint32_t>(0.0f).data, 0u);
    BOOST_CHECK_EQUAL(ml::closed_unit_interval<uint32_t>(1.0f).data, ml::closed_unit_interval<uint32_t>::one);
}

/*
 * comparisons.
 */

BOOST_AUTO_TEST_CASE(comparisons)
{
    BOOST_CHECK_LT(zero, half);
    BOOST_CHECK_LT(zero, one);
    BOOST_CHECK_LT(half, one);

    BOOST_CHECK(!(zero > zero));
    BOOST_CHECK_GE(zero, zero);

    BOOST_CHECK(!(half > half));
    BOOST_CHECK_GE(half, half);
    BOOST_CHECK_GE(one, half);

    BOOST_CHECK(!(one > one));
    BOOST_CHECK_GE(one, one);

    BOOST_CHECK_GT(half, zero);
    BOOST_CHECK_GT(one, zero);
    BOOST_CHECK_GT(one, half);

    BOOST_CHECK(!(zero > zero));
    BOOST_CHECK_GE(zero, zero);

    BOOST_CHECK(!(half > half));
    BOOST_CHECK_GE(half, half);
    BOOST_CHECK_GE(one, half);

    BOOST_CHECK(!(one > one));
    BOOST_CHECK_GE(one, one);
}

BOOST_AUTO_TEST_SUITE_END()
