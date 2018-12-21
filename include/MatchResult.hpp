/**
 * @file MatchResult.hpp
 * @brief the C++ file with declaration of struct representing match between sequences.
 */
#ifndef MatchResult_HPP
#define MatchResult_HPP

#include "OverlapInfo.hpp"
#include "SequenceId.hpp"

/**
 * @brief Struct representing match between sequences.
 */
struct MatchResult {
    SequenceId fromId; /**< first sequence identifier. */
    SequenceId toId; /**< second sequence identifier. */
    int32_t a1; /**< overlap begin index on first sequence. */
    int32_t a2; /**< overlap end index on first sequence. */
    int32_t b1; /**< overlap begin index on second sequence. */
    int32_t b2; /**< overlap end index on second sequence. */
    double score; /**< TODO: */
    double rawScore; /**< TODO: */
    int32_t fromLength; /**< first sequence length. */
    int32_t toLength; /**< second seuquence length. */

    /**
     * Creates MatchResult object based on given parameters.
     * @param fromId_ first sequence identifier.
     * @param toId_ second sequence identifier.
     * @param overlap information about overlap between sequences.
     * @param fromLength_ first sequence length.
     * @param toLength_ second sequence length.
     */
    MatchResult(const SequenceId &fromId_,
                const SequenceId &toId_,
                OverlapInfo &overlap,
                int32_t fromLength_,
                int32_t toLength_);

    /**
     * Creates line with MatchResult compatible with original MHAP.
     * @return string with match information.
     */
    std::string toString() const;
};

#endif //MatchResult_HPP
