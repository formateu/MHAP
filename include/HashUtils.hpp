/**
 * @file HashUtils.hpp
 * @brief the C++ file with declaration of wrapper class for hashing functions.
 */
#ifndef HashUtils_HPP
#define HashUtils_HPP

#include <cstdint>
#include <vector>
#include <string>

/**
 * @brief Wrapper class for hashing functions.
 */
class HashUtils {
public:
    /**
     * Calculates hash values encoded on unsigned 64-bit integers
     * for sequences based on given parameters.
     * @param seq raw sequence data
     * @param nGramSize size of k-mer
     * @param seed random number seed generator
     * @param doReverseCompliment flags determines whether reverse-compliment k-mers are processed
     * @return vector of unsigned 64-bit integer hash values
     */
    static
    std::vector<int64_t> computeSeqHashesLong(const std::string &seq,
                                              size_t nGramSize,
                                              int32_t seed,
                                              bool doReverseCompliment);

    /**
     * Calculates hash values encoded on unsigned 32-bit integers
     * for sequences based on given parameters. Random number generator's seed
     * is fixed on 0U.
     * @param seq raw sequence data
     * @param nGramSize size of k-mer
     * @param doReverseCompliment flags determines whether reverse-compliment k-mers are processed
     * @return vector of unsigned 32-bit integer hash values
     */
    static
    std::vector<int32_t> computeSeqHashes(const std::string &seq,
                                          size_t nGramSize,
                                          bool doReverseCompliment);
};

#endif //HashUtils_HPP
