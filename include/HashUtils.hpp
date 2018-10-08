#ifndef HashUtils_HPP
#define HashUtils_HPP

#include <cstdint>
#include <vector>
#include <string>

class HashUtils {
public:
    static
    std::vector<uint64_t> computeSeqHashesLong(const std::string &seq,
                                               size_t nGramSize,
                                               uint32_t seed,
                                               bool doReverseCompliment);

    static
    std::vector<uint32_t> computeSeqHashes(const std::string &seq,
                                           size_t nGramSize,
                                           bool doReverseCompliment);
};

#endif //HashUtils_HPP
