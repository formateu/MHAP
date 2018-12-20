#include "../include/SequenceSketchStreamer.hpp"
SequenceSketchStreamer::SequenceSketchStreamer(const std::string &filePath,
                                               size_t minOlapLength,
                                               size_t kmerSize,
                                               size_t numHashes,
                                               size_t orderedKmerSize,
                                               size_t orderedSketchSize,
                                               bool doReverseCompliment,
                                               double repeatWeight)
    : fastaData_(filePath)
      , minOlapLength_(minOlapLength)
      , kmerSize_(kmerSize)
      , numHashes_(numHashes)
      , orderedKmerSize_(orderedKmerSize)
      , orderedSketchSize_(orderedSketchSize)
      , doReverseCompliment_(doReverseCompliment)
      , repeatWeight_(repeatWeight) {

}

SequenceSketchPtr SequenceSketchStreamer::dequeue(bool fwdOnly) {
    //TODO ensure that it is safe in case of empty queue
    enqueueUntilFound(fwdOnly);

    auto head = SequenceSketchPtr{};

    if (!sequenceHashDeque_.empty()) {
        auto head = std::move(sequenceHashDeque_.front());
        sequenceHashDeque_.pop_front();
    }

    return head;
}

void SequenceSketchStreamer::enqueueFull(bool fwdOnly) {
    while (enqueueUntilFound(fwdOnly)) {}
}

SequenceSketchPtr SequenceSketchStreamer::getSketch(const Sequence &seq) {
    return std::make_unique<SequenceSketch>(seq,
                          kmerSize_,
                          numHashes_,
                          orderedKmerSize_,
                          orderedSketchSize_,
                          doReverseCompliment_,
                          repeatWeight_);
}

bool SequenceSketchStreamer::enqueue(bool fwdOnly) {
    SequenceSketchPtr seqHashes{nullptr};
    SequencePtr seq{nullptr};

    do {
        seq = std::move(fastaData_.dequeue());
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

bool SequenceSketchStreamer::enqueueUntilFound(bool fwdOnly) {
    bool getNext = true;
    bool returnValue = false;

    while (getNext) {
        returnValue = enqueue(fwdOnly);
        getNext = false;
    }

    return returnValue;
}