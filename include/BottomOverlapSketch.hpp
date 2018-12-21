#ifndef BottomOverlapSketch_HPP
#define BottomOverlapSketch_HPP

#include <string>
#include <array>
#include <vector>
//#include <boost/scoped_array.hpp>
class MatchData;
struct OverlapInfo;

class BottomOverlapSketch {
public:
    BottomOverlapSketch(const std::string &seq,
                        size_t kmerSize,
                        size_t sketchSize,
                        bool doReverseCompliment);

    BottomOverlapSketch(const BottomOverlapSketch &oth) = delete;
    BottomOverlapSketch(BottomOverlapSketch &&oth) = default;

    std::size_t sequenceSize() const { return seqLength_; }
    std::size_t size() const { return orderedHashes_.size(); }

    double jaccardToIdentity(double score, size_t kmerSize) const;
    OverlapInfo getOverlapInfo(const BottomOverlapSketch &toSequence, double maxShiftPercent) const;

protected:
    struct HashPosPair {
        uint32_t hash;
        int32_t pos;
        HashPosPair() : hash(0U), pos(0) {}
    };

    using KmerHashes = std::vector<HashPosPair>;
    size_t kmerSize_;
    KmerHashes orderedHashes_;
    size_t seqLength_;

    //TODO: this has to be static?
    static double computeKBottomSketchJaccard(const KmerHashes &seq1Hashes,
                                              const KmerHashes &seq2Hashes,
                                              int32_t a1,
                                              int32_t a2,
                                              int32_t b1,
                                              int32_t b2);

    void recordMatchingKmers(MatchData &matchData, const KmerHashes &seqKmerHashes) const;
};

#endif //BottomOverlapSketch_HPP
