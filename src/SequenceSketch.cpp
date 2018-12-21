#include <SequenceSketch.hpp>

SequenceSketch::SequenceSketch(const Sequence &seq,
                               size_t kmerSize,
                               size_t numHashes,
                               size_t orderedKmerSize,
                               size_t orderedSketchSize,
//                               bool doReverseCompliment,
                               double repeatWeight)
    : sequenceSize_(seq.size())
      , id_(seq.id)
      , mainHashes_(seq.read, kmerSize, numHashes, false, repeatWeight)
      , orderedHashes_(seq.read, orderedKmerSize, orderedSketchSize, false) {}

