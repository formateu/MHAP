#include <SequenceSketchStreamer.hpp>

SequenceSketchStreamer::SequenceSketchStreamer(const std::string &filePath,
                                               size_t firstId,
                                               size_t minOlapLength,
                                               size_t kmerSize,
                                               size_t numHashes,
                                               size_t orderedKmerSize,
                                               size_t orderedSketchSize,
                                               bool doReverseCompliment,
                                               double repeatWeight)
    : fastaData_(filePath, firstId),
      minOlapLength_(minOlapLength),
      kmerSize_(kmerSize),
      numHashes_(numHashes),
      orderedKmerSize_(orderedKmerSize),
      orderedSketchSize_(orderedSketchSize),
      doReverseCompliment_(doReverseCompliment),
      repeatWeight_(repeatWeight) {

}

SequenceSketchStreamer::SequenceSketchStreamer(const std::string &filePath,
                                               size_t minOlapLength,
                                               size_t kmerSize,
                                               size_t numHashes,
                                               size_t orderedKmerSize,
                                               size_t orderedSketchSize,
                                               bool doReverseCompliment,
                                               double repeatWeight)
    : SequenceSketchStreamer(filePath,
                             0UL,
                             minOlapLength,
                             kmerSize,
                             numHashes,
                             orderedKmerSize,
                             orderedSketchSize,
                             doReverseCompliment,
                             repeatWeight) {}

SequenceSketchPtr SequenceSketchStreamer::dequeue(bool fwdOnly) {
    enqueueUntilFound(fwdOnly);

    auto head = SequenceSketchPtr{};

    if (!sequenceHashDeque_.empty()) {
        head = std::move(sequenceHashDeque_.front());
        sequenceHashDeque_.pop_front();
    }

    return head;
}

void SequenceSketchStreamer::enqueueFull(bool fwdOnly) {
    while (enqueueUntilFound(fwdOnly)) {}
}

SequenceSketchPtr SequenceSketchStreamer::getSketch(const Sequence &seq) {
    // TODO: maybe Sequence as rvalue-reference?
    return std::make_unique<SequenceSketch>(seq,
                                            kmerSize_,
                                            numHashes_,
                                            orderedKmerSize_,
                                            orderedSketchSize_,
//                          doReverseCompliment_,
                                            repeatWeight_);
}

bool SequenceSketchStreamer::enqueue(bool fwdOnly) {
    SequenceSketchPtr seqHashes{nullptr};
    SequencePtr seq{nullptr};

    do {
        //according to clang 6.0.0 std::move usage here prevents copy elision
        seq = fastaData_.dequeue();
    } while (seq && seq->size() < minOlapLength_);

    if (seq) {
        seqHashes = getSketch(*seq);
    }

    if (!seqHashes) {
        return false;
    }

    sequenceHashDeque_.emplace_back(std::move(seqHashes));

    if (!fwdOnly) {
        seqHashes = getSketch(seq->getReverseCompliment());
        sequenceHashDeque_.emplace_back(std::move(seqHashes));
    }

    return true;
}

// TODO: is this function even necessary now?
// It represents same behaviour as enqueue
bool SequenceSketchStreamer::enqueueUntilFound(bool fwdOnly) {
    bool getNext = true;
    bool returnValue = false;

    while (getNext) {
        returnValue = enqueue(fwdOnly);
        getNext = false;
    }

    return returnValue;
}