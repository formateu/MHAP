#include "../include/MinHashSearch.hpp"
#include <utility>

#include "../include/HitCounter.hpp"

MinHashSearch::MinHashSearch(SequenceSketchStreamer &data,
                             size_t numHashes,
                             size_t numMinMatches,
                             bool storeResults,
                             size_t minStoreLength,
                             double maxShift,
                             double acceptScore,
                             bool doReverseCompliment)
    : hashes_(numHashes)
      , numMinMatches_(numMinMatches)
      , minStoreLength_(minStoreLength)
      , maxShift_(maxShift)
      , acceptScore_(acceptScore) {

    data.enqueueFull(false);
    addData(data, doReverseCompliment);
}

void MinHashSearch::addData(SequenceSketchStreamer &seqStreamer, bool doReverseCompliment) {
    //TODO: parlallelize

    auto seqHashes = seqStreamer.dequeue(!doReverseCompliment);

    while (seqHashes) {
        addSequence(std::move(seqHashes));
        seqHashes = seqStreamer.dequeue(!doReverseCompliment);
    }
}

bool MinHashSearch::addSequence(SequenceSketchPtr &&currHash) {
    auto it = sequenceVectorHash_.emplace(currHash->getSequenceId(), std::move(*currHash)).first;

    size_t count = 0;
    auto id = it->second.getSequenceId();
    auto currMinHashes = it->second.getMinHashes().getMinHashArray();

    for (auto &hash : hashes_) {
        uint32_t hashVal = currMinHashes[count];

        if (hash.find(hashVal) == hash.end()) {
            hash.emplace(hashVal, std::vector<SequenceId>());
        }

        hash[hashVal].emplace_back(id);
        ++count;
    }

    return true;
}

std::list<MatchResult> MinHashSearch::findMatches() {
    //TODO: parallelize
    auto seqDeque{getStoredForwardSeqIds()};
    auto nextSequence = std::move(seqDeque.back());
    seqDeque.pop_back();
    std::list<MatchResult> matches;

    while (!seqDeque.empty()) {
        const auto &sequenceHashes = sequenceVectorHash_.find(nextSequence)->second;
        matches.splice(matches.end(), findMatches(sequenceHashes, true));
        auto nextSequence = std::move(seqDeque.back());
        seqDeque.pop_back();
    }

    const auto &sequenceHashes = sequenceVectorHash_.find(nextSequence)->second;
    auto localMatches = findMatches(sequenceHashes, true);
}

std::list<MatchResult> MinHashSearch::findMatches(const SequenceSketch &seqHashes, bool toSelf) {
    const auto &minHash = seqHashes.getMinHashes();
    std::unordered_map<SequenceId, HitCounter> bestSequenceHit;
}

std::deque<SequenceId> MinHashSearch::getStoredForwardSeqIds() {
    std::deque<SequenceId> seqIds;

    for (const auto &hash : sequenceVectorHash_) {
        const auto &sketch = hash.second;

        if (sketch.getSequenceId().isFwd) {
            seqIds.emplace_back(sketch.getSequenceId());
        }
    }

    return seqIds;
}
