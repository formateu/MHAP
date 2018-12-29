/**
 * @file HashUtils.hpp
 * @brief the C++ file with declaration of wrapper class for hashing functions.
 */
#ifndef HashUtils_HPP
#define HashUtils_HPP

#include <cstdint>
#include <vector>
#include <string>

#include <Sequence.hpp>

/**
 * @brief Wrapper class for hashing functions.
 */
class HashUtils {
public:
    // DEPRECATED: TODO: remove
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

    // DEPRECATED: TODO: remove
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

    /**
     * Return hash value for given k-mer.
     * If doReverseCompliment, then reverse-complementary k-mer is lexically
     * compared and smaller string is hashed.
     * @param Hasher Hashing class,
     * must implement putEncodedChars(std::string_view) and hash() methods.
     * @param kmer view on k-mer data.
     * @param hasher Hasher class object.
     * @param doReverseCompliment flag determining whether process reverse-complementary k-mer.
     * @return hash value for given k-mer (type is Hasher dependent).
     */
    template<class Hasher>
    static inline auto hashKmer(std::string_view kmer, Hasher &hasher, bool doReverseCompliment) {
        std::string kmerRevCompl;

        if (doReverseCompliment) {
            kmerRevCompl = Sequence::revCompSeq(kmer);

            if (kmerRevCompl.compare(kmer) < 0) {
                kmer = kmerRevCompl;
            }
        }

        hasher.putUnencodedChars(kmer);

        return hasher.hash();
    }
};

#endif //HashUtils_HPP
