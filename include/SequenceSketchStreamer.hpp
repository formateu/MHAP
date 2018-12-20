#ifndef SequenceSketchStreamer_HPP
#define SequenceSketchStreamer_HPP

#include <deque>
#include <string>
#include "SequenceStreamer.hpp"
#include "SequenceSketch.hpp"

class SequenceSketchStreamer {
public:
    SequenceSketchStreamer(const std::string &filePath,
                           size_t minOlapLength,
                           size_t kmerSize,
                           size_t numHashes,
                           size_t orderedKmerSize,
                           size_t orderedSketchSize,
                           bool doReverseCompliment,
                           double repeatWeight);

    SequenceSketchPtr dequeue(bool fwdOnly);
    void enqueueFull(bool fwdOnly);
    SequenceSketchPtr getSketch(const Sequence &seq);
protected:
    bool enqueue(bool fwdOnly);
    bool enqueueUntilFound(bool fwdOnly);
private:
    std::deque<SequenceSketchPtr> sequenceHashDeque_;
    SequenceStreamer fastaData_;
    size_t minOlapLength_;
    size_t kmerSize_;
    size_t numHashes_;
    size_t orderedKmerSize_;
    size_t orderedSketchSize_;
    bool doReverseCompliment_;
    double repeatWeight_;

};

#endif //SequenceSketchStreamer_HPP
