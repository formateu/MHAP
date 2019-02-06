#include <algorithm>
#include <limits>
#include <cmath>

#include <MatchData.hpp>
#include <BottomOverlapSketch.hpp>
//#include "../include/EdgeData.hpp"

MatchData::MatchData(const BottomOverlapSketch &o1,
                     const BottomOverlapSketch &o2,
                     double maxShiftPercent)
    : maxShiftPercent_(maxShiftPercent),
      posShift_(std::max(o1.size(), o2.size()) / 4 + 1, 0),
      pos1Index_(posShift_.size(), 0),
      pos2Index_(posShift_.size(), 0),
      seqSize1_(static_cast<int32_t>(o1.sequenceSize())),
      seqSize2_(static_cast<int32_t>(o2.sequenceSize())) {
    reset();
}

EdgeDataPtr MatchData::computeEdges() {
    int32_t leftEdge1 = std::numeric_limits<int32_t>::max();
    int32_t leftEdge2 = std::numeric_limits<int32_t>::max();

    int32_t rightEdge1 = std::numeric_limits<int32_t>::min();
    int32_t rightEdge2 = std::numeric_limits<int32_t>::min();

    int32_t validCount = 0;
    int32_t medianShift = getMedianShift();
    int32_t absMaxShiftInOverlap = getAbsMaxShift();

    int32_t count = size();

    for (int32_t iter = 0; iter < count; ++iter) {
        int32_t pos1 = pos1Index_[iter];
        int32_t pos2 = pos2Index_[iter];

        if (std::abs(posShift_[iter] - medianShift) > absMaxShiftInOverlap) {
            continue;
        }

        //TODO: rethink if maybe assign just maxes of two values
        // instead of ifs
        if (pos1 < leftEdge1) {
            leftEdge1 = pos1;
        }

        if (pos2 < leftEdge2) {
            leftEdge2 = pos2;
        }

        if (pos1 > rightEdge1) {
            rightEdge1 = pos1;
        }

        if (pos2 > rightEdge2) {
            rightEdge2 = pos2;
        }

        ++validCount;
    }

    if (validCount < 3) {
        return EdgeDataPtr{nullptr};
    }
    // get edge info uniformly minimum variance unbiased (UMVU) estimators
    // a = (n*a-b)/(n-1)
    // b = (n*b-a)/(n-1)
    int32_t n = validCount;
    double n_1 = n - 1;

    double na1_b1 = n * leftEdge1 - rightEdge1;
    double nb1_a1 = n * rightEdge1 - leftEdge1;
    double na2_b2 = n * leftEdge2 - rightEdge2;
    double nb2_a2 = n * rightEdge2 - leftEdge2;

    int32_t a1 = std::max(0, static_cast<int32_t>(std::round(na1_b1 / n_1)));
    int32_t a2 = std::min(seqSize1_, static_cast<int32_t>(std::round(nb1_a1 / n_1)));

    int32_t b1 = std::max(0, static_cast<int32_t>(std::round(na2_b2 / n_1)));
    int32_t b2 = std::min(seqSize2_, static_cast<int32_t>(std::round(nb2_a2 / n_1)));

    return std::make_unique<EdgeData>(a1, a2, b1, b2, validCount);
}

int32_t MatchData::getAbsMaxShift() {
    performUpdate();
    return absMaxShiftInOverlap_;
}

int32_t MatchData::getMedianShift() {
    performUpdate();
    return medianShift_;
}

void MatchData::optimizeShifts() {
    if (isEmpty()) {
        return;
    }

    int32_t reducedCount = -1;
    int32_t medianShfit = getMedianShift();

    for (int32_t iter = 0; iter < count_; ++iter) {
        if (reducedCount >= 0 && pos1Index_[reducedCount] == pos1Index_[iter]) {

            //if better, record it
            if (std::abs(posShift_[reducedCount] - medianShfit)
                > std::abs(posShift_[iter] - medianShfit)) {
                pos1Index_[reducedCount] = pos1Index_[iter];
                pos2Index_[reducedCount] = pos2Index_[iter];
                posShift_[reducedCount] = posShift_[iter];
            }
        } else {
            //add the new data
            ++reducedCount;
            pos1Index_[reducedCount] = pos1Index_[iter];
            pos2Index_[reducedCount] = pos2Index_[iter];
            posShift_[reducedCount] = posShift_[iter];
        }
    }

    count_ = reducedCount + 1;
    needRecompute_ = true;
}

//private
void MatchData::performUpdate() {
    if (needRecompute_) {
        if (count_ > 0) {
            auto posShiftCopy{posShift_};
            posShiftCopy.resize(count_);

            std::nth_element(posShiftCopy.begin(),
                             posShiftCopy.begin() + posShiftCopy.size() / 2,
                             posShiftCopy.end());

            medianShift_ = posShiftCopy[posShiftCopy.size() / 2];
            //not needed anymore
            posShiftCopy.clear();

            int32_t leftPosition = std::max(0, -medianShift_);
            int32_t rightPosition = std::min(seqSize1_, seqSize2_ - medianShift_);

            int32_t overlapSize = std::max(10, rightPosition - leftPosition);

            absMaxShiftInOverlap_ = std::min(std::max(seqSize1_, seqSize2_),
                                             static_cast<int32_t>((double) overlapSize
                                                 * maxShiftPercent_));
        } else {
            medianShift_ = 0;
            absMaxShiftInOverlap_ = std::max(seqSize1_, seqSize2_) + 1;
        }
    }

    needRecompute_ = false;
}

void MatchData::recordMatch(int32_t pos1, int32_t pos2, int32_t shift) {
    if (posShift_.size() <= static_cast<size_t>(count_)) {
        posShift_.resize(posShift_.size() * 2, 0);
        pos1Index_.resize(pos1Index_.size() * 2, 0);
        pos2Index_.resize(pos2Index_.size() * 2, 0);
    }

    posShift_[count_] = shift;
    pos1Index_[count_] = pos1;
    pos2Index_[count_] = pos2;

    ++count_;
    needRecompute_ = true;
}

int32_t MatchData::valid1Lower() {
    performUpdate();
    return std::max(0, -getMedianShift() - getAbsMaxShift());
}

int32_t MatchData::valid1Upper() {
    performUpdate();
    return std::min(seqSize1_, seqSize2_ - getMedianShift() + getAbsMaxShift());
}

int32_t MatchData::valid2Lower() {
    performUpdate();
    return std::max(0, getMedianShift() - getAbsMaxShift());
}

int32_t MatchData::valid2Upper() {
    performUpdate();
    return std::min(seqSize2_, seqSize1_ + getMedianShift() + getAbsMaxShift());
}