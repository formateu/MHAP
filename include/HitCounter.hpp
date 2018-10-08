#ifndef HitCounter_HPP
#define HitCounter_HPP

#include <cstdint>

struct HitCounter {
    int32_t count;

    HitCounter(int32_t cnt) : count(cnt) {}
    HitCounter() : HitCounter(0U) {}

    HitCounter(const HitCounter &oth) = delete;
    HitCounter(HitCounter &&oth) = delete;

    HitCounter &addHit() {
        ++count;
        return *this;
    }

    //TODO: reconsider this instead addHit function, maybe more readable
//    HitCounter &operator++() {
//        ++count;
//        return *this;
//    }

};

#endif //HitCounter_HPP
