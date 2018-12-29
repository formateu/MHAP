#include "../include/HashUtils.hpp"

#include <string_view>
#include <array>
#include <Sequence.hpp>
//#include <MurmurHash3.h>

#include <GM3cpp/Murmur3_32.hpp>
#include <GM3cpp/Murmur3_128.hpp>



template<typename T, class Hasher>
std::vector<T> computeSeqHashesGeneric(const std::string &seq,
                                       size_t nGramSize,
                                       int32_t seed,
                                       bool doReverseCompliment) {
    std::vector<T> hashes(seq.size() - nGramSize + 1);
    std::string_view seqView(seq);
    Hasher hasher(seed);
    /**
     * For each kmer in sequence
     * check if it is lexicography smaller than its reverse compliment
     * and replace if it is
     * hash kmer with Hasher object
     */
    for (size_t iter = 0; iter < hashes.size(); ++iter) {
        auto kmer = seqView.substr(iter, nGramSize);

        hashes[iter] = HashUtils::hashKmer(kmer, hasher, doReverseCompliment);

        hasher.reset();
    }

    return hashes;
}

//static method
std::vector<int64_t> HashUtils::computeSeqHashesLong(const std::string &seq,
                                                     size_t nGramSize,
                                                     int32_t seed,
                                                     bool doReverseCompliment) {

    return computeSeqHashesGeneric<int64_t, GM3cpp::Murmur3_128>(seq,
                                                                 nGramSize,
                                                                 seed,
                                                                 doReverseCompliment);
}

//static method
std::vector<int32_t> HashUtils::computeSeqHashes(const std::string &seq,
                                                 size_t nGramSize,
    //no seed here - fixed on 0U
                                                 bool doReverseCompliment) {
    return computeSeqHashesGeneric<int32_t, GM3cpp::Murmur3_32>(seq,
                                                                nGramSize,
                                                                0,
                                                                doReverseCompliment);
}