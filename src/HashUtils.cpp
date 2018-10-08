#include "../include/HashUtils.hpp"

#include <string_view>
#include <array>
#include "../include/Sequence.hpp"
#include "../include/MurmurHash3.h"

//TODO: avoid copypaste in these 2 functions

//static method
std::vector<uint64_t> HashUtils::computeSeqHashesLong(const std::string &seq,
                                                     size_t nGramSize,
                                                     uint32_t seed,
                                                     bool doReverseCompliment) {

//    void MurmurHash3_x64_128 ( const void * key, int len, uint32_t seed, void * out );
    std::vector<uint64_t> hashes(seq.size() - nGramSize + 1);

    std::string_view seqView(seq);
    std::array<uint64_t, 2> tempHashOut{0UL, 0UL};

    /**
     * For each kmer in sequence
     * check if it is lexicography smaller than its reverse compliment
     * and replace if it is
     * hash kmer with Murmur3 x64_128
     */
    for (size_t iter = 0; iter < hashes.size(); ++iter) {
        auto kmer = seqView.substr(iter, nGramSize);
        std::string kmerRevCompl;

        if (doReverseCompliment) {
            kmerRevCompl = Sequence::revCompSeq(kmer);

            if (kmerRevCompl.compare(kmer) < 0) {
                kmer = kmerRevCompl;
            }
        }

        //implicit cast : uint64_t -> int64_t
        MurmurHash3_x64_128(kmer.data(), kmer.size(), seed, tempHashOut.data());
        //actually does not matter which one
        hashes[iter] = tempHashOut[1];
    }

    return hashes;
}

//static method
std::vector<uint32_t> HashUtils::computeSeqHashes(const std::string &seq,
                                                 size_t nGramSize,
    //no seed here - fixed on 0U
                                                 bool doReverseCompliment) {
//    void MurmurHash3_x86_32  ( const void * key, int len, uint32_t seed, void * out );
    std::vector<uint32_t> hashes(seq.size() - nGramSize + 1);

    std::string_view seqView(seq);
    uint32_t tempHashOut{0};

    /**
     * For each kmer in sequence
     * check if it is lexicography smaller than its reverse compliment
     * and replace if it is
     * hash kmer with Murmur3 (x86_32)
     */
    for (size_t iter = 0; iter < hashes.size(); ++iter) {
        auto kmer = seqView.substr(iter, nGramSize);
        std::string kmerRevCompl;

        if (doReverseCompliment) {
            kmerRevCompl = Sequence::revCompSeq(kmer);

            if (kmerRevCompl.compare(kmer) < 0) {
                kmer = kmerRevCompl;
            }
        }

        MurmurHash3_x86_32(kmer.data(), kmer.size(), 0U, &tempHashOut);
        hashes[iter] = tempHashOut;
    }

    return hashes;
}