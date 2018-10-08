#include "common.hpp"
#include <boost/test/unit_test.hpp>

#include <memory>
#include "../include/SequenceId.hpp"

BOOST_AUTO_TEST_SUITE(TestSequenceId)

BOOST_AUTO_TEST_CASE(TestConstructor)
{
    SequenceId id(0U, true);

    BOOST_CHECK_EQUAL(id.id, 0U);
    BOOST_CHECK_EQUAL(id.isFwd, true);
}

BOOST_AUTO_TEST_CASE(TestConstructorDefaultFwd)
{
    SequenceId id(0U);

    BOOST_CHECK_EQUAL(id.id, 0U);
    BOOST_CHECK_EQUAL(id.isFwd, true);
}

BOOST_AUTO_TEST_CASE(TestImplicitCopyConstructor)
{
    SequenceId id(0U, true);

    SequenceId copyId(id);

    BOOST_CHECK_EQUAL(copyId.id, 0U);
    BOOST_CHECK_EQUAL(copyId.isFwd, true);
}

BOOST_AUTO_TEST_CASE(TestImplicitMoveConstructor)
{
    SequenceId id(0U, true);

    SequenceId moveId(std::move(id));

    BOOST_CHECK_EQUAL(moveId.id, 0U);
    BOOST_CHECK_EQUAL(moveId.isFwd, true);
}

BOOST_AUTO_TEST_CASE(TestEqualityOperator)
{
    SequenceId id1(0U, true);
    SequenceId id2(0U, true);
    SequenceId id3(1U, true);
    SequenceId id4(0U, false);

    BOOST_CHECK_EQUAL(id1 == id2, true);
    BOOST_CHECK_EQUAL(id1 == id3, false);
    BOOST_CHECK_EQUAL(id1 == id4, false);
}

BOOST_AUTO_TEST_CASE(TestComplimentId)
{
    SequenceId id1(0U, true);
    auto id2{id1.complimentId()};
    BOOST_CHECK_EQUAL(id1 == id2, false);
    BOOST_CHECK_EQUAL(id1.isFwd, !id2.isFwd);
}

BOOST_AUTO_TEST_SUITE_END()
