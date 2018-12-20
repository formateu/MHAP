#ifndef MinHashSketch_HPP
#define MinHashSketch_HPP

#include <cstdint>
#include <vector>
#include <string>

class MinHashSketch {
public:
    MinHashSketch(const std::string &seq,
                  size_t nGramSize,
                  size_t numHashes,
                  bool doReverseCompliment,
                  double repeatWeight);

    MinHashSketch(const MinHashSketch &oth) = delete;
    MinHashSketch(MinHashSketch &&oht) = default;

    const std::vector<uint32_t> &getMinHashArray() const { return minHashes_; }
protected:
    std::vector<uint32_t> minHashes_;

    std::vector<uint32_t> computeNgramMinHashesWeighted(const std::string &seq,
                                                        size_t nGramSize,
                                                        size_t numHashes,
                                                        bool doReverseCompliment,
                                                        double repeatWeight);
};

#endif //MinHashSketch_HPP
