#include <MinHashSearch.hpp>
#include <utility>

#include <HitCounter.hpp>

MinHashSearch::MinHashSearch(SequenceSketchStreamer &data,
                             size_t numHashes,
                             size_t numMinMatches,
//                             bool storeResults,
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
    //TODO: parallelize

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
        int32_t hashVal = currMinHashes[count];

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
    auto nextSequence = seqDeque.back();
    seqDeque.pop_back();
    std::list<MatchResult> matches;

    while (!seqDeque.empty()) {
        const auto &sequenceHashes = sequenceVectorHash_.find(nextSequence)->second;
        matches.splice(matches.end(), findMatches(sequenceHashes, true));
        nextSequence = seqDeque.back();
        seqDeque.pop_back();
    }

    const auto &sequenceHashes = sequenceVectorHash_.find(nextSequence)->second;
    matches.splice(matches.end(), findMatches(sequenceHashes, true));

    return matches;
}

std::list<MatchResult> MinHashSearch::findMatches(const SequenceSketch &seqHashes, bool toSelf) {
    const auto &minHash = seqHashes.getMinHashes();
    std::unordered_map<SequenceId, HitCounter, SequenceIdHasher> bestSequenceHit;

    const auto &minHashes = minHash.getMinHashArray();

    size_t hashIndex = 0;

    for (const auto &currHash : hashes_) {
        auto currentHashMatchListIt = currHash.find(minHashes[hashIndex]);

        if (currentHashMatchListIt != currHash.end()) {
            const auto &currentHashMatchList = currentHashMatchListIt->second;

            for (const auto &sequenceId : currentHashMatchList) {
                if (bestSequenceHit.find(sequenceId) != bestSequenceHit.end()) {
                    bestSequenceHit[sequenceId].addHit();
                } else {
                    bestSequenceHit.emplace(sequenceId, 1);
                }
            }

        }
        ++hashIndex;
    }

    std::list<MatchResult> matches;

    for (const auto &match : bestSequenceHit) {
        const auto &matchId = match.first;
        const auto &matchValue = match.second;

        if (toSelf && matchId.id == seqHashes.getSequenceId().id) {
            continue;
        }

        if (matchValue.count >= static_cast<int32_t>(numMinMatches_)) {
            const auto &matchedHashes = sequenceVectorHash_.find(matchId)->second;

            // never process short to short
            if (matchedHashes.getSequenceSize() < minStoreLength_
                && seqHashes.getSequenceSize() < minStoreLength_) {
                continue;
            }

            // never process long to long in self, with greater id
            if (toSelf && matchId.id > seqHashes.getSequenceId().id
                && matchedHashes.getSequenceSize() >= minStoreLength_
                && seqHashes.getSequenceSize() >= minStoreLength_) {
                continue;
            }

            // never process short to long
            if (toSelf && matchedHashes.getSequenceSize() < minStoreLength_
                && seqHashes.getSequenceSize() >= minStoreLength_) {
                continue;
            }

            OverlapInfo result =
                seqHashes.getOrderedHashes().getOverlapInfo(matchedHashes.getOrderedHashes(),
                                                            maxShift_);

            bool accept = result.score >= acceptScore_;

            if (accept) {
                auto currResult = MatchResult(seqHashes.getSequenceId(),
                                              matchId,
                                              result,
                                              seqHashes.getSequenceSize(),
                                              matchedHashes.getSequenceSize());
                matches.emplace_back(currResult);
            }
        }
    }

    return matches;
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