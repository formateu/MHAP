#include "common.hpp"
#include <boost/test/unit_test.hpp>

#include <memory>
#include "../include/SequenceStreamer.hpp"


class SequenceStreamerWrapper : public SequenceStreamer {
public:
    SequenceStreamerWrapper(const std::string &s) : SequenceStreamer(s) {}

    const std::vector<SequencePtr> &getSequences() const {
        return sequences_;
    }

    bool getReadAll() const { return readAll_;}
};

BOOST_AUTO_TEST_SUITE(TestSequenceStreamer)

BOOST_AUTO_TEST_CASE(TestConstructorNoThrow) {
    BOOST_CHECK_NO_THROW(SequenceStreamer("t1.fa"));
}

BOOST_AUTO_TEST_CASE(TestConstructorThrow) {
    BOOST_CHECK_THROW(SequenceStreamer("notExistsingFile"),
                      std::ifstream::failure);
}

BOOST_AUTO_TEST_CASE(TestEnqueueNextSequence) {
    SequenceStreamerWrapper s("t1.fa");

    BOOST_CHECK_EQUAL(s.enqueueNextSequence(), true);
    BOOST_CHECK_EQUAL(s.enqueueNextSequence(), true);
    BOOST_CHECK_EQUAL(s.enqueueNextSequence(), false);
}

BOOST_AUTO_TEST_CASE(TestEnqueueAll) {
    SequenceStreamerWrapper s("t1.fa");

    s.enqueueAll();
    BOOST_CHECK_EQUAL(s.getSequences().size(), 2U);
    BOOST_CHECK_EQUAL(s.getSequences()[0]->read, "ACTG");
    BOOST_CHECK_EQUAL(s.getSequences()[0]->id.id, 0U);
    BOOST_CHECK_EQUAL(s.getSequences()[0]->id.isFwd, true);
    BOOST_CHECK_EQUAL(s.getSequences()[1]->read, "CACC");
    BOOST_CHECK_EQUAL(s.getSequences()[1]->id.id, 1U);
    BOOST_CHECK_EQUAL(s.getSequences()[1]->id.isFwd, true);
    BOOST_CHECK_EQUAL(s.getReadAll(), true);
}

BOOST_AUTO_TEST_CASE(TestDequeue) {
    SequenceStreamerWrapper s("t1.fa");
    s.enqueueAll();
    auto seq1 = s.dequeue();
    auto seq2 = s.dequeue();

    BOOST_CHECK_EQUAL(s.getSequences().size(), 0U);

    BOOST_CHECK_EQUAL(seq1->read, "CACC");
    BOOST_CHECK_EQUAL(seq1->id.id, 1U);
    BOOST_CHECK_EQUAL(seq1->id.isFwd, true);

    BOOST_CHECK_EQUAL(seq2->read, "ACTG");
    BOOST_CHECK_EQUAL(seq2->id.id, 0U);
    BOOST_CHECK_EQUAL(seq2->id.isFwd, true);

    BOOST_CHECK_EQUAL(s.getReadAll(), true);
}

BOOST_AUTO_TEST_SUITE_END()
