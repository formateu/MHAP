#include "common.hpp"
#include <boost/test/unit_test.hpp>

#include <memory>
#include "../include/Sequence.hpp"

BOOST_AUTO_TEST_SUITE(TestSequence)

BOOST_AUTO_TEST_CASE(TestConstructorWithCopy)
{
    const std::string s{"ACTG"};
    SequenceId id(0U, true);
    Sequence seq(s, id);

    BOOST_CHECK(seq.id == id);
    BOOST_CHECK_EQUAL(seq.read, s);
}

BOOST_AUTO_TEST_CASE(TestConstructorWithMove)
{
    std::string s{"ACTG"};
    SequenceId id(0U, true);
    Sequence seq(std::move(s), std::move(id));

    BOOST_CHECK(seq.id == id);
    BOOST_CHECK_EQUAL(seq.read, s);
}

BOOST_AUTO_TEST_CASE(TestrevCompSeq)
{
    std::string s{"ACTG"};
    std::string rc{"CAGT"};

    BOOST_CHECK_EQUAL(Sequence::revCompSeq(s), rc);
}

BOOST_AUTO_TEST_CASE(TestgetReverseCompliment)
{
    std::string s{"ACTG"};
    std::string rc{"CAGT"};
    SequenceId id(0U, true);
    SequenceId cmpId(0U, false);
    Sequence seq(s, id);

    auto rcSeq = seq.getReverseCompliment();
    BOOST_CHECK_EQUAL(rcSeq.read, rc);
    BOOST_CHECK_EQUAL(rcSeq.id.id, cmpId.id);
    BOOST_CHECK_EQUAL(rcSeq.id.isFwd, cmpId.isFwd);
}

BOOST_AUTO_TEST_CASE(TestSize)
{
    std::string s{"ACTG"};
    SequenceId id(0U, true);
    Sequence seq(s, id);

    BOOST_CHECK_EQUAL(seq.size(), s.size());
}

BOOST_AUTO_TEST_SUITE_END()
