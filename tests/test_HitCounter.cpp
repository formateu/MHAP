#include "common.hpp"
#include <boost/test/unit_test.hpp>

#include "../include/HitCounter.hpp"

BOOST_AUTO_TEST_SUITE(TestHitCounter)

BOOST_AUTO_TEST_CASE(TestConstructor)
{
    HitCounter cnt0;
    HitCounter cnt1(1U);

    BOOST_CHECK_EQUAL(cnt0.count, 0U);
    BOOST_CHECK_EQUAL(cnt1.count, 1U);
}

BOOST_AUTO_TEST_CASE(TestAddHit)
{
    HitCounter cnt0;
    cnt0.addHit();

    BOOST_CHECK_EQUAL(cnt0.count, 1U);
}

BOOST_AUTO_TEST_SUITE_END()
