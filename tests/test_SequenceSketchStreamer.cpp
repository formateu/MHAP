#include "common.hpp"
#include <boost/test/unit_test.hpp>

#include <memory>
#include "../include/SequenceSketchStreamer.hpp"

class SequenceSketchStreamerWrapper : public SequenceSketchStreamer {
public:
    SequenceSketchStreamerWrapper(const std::string &s)
        : SequenceSketchStreamer(s, 0, 0, 0, 0, 0, true, 0) {}

    std::size_t queueSize() const { return sequenceHashDeque_.size(); }

    bool enqueue(bool fwdOnly) { return SequenceSketchStreamer::enqueue(fwdOnly); }
    bool enqueueUntilFound(bool fwdOnly) { return SequenceSketchStreamer::enqueueUntilFound(fwdOnly); }
};

BOOST_AUTO_TEST_SUITE(TestSequenceSketchStreamer)

BOOST_AUTO_TEST_CASE(TestConstructor) {
    SequenceSketchStreamer seq("t1.fa", 0, 0, 0, 0, 0, true, 0);
}

BOOST_AUTO_TEST_CASE(TestEnqueueFull) {
    SequenceSketchStreamerWrapper seq("t1.fa");
    seq.enqueueFull(false);
    BOOST_CHECK_EQUAL(seq.queueSize(), 4);
}

BOOST_AUTO_TEST_CASE(TestEnqueueFullFwdOnly) {
    SequenceSketchStreamerWrapper seq("t1.fa");
    seq.enqueueFull(true);
    BOOST_CHECK_EQUAL(seq.queueSize(), 2);
}

BOOST_AUTO_TEST_CASE(TestDequeue) {
    SequenceSketchStreamerWrapper seq("t1.fa");
    seq.enqueueFull(true);

    auto sketchPtr = seq.dequeue(true);
    BOOST_CHECK_EQUAL(seq.queueSize(), 1);
    BOOST_CHECK(sketchPtr);

    sketchPtr = seq.dequeue(true);
    BOOST_CHECK_EQUAL(seq.queueSize(), 0);
    BOOST_CHECK(sketchPtr);

    sketchPtr = seq.dequeue(true);
    BOOST_CHECK_EQUAL(seq.queueSize(), 0);

    BOOST_CHECK(!sketchPtr);
}

BOOST_AUTO_TEST_CASE(TestGetSketch) {
    SequenceSketchStreamerWrapper seq("t1.fa");
    Sequence sequence("ACTG", 0UL);

    auto sketchPtr = seq.getSketch(sequence);

    BOOST_CHECK(sketchPtr);
}

BOOST_AUTO_TEST_CASE(TestEnqueue) {
    SequenceSketchStreamerWrapper seq("t1.fa");
    BOOST_CHECK_EQUAL(seq.enqueue(false), true);
    BOOST_CHECK_EQUAL(seq.queueSize(), 2);

    BOOST_CHECK_EQUAL(seq.enqueue(false), true);
    BOOST_CHECK_EQUAL(seq.queueSize(), 4);

    BOOST_CHECK_EQUAL(seq.enqueue(false), false);
    BOOST_CHECK_EQUAL(seq.queueSize(), 4);
}

BOOST_AUTO_TEST_CASE(TestEnqueueFwdOnly) {
    SequenceSketchStreamerWrapper seq("t1.fa");
    BOOST_CHECK_EQUAL(seq.enqueue(true), true);
    BOOST_CHECK_EQUAL(seq.queueSize(), 1);

    BOOST_CHECK_EQUAL(seq.enqueue(true), true);
    BOOST_CHECK_EQUAL(seq.queueSize(), 2);

    BOOST_CHECK_EQUAL(seq.enqueue(true), false);
    BOOST_CHECK_EQUAL(seq.queueSize(), 2);
}

BOOST_AUTO_TEST_CASE(TestEnqueueUntilFound) {
    SequenceSketchStreamerWrapper seq("t1.fa");
    BOOST_CHECK_EQUAL(seq.enqueueUntilFound(false), true);
    BOOST_CHECK_EQUAL(seq.queueSize(), 2);

    BOOST_CHECK_EQUAL(seq.enqueueUntilFound(false), true);
    BOOST_CHECK_EQUAL(seq.queueSize(), 4);

    BOOST_CHECK_EQUAL(seq.enqueueUntilFound(false), false);
    BOOST_CHECK_EQUAL(seq.queueSize(), 4);
}

BOOST_AUTO_TEST_CASE(TestEnqueueUntilFoundFwdOnly) {
    SequenceSketchStreamerWrapper seq("t1.fa");
    BOOST_CHECK_EQUAL(seq.enqueueUntilFound(true), true);
    BOOST_CHECK_EQUAL(seq.queueSize(), 1);

    BOOST_CHECK_EQUAL(seq.enqueueUntilFound(true), true);
    BOOST_CHECK_EQUAL(seq.queueSize(), 2);

    BOOST_CHECK_EQUAL(seq.enqueueUntilFound(true), false);
    BOOST_CHECK_EQUAL(seq.queueSize(), 2);
}

BOOST_AUTO_TEST_SUITE_END()