#ifndef SequenceSketch_HPP
#define SequenceSketch_HPP

#include "Sequence.hpp"
#include "MinHashSketch.hpp"
#include "BottomOverlapSketch.hpp"

class SequenceSketch {
public:
    SequenceSketch(const Sequence &seq,
                   size_t kmerSize,
                   size_t numHashes,
                   size_t orderedKmerSize,
                   size_t orderedSketchSize,
                   bool doReverseCompliment,
                   double repeatWeight);

    SequenceSketch(const SequenceSketch &oth) = delete;
    SequenceSketch(SequenceSketch &&oth) = default;

    const MinHashSketch &getMinHashes() const { return mainHashes_; }

    const SequenceId &getSequenceId() const { return id_; }
    //for problem with std::pair creation from (const SequenceId &, SequenceSketch &&)
    SequenceId getSeqIdCopy() const { return id_; }
private:
    size_t sequenceSize_;
    SequenceId id_;
    MinHashSketch mainHashes_;
    BottomOverlapSketch orderedHashes_;
};

using SequenceSketchPtr = std::unique_ptr<SequenceSketch>;

#endif //SequenceSketch_HPP
