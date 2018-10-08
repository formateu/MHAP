#ifndef Sequence_HPP
#define Sequence_HPP

#include <algorithm>
#include <string>
#include <memory>
//#include <iostream> TODO: remove

#include "SequenceId.hpp"

struct Sequence {
    const std::string read;
    const SequenceId id;

    Sequence(const std::string &seq, const SequenceId &id_) : read(seq), id(id_) {}
    Sequence(std::string &&seq, SequenceId &&id_) : read(seq), id(id_) {}

    template <class T>
    static std::string revCompSeq(const T &seq) {
        std::string reverseCompliment(seq);

        for (auto &base : reverseCompliment) {
            switch (base) {
                case 'C':
                    base = 'G';
                    break;
                case 'G':
                    base = 'C';
                    break;
                case 'A':
                    base = 'T';
                    break;
                case 'T':
                    base = 'A';
                    break;
            }
        }

        std::reverse(reverseCompliment.begin(), reverseCompliment.end());

        return reverseCompliment;
    }

    Sequence getReverseCompliment() { return Sequence(revCompSeq(read), id.complimentId()); }

    size_t size() const { return read.size(); }
};

using SequencePtr = std::unique_ptr<Sequence>;

#endif //Sequence_HPP
