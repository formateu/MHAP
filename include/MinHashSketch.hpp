/**
 * @file MinHashSketch.hpp
 * @brief the C++ file with declaration of class representing
 * MinHash sketch of the sequence.
 */
#ifndef MinHashSketch_HPP
#define MinHashSketch_HPP

#include <cstdint>
#include <vector>
#include <string>

/**
 * @brief Class representing MinHash sketch of the sequence.
 */
class MinHashSketch {
public:
    /**
     * Creates MinHashSketch object from given sequence based on parameters.
     * @param seq raw string containing sequence.
     * @param nGramSize size of the k-mer.
     * @param numHashes number of hashes representing sequence.
     * @param doReverseCompliment flag determining whether sequence is forward or reverse-compliment.
     * @param repeatWeight TODO: fill.
     */
    MinHashSketch(const std::string &seq,
                  size_t nGramSize,
                  size_t numHashes,
                  bool doReverseCompliment,
                  double repeatWeight);

    /**
     * Deleted copy constructor for MinHashSketch class.
     * @param oth const reference to other MinHashSketch object.
     */
    MinHashSketch(const MinHashSketch &oth) = delete;

    /**
     * Deleted move constructor for MinHashSketch class.
     * @param oth r-value reference to other MinHashSketch object.
     */
    MinHashSketch(MinHashSketch &&oht) = default;

    /**
     * Accessor to internal minhash array (read-only).
     * @return
     */
    const std::vector<uint32_t> &getMinHashArray() const { return minHashes_; }
protected:
    std::vector<uint32_t> minHashes_; /**< internal MinHash array. */

    /**
     * Creates raw MinHash sketch from sequence string based on given parameters..
     * @param seq read data.
     * @param nGramSize k-mer size.
     * @param numHashes number of hashes used for sketch.
     * @param doReverseCompliment flag determining if sequence is reverse-compliment.
     * @param repeatWeight TODO: fill.
     * @return vector of hashes encoded on unsigned 32-bit integers.
     */
    std::vector<uint32_t> computeNgramMinHashesWeighted(const std::string &seq,
                                                        size_t nGramSize,
                                                        size_t numHashes,
                                                        bool doReverseCompliment,
                                                        double repeatWeight);
};

#endif //MinHashSketch_HPP
