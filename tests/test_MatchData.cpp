#include "common.hpp"
#include <boost/test/unit_test.hpp>

#include <memory>
#include "../include/MatchData.hpp"
#include "../include/BottomOverlapSketch.hpp"

class MatchDataTest : public MatchData {
public:

};

BOOST_AUTO_TEST_SUITE(TestMatchData)

BOOST_AUTO_TEST_CASE(TestConstructor)
{
    std::string seq1{"ACTGCAC"};
    std::string seq2{"GCACCCAC"};

    BottomOverlapSketch o1{seq1, 4, 10, false};
    BottomOverlapSketch o2{seq2, 4, 10, false};

    MatchData m{o1, o2, 0.2};


//    EdgeData e{1,2,3,4,5};
//    BOOST_CHECK_EQUAL(e.a1, 1);
//    BOOST_CHECK_EQUAL(e.a2, 2);
//    BOOST_CHECK_EQUAL(e.b1, 3);
//    BOOST_CHECK_EQUAL(e.b2, 4);
//    BOOST_CHECK_EQUAL(e.count, 5);
}

BOOST_AUTO_TEST_SUITE_END()
