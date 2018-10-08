#ifndef SequenceSketch_HPP
#define SequenceSketch_HPP

#include "Sequence.hpp"

class SequenceSketch {
public:
    SequenceSketch(const Sequence &seq,
                   size_t kmerSize,
                   size_t numHashes,
                   size_t orderedKmerSize,
                   size_t orderedSketchSize,
                   bool doReverseCompliment,
                   double repeatWeight);
private:
    size_t sequenceSize_;
    SequenceId id_;
    MinHashSketch mainHashes_;
    BottomOverlapSketch orderedHashes_;
};

#endif //SequenceSketch_HPP
