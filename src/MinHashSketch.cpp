#include <MinHashSketch.hpp>
#include <Sequence.hpp>
#include <HashUtils.hpp>
#include <HitCounter.hpp>

#include <unordered_map>
#include <string_view>

#include <GM3cpp/Murmur3_128.hpp>

//public
MinHashSketch::MinHashSketch(const std::string &seq,
                             size_t nGramSize,
                             size_t numHashes,
                             bool doReverseCompliment,
                             double repeatWeight)
    : minHashes_(computeNgramMinHashesWeighted(seq,
                                               nGramSize,
                                               numHashes,
                                               doReverseCompliment,
                                               repeatWeight)) {}

//protected
std::vector<int32_t>
MinHashSketch::computeNgramMinHashesWeighted(const std::string &seq,
                                             size_t nGramSize,
                                             size_t numHashes,
                                             bool doReverseCompliment,
                                             double repeatWeight) {
    // create hash map in-place not from a vector of hashes.
    std::unordered_map<int64_t, HitCounter> hitMap;
    std::string_view seqView{seq};
    size_t numOfKmers = seq.size() - nGramSize + 1;
    GM3cpp::Murmur3_128 hasher(0U);

    /**
     * Find all unique kmers with their frequencies
     */
    for (size_t i = 0UL; i < numOfKmers; ++i) {
        auto kmer = seqView.substr(i, nGramSize);
        auto hashedKmer = HashUtils::hashKmer(kmer, hasher, doReverseCompliment);
        hasher.reset();

        if (hitMap.find(hashedKmer) == hitMap.end()) {
            hitMap.emplace(hashedKmer, 1U);
        } else {
            hitMap[hashedKmer].addHit();
        }

    }

    std::vector<int32_t> hashes(std::max(1UL, numHashes), 0);
    // each function call has own best vector for parallel execution
    std::vector<int64_t> best(numHashes, std::numeric_limits<int64_t>::max());

    /**
     * According to repeatWeight, choose n smallest
     * with greater probability when repeats
     */
    for (const auto &mapEntry : hitMap) {

        int64_t key = mapEntry.first;
        int32_t weight = mapEntry.second.count;

        if (repeatWeight < 0.0f) {
            weight = 1.0f;
        }

        if (weight <= 0) {
            continue;
        }

        int64_t x = key;

        for (size_t word = 0U; word < numHashes; ++word) {
            //dunno which type is best here actually
            for (int32_t count = 0; count < weight; ++count) {
                // XORShift Random Number Generators
                // https://www.javamex.com/tutorials/random_numbers/xorshift.shtml
                // probably from here ^^
                x ^= (x << 21U);
                x ^= (int64_t) ((uint64_t) x >> 35U);
                x ^= (x << 4U);

                if (x < best[word]) {
                    best[word] = x;

                    if (word % 2 == 0) {
                        hashes[word] = static_cast<int32_t>(key);
                    } else {
                        hashes[word] = static_cast<int32_t>((int64_t) ((uint64_t) key >> 32U));
                        //unsigned shift no effect on unsigned value
                    }
                }
            }
        }
    }

    return hashes;
}