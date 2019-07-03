/**
 * @file MinHashSearch.hpp
 * @brief the C++ file with declaration of class representing overlap search engine.
 */
#ifndef MinHashSearch_HPP
#define MinHashSearch_HPP

#include <list>
#include <vector>
#include <unordered_map>


#include "SequenceSketchStreamer.hpp"
#include "MatchResult.hpp"

/**
 * @brief Class representing overlap search engine.
 */
class MinHashSearch {
public:
    /**
     * Creates MinHashSearch object based on given parameters.
     * @param data producer (queue mechanism) providing sequence sketches.
     * @param numHashes number of hashes in MinHash sketches.
     * @param numMinMatches matching k-mers between sequences threshold.
     * @param storeResults TODO: later, UNUSED actually.
     * @param minStoreLength minimum length of read used for overlapping.
     * @param maxShift margin of moving overlap boundaries, where overlap is still valid (second stage filter only).
     * @param acceptScore score threshold, below that value overlaps are filtered.
     * @param doReverseCompliment flag determines if reverse-compliment sequences are processed.
     */
    MinHashSearch(SequenceSketchStreamer &data,
                  size_t numHashes,
                  size_t numMinMatches,
//                  bool storeResults,
                  size_t minStoreLength,
                  double maxShift,
                  double acceptScore,
                  bool doReverseCompliment);

    /**
     * Adds sequence sketch to search database.
     * @param currHash r-value reference to sequence sketch.
     * @return TODO: redundant, cannot fail actually??
     */
    bool addSequence(SequenceSketchPtr &&currHash);

    /**
     * Finds matches in stored sequences.
     * @return list of found matches.
     */
    std::list<MatchResult> findMatches();

    /**
     * Finds matches for concrete sequence.
     * @param seqHashes sequence to find matches for.
     * @param toSelf TODO: fill
     * @return list of found matches.
     */
    std::list<MatchResult> findMatches(const SequenceSketch &seqHashes, bool toSelf);

    /**
     * Gets all forward sequence Ids from database.
     * @return forward sequence Ids queue.
     */
    std::deque<SequenceId> getStoredForwardSeqIds();
protected:
    /**
     * Loads sequence sketches from streamer into search engine.
     * @param seqStreamer sequence sketch streamer.
     * @param doReverseCompliment flags determines whether reverse-compliment
     * sequences are processed
     */
    void addData(SequenceSketchStreamer &seqStreamer, bool doReverseCompliment);
private:
    std::vector<std::unordered_map<int32_t, std::vector<SequenceId>>> hashes_; /**< TODO: fill */
    std::unordered_map<SequenceId, SequenceSketch, SequenceIdHasher>
        sequenceVectorHash_; /**< map: Id -> Sketch. */
//    size_t numHashes_; /**< number of hashes in MinHash sketch. */
    size_t numMinMatches_; /**< minimum number of matches needed to record overlap.*/
//    bool storeResults_;
    size_t minStoreLength_; /**< minimum sequence length required for data to be processed. */
    double maxShift_; /**< margin of moving overlap boundaries. */
    double acceptScore_; /**< score threshold, below that value overlaps are filtered. */
};

#endif //MinHashSearch_HPP
