#include <BottomOverlapSketch.hpp>
#include <HashUtils.hpp>
#include <MatchData.hpp>
#include <OverlapInfo.hpp>

#include <algorithm>
#include <numeric>
#include <cmath>

BottomOverlapSketch::BottomOverlapSketch(const std::string &seq,
                                         size_t kmerSize,
                                         size_t sketchSize,
                                         bool doReverseCompliment)
    : kmerSize_(kmerSize)
      , seqLength_(seq.size() - kmerSize + 1) {
    auto hashes{HashUtils::computeSeqHashes(seq, kmerSize, doReverseCompliment)};

    std::vector<int32_t> perm(hashes.size());
    std::iota(perm.begin(), perm.end(), 0);

    //copy uint32_t
    std::stable_sort(perm.begin(), perm.end(), [&hashes](auto lhs, auto rhs) {
        return hashes[lhs] < hashes[rhs];
    });

    size_t k = std::min(sketchSize, hashes.size());

    orderedHashes_.resize(k);

    for (size_t i = 0U; i < orderedHashes_.size(); ++i) {
        int32_t index = perm[i];
        orderedHashes_[i].hash = hashes[index];
        orderedHashes_[i].pos = index;
    }
}

//static
double BottomOverlapSketch::computeKBottomSketchJaccard(const KmerHashes &seq1Hashes,
                                                        const KmerHashes &seq2Hashes,
//        int32_t medianShift,
//        int32_t absMaxShiftInOverlap
                                                        int32_t a1,
                                                        int32_t a2,
                                                        int32_t b1,
                                                        int32_t b2) {

    int32_t s1 = 0;
    //TODO: rethink how to not copy struct here (not necessary at all)
    //maybe pointers, but dirty when using smart ptrs
    KmerHashes array1{seq1Hashes.size()};

    //get k for first sequence
    for (size_t i = 0; i < seq1Hashes.size(); ++i) {
        int32_t pos = seq1Hashes[i].pos;

        if (pos >= a1 && pos <= a2) {
            array1[s1] = seq1Hashes[i];
            ++s1;
        }
    }

    int32_t s2 = 0;
    //get k for second sequence
    KmerHashes array2{seq2Hashes.size()};
    for (size_t j = 0; j < seq2Hashes.size(); ++j) {
        int32_t pos = seq2Hashes[j].pos;

        if (pos >= b1 && pos <= b2) {
            array2[s2] = seq2Hashes[j];
            ++s2;
        }
    }

    int32_t k = std::min(s1, s2);

    if (k == 0) {
        return k;
    }

    int32_t i = 0;
    int32_t j = 0;
    int32_t intersectCount = 0;
    int32_t unionCount = 0;

    while (unionCount < k) {
        if (array1[i].hash < array2[j].hash) {
            ++i;
        } else if (array1[i].hash > array2[j].hash) {
            ++j;
        } else {
            ++intersectCount;
            ++i;
            ++j;
        }

        ++unionCount;
    }

    //score
    return intersectCount / static_cast<double>(k);
}

double BottomOverlapSketch::jaccardToIdentity(double score, size_t kmerSize) const {
    double d = -1.0f / static_cast<double>(kmerSize) * std::log(2.0 * score / (1.0 + score));
    return std::exp(-d);
}

void BottomOverlapSketch::recordMatchingKmers(MatchData &matchData,
                                              const KmerHashes &seqKmerHashes) const {
    int32_t hash1;
    int32_t hash2;
    int32_t pos1;
    int32_t pos2;

    int32_t medianShift = matchData.getMedianShift();
    int32_t absMaxShift = matchData.getAbsMaxShift();

    int32_t valid1Lower = matchData.valid1Lower();
    int32_t valid2Lower = matchData.valid2Lower();

    int32_t valid1Upper = matchData.valid1Upper();
    int32_t valid2Upper = matchData.valid2Upper();

    size_t i1 = 0;
    size_t i2 = 0;

    matchData.reset();

    while (true) {
        if (i1 >= orderedHashes_.size()) {
            break;
        }

        if (i2 >= seqKmerHashes.size()) {
            break;
        }

        hash1 = orderedHashes_[i1].hash;
        pos1 = orderedHashes_[i1].pos;

        hash2 = seqKmerHashes[i2].hash;
        pos2 = seqKmerHashes[i2].pos;

        if (hash1 < hash2 || pos1 < valid1Lower || pos1 >= valid1Upper) {
            ++i1;
        } else if (hash2 < hash1 || pos2 < valid2Lower || pos2 >= valid2Upper) {
            ++i2;
        } else {
            // check if current shift makes sense positionally
            int32_t currShift = pos2 - pos1;
            int32_t diffFromExpected = currShift - medianShift;

            if (diffFromExpected > absMaxShift) {
                ++i1;
            } else if (diffFromExpected < -absMaxShift) {
                ++i2;
            } else {
                //record match
                matchData.recordMatch(pos1, pos2, currShift);

                //we need to create symmetry for reverse compliment, so we will look at first and last matches

                //move the index to last point of same hash
                size_t i1Last = i1;
                size_t i1Try = i1 + 1;

                if (i1Try < orderedHashes_.size()) {
                    int32_t hash1Try = orderedHashes_[i1Try].hash;
                    int32_t pos1Try = orderedHashes_[i1Try].pos;

                    while (hash1Try == hash1 && pos1Try >= valid1Lower && pos1Try < valid1Upper) {
                        i1Last = i1Try;
                        ++i1Try;

                        if (i1Try >= orderedHashes_.size()) {
                            break;
                        }

                        hash1Try = orderedHashes_[i1Try].hash;
                        pos1Try = orderedHashes_[i1Try].pos;
                    }
                }

                //move the index to last point of same hash
                size_t i2Last = i2;
                size_t i2Try = i2 + 1;

                if (i2Try < seqKmerHashes.size()) {
                    int32_t hash2Try = seqKmerHashes[i2Try].hash;
                    int32_t pos2Try = seqKmerHashes[i2Try].pos;

                    while (hash2Try == hash2 && pos2Try >= valid2Lower && pos2Try < valid2Upper) {
                        i2Last = i2Try;
                        ++i2Try;

                        if (i2Try >= seqKmerHashes.size()) {
                            break;
                        }

                        hash2Try = seqKmerHashes[i2Try].hash;
                        pos2Try = seqKmerHashes[i2Try].pos;
                    }
                }

                //store the match and update the counters
                if (i1 != i1Last && i2 != i2Last) {
                    int32_t pos1New = orderedHashes_[i1Last].pos;
                    int32_t pos2New = seqKmerHashes[i2Last].pos;

                    matchData.recordMatch(pos1New, pos2New, pos2New - pos1New);
                    i1 = i1Last + 1;
                    i2 = i2Last + 1;
                } else {
                    //simply move on if they don't match
                    ++i1;
                    ++i2;
                }
            }
        }
    }
}

OverlapInfo BottomOverlapSketch::getOverlapInfo(const BottomOverlapSketch &toSequence,
                                                double maxShiftPercent) const {
    MatchData matchData{*this, toSequence, maxShiftPercent};

    //get the initial matches
    recordMatchingKmers(matchData, toSequence.orderedHashes_);

    if (matchData.isEmpty()) {
        return OverlapInfo::empty();
    }

    //get matches again, but now in a better region
    recordMatchingKmers(matchData, toSequence.orderedHashes_);

    if (matchData.isEmpty()) {
        return OverlapInfo::empty();
    }

    matchData.optimizeShifts();

    if (matchData.isEmpty()) {
        return OverlapInfo::empty();
    }

    auto edgeData = matchData.computeEdges();

    if (!edgeData) {
        return OverlapInfo::empty();
    }


    matchData.getMedianShift();
    matchData.getAbsMaxShift();
    double score = computeKBottomSketchJaccard(orderedHashes_,
                                               toSequence.orderedHashes_,
//                                               matchData.getMedianShift(),
//                                               matchData.getAbsMaxShift(),
                                               edgeData->a1,
                                               edgeData->a2,
                                               edgeData->b1,
                                               edgeData->b2);

    score = jaccardToIdentity(score, kmerSize_);

    auto rawScore = static_cast<double>(edgeData->count);

    return OverlapInfo(score, rawScore, edgeData->a1, edgeData->a2, edgeData->b1, edgeData->b2);
}