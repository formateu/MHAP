/**
 * @file BottomOverlapSketch.hpp
 * @brief the C++ file with declaration of class representing K-Bottom sketch of sequence.
 */
#ifndef BottomOverlapSketch_HPP
#define BottomOverlapSketch_HPP

#include <string>
#include <array>
#include <vector>

class MatchData;
struct OverlapInfo;

/**
 * @brief Class representing K-Bottom sketch of sequence.
 */
class BottomOverlapSketch {
public:
    /**
     * Creates K-Bottom sketch from raw sequence data based on given parameters.
     * @param seq raw sequence data.
     * @param kmerSize length of the k-mer.
     * @param sketchSize number of hash values used to represent a sequence.
     * @param doReverseCompliment flag determines whether reverse-compliment sequences are processed.
     */
    BottomOverlapSketch(const std::string &seq,
                        size_t kmerSize,
                        size_t sketchSize,
                        bool doReverseCompliment);

    /**
     * Deleted copy constructor for BottomOverlapSketch class.
     * @param oth const reference to other BottomOverlapSketch object.
     */
    BottomOverlapSketch(const BottomOverlapSketch &oth) = delete;

    /**
     * Deleted move constructor for BottomOverlapSketch class.
     * @param oth r-value reference to other BottomOverlapSketch object.
     */
    BottomOverlapSketch(BottomOverlapSketch &&oth) = default;

    /**
     * Accessor for original sequence size.
     * @return sketched sequence size.
     */
    std::size_t sequenceSize() const { return seqLength_; }

    /**
     * Accessor for number of hash, pos pairs in sketch.
     * @return number of hash, pos pairs in sketch.
     */
    std::size_t size() const { return orderedHashes_.size(); }

    /**
     * Converts Jaccard similarity to ?
     * @param score Jaccard similarity
     * @param kmerSize size of the k-mer used in sketch.
     * @return TODO:
     */
    double jaccardToIdentity(double score, size_t kmerSize) const;

    /**
     * Calculates overlap information between two K-Bottom sketches.
     * @param toSequence sequence to calculate overlap from *this.
     * @param maxShiftPercent margin of moving overlap boundaries, where overlap is still valid.
     * @return overlap information between two K-Bottom sketches.
     */
    OverlapInfo getOverlapInfo(const BottomOverlapSketch &toSequence, double maxShiftPercent) const;

protected:
    /**
     * Struct holding hash value and position in original sequence.
     * Created to improve readability (better than std::pair, std::tuple first, second, etc.)
     */
    struct HashPosPair {
        uint32_t hash; /**< k-mer hash value  */
        int32_t pos; /**< k-mer position in original sequence */

        /**
         * Default initialization of structure.
         */
        HashPosPair() : hash(0U), pos(0) {}
    };

    using KmerHashes = std::vector<HashPosPair>;

    size_t kmerSize_; /**< size of k-mer used to create K-Bottom sketch.*/
    KmerHashes orderedHashes_; /**< internal vector of hash, position pairs. */
    size_t seqLength_; /**< length of sketched sequence. */

    //TODO: has to be static?
    /**
     * Computes Jaccard similarity between given K-Bottom sketches.
     * @param seq1Hashes internal hash, position vector of first sequence sketch.
     * @param seq2Hashes internal hash, position vector of second sequence sketch.
     * @param a1 overlap begin index on first sequence.
     * @param a2 overlap end index on first sequence.
     * @param b1 overlap begin index on second sequence.
     * @param b2 overlap end index on second sequence.
     * @return Jaccard similarity between given K-Bottom sketches.
     */
    static double computeKBottomSketchJaccard(const KmerHashes &seq1Hashes,
                                              const KmerHashes &seq2Hashes,
                                              int32_t a1,
                                              int32_t a2,
                                              int32_t b1,
                                              int32_t b2);

    /**
     * Find matches between *this and given K-Bottom sketch.
     * @param matchData struct containing information about matches in sketches.
     * @param seqKmerHashes internal hash, position vector of another sequence sketch.
     */
    void recordMatchingKmers(MatchData &matchData, const KmerHashes &seqKmerHashes) const;
};

#endif //BottomOverlapSketch_HPP
