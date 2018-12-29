/**
 * @file HitCounter.hpp
 * @brief the C++ file with declaration of struct used to record matches
 * in first stage MinHash filter.
 */
#ifndef HitCounter_HPP
#define HitCounter_HPP

#include <cstdint>

/**
 * @brief  struct used to record matches in first stage MinHash filter.
 */
struct HitCounter {
    int32_t count; /**< Number of matches. */

    /**
     * Creates HitCounter object initialized with given counter value.
     * @param cnt counter value.
     */
    explicit HitCounter(int32_t cnt) : count(cnt) {}

    /**
     * Creates HitCounter with reset counter.
     */
    HitCounter() : HitCounter(0U) {}

    /**
     * Deleted copy constructor for HitCounter struct.
     * @param oth const reference to other HitCounter object.
     */
    HitCounter(const HitCounter &oth) = delete;

    /**
     * Deleted move constructor for HitCounter struct.
     * @param oth r-value reference to other HitCounter object.
     */
    HitCounter(HitCounter &&oth) = delete;

    /**
     * Increments counter.
     * @return self-reference after modification.
     */
    HitCounter &addHit() {
        ++count;
        return *this;
    }
};

#endif //HitCounter_HPP
