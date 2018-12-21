/**
 * @file OverlapInfo.hpp
 * @brief the C++ file with declaration of struct responsible for
 * holding overlap information.
 */

#ifndef OverlapInfo_HPP
#define OverlapInfo_HPP

#include <cstdint>

/**
 * @brief OverlapInfo structure. Simple data structure containing overlap
 * information between two given sequences.
 */
struct OverlapInfo {
    int32_t a1; /**< overlap begin index on first sequence. */
    int32_t a2; /**< overlap end index on first sequence. */
    int32_t b1; /**< overlap begin index on second sequence. */
    int32_t b2; /**< overlap end index on second sequence. */
    double rawScore; /**< TODO: */
    double score; /**< TODO: */

    /**
     * Creates OverlapInfo structure with given parameters.
     * @param score_ TODO:
     * @param rawScore_ TODO:
     * @param a1_ overlap begin index on first sequence.
     * @param a2_ overlap end index on first sequence.
     * @param b1_ overlap begin index on second sequence.
     * @param b2_ overlap end index on second sequence.
     */
    OverlapInfo(double score_, double rawScore_, int32_t a1_, int32_t a2_, int32_t b1_, int32_t b2_)
        : a1(a1_)
          , a2(a2_)
          , b1(b1_)
          , b2(b2_)
          , rawScore(rawScore_)
          , score(score_) {}

    /**
     * Default OverlapInfo constructor symbolizing no overlap between sequences.
     */
    OverlapInfo() : OverlapInfo(0.0, 0.0, 0, 0, 0, 0) {}

    /**
     * Function wrapper for default constructor (empty overlap)
     * created for code readability purposes.
     * @return OverlapInfo structure (empty, default initialized).
     */
    static OverlapInfo empty() { return OverlapInfo(); }
};

#endif //OverlapInfo_HPP
