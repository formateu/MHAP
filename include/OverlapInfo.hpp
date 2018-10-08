#ifndef OverlapInfo_HPP
#define OverlapInfo_HPP

struct OverlapInfo {
    int32_t a1;
    int32_t a2;
    int32_t b1;
    int32_t b2;
    double rawScore;
    double score;

    OverlapInfo(double score_, double rawScore_, int32_t a1_, int32_t a2_, int32_t b1_, int32_t b2_)
        : a1(a1_)
          , a2(a2_)
          , b1(b1_)
          , b2(b2_)
          , rawScore(rawScore_)
          , score(score_) {}

    OverlapInfo() : OverlapInfo(0.0, 0.0, 0, 0, 0, 0) {}

    static OverlapInfo empty() { return OverlapInfo(); }


//    std::ostream& operator<< (std::ostream& os, const OverlapInfo& overlapInfo) {
//        os <<
//    }
};

#endif //OverlapInfo_HPP
