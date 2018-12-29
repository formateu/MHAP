#include <BottomOverlapSketch.hpp>
#include <HashUtils.hpp>
#include <MatchData.hpp>
#include <OverlapInfo.hpp>

#include <algorithm>
#include <numeric>
#include <cmath>

#include <GM3cpp/Murmur3_32.hpp>

BottomOverlapSketch::KmerHashes
BottomOverlapSketch::initializeSketch(const std::string &seq,
                                      size_t kmerSize,
                                      size_t sketchSize,
                                      bool doReverseCompliment) {
    KmerHashes kBottomSketch{seq.size() - kmerSize + 1};
    std::string_view seqView{seq};
    GM3cpp::Murmur3_32 hasher(0U);

    for (size_t i = 0UL; i < kBottomSketch.size(); ++i) {
        auto kmer = seqView.substr(i, kmerSize);
        kBottomSketch[i].hash = HashUtils::hashKmer(kmer, hasher, doReverseCompliment);
        kBottomSketch[i].pos = static_cast<int32_t>(i);
        hasher.reset();
    }

    std::stable_sort(kBottomSketch.begin(), kBottomSketch.end(), [](auto lhs, auto rhs) {
        return lhs.hash < rhs.hash;
    });

    auto k = std::min(sketchSize, kBottomSketch.size());
    kBottomSketch.resize(k);

    return kBottomSketch;
}

BottomOverlapSketch::BottomOverlapSketch(const std::string &seq,
                                         size_t kmerSize,
                                         size_t sketchSize,
                                         bool doReverseCompliment)
    : kmerSize_(kmerSize),
      seqLength_(seq.size() - kmerSize + 1),
      orderedHashes_(BottomOverlapSketch::initializeSketch(seq,
                                                           kmerSize,
                                                           sketchSize,
                                                           doReverseCompliment)) {}

// static
double BottomOverlapSketch::computeKBottomSketchJaccard(const KmerHashes &seq1Hashes,
                                                        const KmerHashes &seq2Hashes,
                                                        int32_t a1,
                                                        int32_t a2,
                                                        int32_t b1,
                                                        int32_t b2) {

    auto fillHashPosArrays = [](const auto &seqHashes, auto &array, auto low, auto high) {
        int32_t s = 0;
        for (const auto[hash, pos] : seqHashes) {
            if (pos >= low && pos <= high) {
                array[s].hash = hash;
                array[s].pos = pos;
                ++s;
            }
        }

        return s;
    };

    KmerHashes array1{seq1Hashes.size()};
    KmerHashes array2{seq2Hashes.size()};

    int32_t s1 = fillHashPosArrays(seq1Hashes, array1, a1, a2);
    int32_t s2 = fillHashPosArrays(seq2Hashes, array2, b1, b2);

    int32_t k = std::min(s1, s2);

    if (k == 0) {
        return 0;
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

size_t BottomOverlapSketch::extend(const KmerHashes &seqHashes,
                                   size_t i,
                                   int32_t validLower,
                                   int32_t validUpper,
                                   int32_t hash) {
    auto iLast = i;
    auto iTry = i + 1;

    if (iTry < seqHashes.size()) {
        auto[hashTry, posTry] = seqHashes[iTry];

        while (hashTry == hash && posTry >= validLower && posTry < validUpper) {
            iLast = iTry;
            ++iTry;

            if (iTry >= seqHashes.size()) {
                break;
            }

            hashTry = seqHashes[iTry].hash;
            posTry = seqHashes[iTry].pos;
        }
    }

    return iLast;
}

void BottomOverlapSketch::recordMatchingKmers(MatchData &matchData,
                                              const KmerHashes &seqKmerHashes) const {

    int32_t medianShift = matchData.getMedianShift();
    int32_t absMaxShift = matchData.getAbsMaxShift();
    int32_t valid1Lower = matchData.valid1Lower();
    int32_t valid2Lower = matchData.valid2Lower();
    int32_t valid1Upper = matchData.valid1Upper();
    int32_t valid2Upper = matchData.valid2Upper();

    matchData.reset();

    int32_t hash1;
    int32_t hash2;
    int32_t pos1;
    int32_t pos2;

    for (size_t i1 = 0, i2 = 0; i1 < orderedHashes_.size() && i2 < seqKmerHashes.size();) {
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
                // record match
                matchData.recordMatch(pos1, pos2, currShift);

                // we need to create symmetry for reverse compliment,
                // so we will look at first and last matches

                // move the indices to last points of same hashes
                size_t i1Last = extend(orderedHashes_, i1, valid1Lower, valid1Upper, hash1);
                size_t i2Last = extend(seqKmerHashes, i2, valid2Lower, valid2Upper, hash2);

                // store the match and update the counters
                if (i1 != i1Last && i2 != i2Last) {
                    int32_t pos1New = orderedHashes_[i1Last].pos;
                    int32_t pos2New = seqKmerHashes[i2Last].pos;

                    matchData.recordMatch(pos1New, pos2New, pos2New - pos1New);
                    i1 = i1Last + 1;
                    i2 = i2Last + 1;
                } else {
                    // simply move on if they don't match
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
                                               edgeData->a1,
                                               edgeData->a2,
                                               edgeData->b1,
                                               edgeData->b2);

    score = jaccardToIdentity(score, kmerSize_);

    auto rawScore = static_cast<double>(edgeData->count);

    return OverlapInfo(score, rawScore, edgeData->a1, edgeData->a2, edgeData->b1, edgeData->b2);
}