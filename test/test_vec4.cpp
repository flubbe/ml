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
#include "utils.h"

BOOST_AUTO_TEST_SUITE(vec4)

// paranoid checks.
static_assert(sizeof(ml::vec4) == 4 * sizeof(float));

BOOST_AUTO_TEST_CASE(alignment)
{
    std::vector<ml::vec4> v;
    random_initialize_real_std_vector(10, v);

    // check vector alignment while being paranoid.
    BOOST_TEST_MESSAGE("Checking 16-byte vector alignment");
    for(size_t i = 0; i < v.size(); ++i)
    {
        BOOST_CHECK(is_aligned(&v[i], 16));
    }
}

BOOST_AUTO_TEST_CASE(is_zero)
{
    BOOST_TEST(!ml::vec4(0, 0, 0, 1).is_zero());
    BOOST_TEST(ml::vec4(0, 0, 0, 0).is_zero());
}

BOOST_AUTO_TEST_CASE(component_product)
{
    BOOST_TEST((ml::vec4(1, 2, 3, 4) * ml::vec4(4, 3, 2, 1) == ml::vec4(4, 6, 6, 4)));
    BOOST_TEST((ml::vec4(0, 1, 0, 1) * ml::vec4(-1, 0, -1, 0) == ml::vec4(0, 0, 0, 0)));
    BOOST_TEST((ml::vec4(-2, -3, 3, 2) * ml::vec4(-1, 0, 4, -4) == ml::vec4(2, 0, 12, -8)));
}

BOOST_AUTO_TEST_CASE(dot_product)
{
    BOOST_TEST(ml::dot(ml::vec4(1, -1, 1, -1), ml::vec4(1, -1, 1, -1)) == 4);
    BOOST_TEST(ml::dot(ml::vec4(1, 2, 3, 4), ml::vec4(4, 3, 2, 1)) == 20);
    BOOST_TEST(ml::dot(ml::vec4(1, 2, 3, 4), ml::vec4(-2, 1, -4, 3)) == 0);
}

BOOST_AUTO_TEST_SUITE_END()
