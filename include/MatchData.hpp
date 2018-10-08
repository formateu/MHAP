#ifndef MatchData_HPP
#define MatchData_HPP

#include <cstdint>
#include <vector>

#include "EdgeData.hpp"

class BottomOverlapSketch;
//class EdgeDataPtr;

class MatchData {
public:
    MatchData(const BottomOverlapSketch &o1, const BottomOverlapSketch &o2, double maxShiftPercent);

    EdgeDataPtr computeEdges();

    int32_t getAbsMaxShift();

    int32_t getMedianShift();

    bool isEmpty() const { return count_ <= 0; }

    void optimizeShifts();

    void recordMatch(int32_t pos1, int32_t pos2, int32_t shift);

    void reset() {
        count_ = 0U;
        needRecompute_ = true;
    }

    int32_t size() const { return count_; }

    int32_t valid1Lower();

    int32_t valid1Upper();

    int32_t valid2Lower();

    int32_t valid2Upper();

protected:
    int32_t absMaxShiftInOverlap_;
    int32_t count_;
    double maxShiftPercent_;
    int32_t medianShift_;
    bool needRecompute_;

    std::vector<int32_t> posShift_;
    std::vector<int32_t> pos1Index_;
    std::vector<int32_t> pos2Index_;

    int32_t seqSize1_;
    int32_t seqSize2_;

    void performUpdate();
};

#endif //MatchData_HPP
