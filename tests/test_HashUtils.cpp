#include "common.hpp"
#include <boost/test/unit_test.hpp>

#include <memory>
#include "../include/HashUtils.hpp"

BOOST_AUTO_TEST_SUITE(TestHashUtils)

BOOST_AUTO_TEST_CASE(TestcomputeSeqHashesLong)
{
    const std::string s{"ACTGCACCGGCAC"};

    auto hashes =  HashUtils::computeSeqHashesLong(s, 2UL, 0U, false);
    BOOST_CHECK_EQUAL(hashes.size(), s.size() - 2 + 1);
}

BOOST_AUTO_TEST_CASE(TestcomputeSeqHashes)
{
    const std::string s{"ACTGCACCGGCAC"};

    auto hashes =  HashUtils::computeSeqHashes(s, 2UL, false);
    BOOST_CHECK_EQUAL(hashes.size(), s.size() - 2 + 1);
}

BOOST_AUTO_TEST_CASE(TestConcreteHash)
{
    const std::string s{"ACTG"};

    auto hashes =  HashUtils::computeSeqHashesLong(s, 4UL, 0, false);
    BOOST_TEST_MESSAGE("Hash for ACTG string is: " + std::to_string(hashes[0]));
    //BOOST_CHECK_EQUAL(hashes.size(), s.size() - 2 + 1);
}

BOOST_AUTO_TEST_SUITE_END()
