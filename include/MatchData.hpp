/**
 * @file MatchData.hpp
 * @brief the C++ file with declaration of helper class used for calculation
 * of overlap and similarities between K-Bottom sketches.
 */
#ifndef MatchData_HPP
#define MatchData_HPP

#include <cstdint>
#include <vector>

#include "EdgeData.hpp"

class BottomOverlapSketch;

/**
 * @brief Helper class used for calculation of overlap and similarities between K-Bottom sketches.
 */
class MatchData {
public:
    /**
     * Creates MatchData object for K-Bottom sketch comparison.
     * @param o1 K-Bottom sketch of first sequence
     * @param o2 K-Bottom sketch of second sequence
     * @param maxShiftPercent margin of moving overlap boundaries, where overlap is still valid.
     */
    MatchData(const BottomOverlapSketch &o1, const BottomOverlapSketch &o2, double maxShiftPercent);

    /**
     * TODO: fill
     * @return
     */
    EdgeDataPtr computeEdges();

    /**
     * Recalculates absolute maxshift, median shift and returns maxshift value.
     * @return absolute maxshift.
     */
    int32_t getAbsMaxShift();

    /**
     * Recalculates absolute maxshift, median shift and returns median shift value.
     * @return median shift.
     */
    int32_t getMedianShift();

    /**
     * Test whether there is no match.
     * @return whether sequences probably don't match.
     */
    bool isEmpty() const { return count_ <= 0; }


    /**
     * TODO: fill
     */
    void optimizeShifts();

    /**
     * Records another match between sketches.
     * @param pos1 position that matches in first sequence.
     * @param pos2 position that matches in second sequence.
     * @param shift TODO: fill
     */
    void recordMatch(int32_t pos1, int32_t pos2, int32_t shift);

    /**
     * Resets match data (count and allow shifts update).
     */
    void reset() {
        count_ = 0U;
        needRecompute_ = true;
    }

    /**
     * Accessor returns match size.
     * @return match size.
     */
    int32_t size() const { return count_; }

    /**
     * Recalculates absolute maximum and median shifts and returns valid match lower boundary index
     * on first sequence.
     * @return valid match lower boundary index on first sequence.
     */
    int32_t valid1Lower();

    /**
     * Recalculates absolute maximum and median shifts and returns valid match upper boundary index
     * on first sequence.
     * @return valid match upper boundary index on first sequence.
     */
    int32_t valid1Upper();

    /**
     * Recalculates absolute maximum and median shifts and returns valid match lower boundary index
     * on second sequence.
     * @return valid match lower boundary index on second sequence.
     */
    int32_t valid2Lower();

    /**
     * Recalculates absolute maximum and median shifts and returns valid match upper boundary index
     * on second sequence.
     * @return valid match upper boundary index on second sequence.
     */
    int32_t valid2Upper();

protected:
    int32_t absMaxShiftInOverlap_; /**< absolute maxshift in overlap. */
    int32_t count_; /**< overlap size. */
    double maxShiftPercent_; /**< maximum allowed shift percent. */
    int32_t medianShift_; /**< median shift size. */
    bool needRecompute_; /**< flag determines whether shifts need to be recalculated. */

    std::vector<int32_t> posShift_; /**< vector containing position shifts. */
    std::vector<int32_t> pos1Index_; /**< TODO: fill */
    std::vector<int32_t> pos2Index_; /**< TODO: fill */

    int32_t seqSize1_; /**< first sequence size. */
    int32_t seqSize2_; /**< second sequence size. */

    /**
     * Recalculates absolute maximum and median shifts if there is need for recompute.
     */
    void performUpdate();
};

#endif //MatchData_HPP
