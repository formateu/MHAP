#ifndef MinHashSearch_HPP
#define MinHashSearch_HPP

#include <list>
#include <vector>
#include <unordered_map>

#include "SequenceSketchStreamer.hpp"
#include "MatchResult.hpp"

class MinHashSearch {
public:
    MinHashSearch(SequenceSketchStreamer &data,
                  size_t numHashes,
                  size_t numMinMatches,
                  bool storeResults,
                  size_t minStoreLength,
                  double maxShift,
                  double acceptScore,
                  bool doReverseCompliment);

    bool addSequence(SequenceSketchPtr &&currHash);
    std::list<MatchResult> findMatches();
    std::list<MatchResult> findMatches(const SequenceSketch &seqHashes, bool toSelf);
    std::deque<SequenceId> getStoredForwardSeqIds();
protected:
    void addData(SequenceSketchStreamer &seqStreamer, bool doReverseCompliment);
private:
    std::vector<std::unordered_map<uint32_t, std::vector<SequenceId>>> hashes_;
    std::unordered_map<SequenceId, SequenceSketch, SequenceIdHasher> sequenceVectorHash_;
    size_t numHashes_;
    size_t numMinMatches_;
//    bool storeResults_;
    size_t minStoreLength_;
    double maxShift_;
    double acceptScore_;
};

#endif //MinHashSearch_HPP
