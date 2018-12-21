#include <MatchResult.hpp>

#include <boost/algorithm/string/join.hpp>

MatchResult::MatchResult(const SequenceId &fromId_,
                         const SequenceId &toId_,
                         OverlapInfo &overlap,
                         int32_t fromLength_,
                         int32_t toLength_)
    : fromId(fromId_), toId(toId_), fromLength(fromLength_), toLength(toLength_) {
    a1 = fromId.isFwd ? overlap.a1 : fromLength - overlap.a2 - 1;
    a2 = fromId.isFwd ? overlap.a2 : fromLength - overlap.a1 - 1;
    b1 = toId.isFwd ? overlap.b1 : toLength - overlap.b2 - 1;
    b2 = toId.isFwd ? overlap.b2 : toLength - overlap.b1 - 1;

    rawScore = overlap.rawScore;

    if (overlap.score > 1.0) {
        score = 1.0;
    } else {
        score = overlap.score;
    }
}

std::string MatchResult::toString() const {
    auto stringList = {
        fromId.getHeader(),
        toId.getHeader(),
        std::to_string(1.0 - score),
        std::to_string(rawScore),
        std::to_string(fromId.isFwd ? 0 : 1),
        std::to_string(a1),
        std::to_string(a2),
        std::to_string(fromLength),
        std::to_string(toId.isFwd ? 0 : 1),
        std::to_string(b1),
        std::to_string(b2),
        std::to_string(toLength)
    };

    return boost::algorithm::join(stringList, " ");
}