/**
 * \file Sequence.hpp
 * \brief the C++ file with declaration of struct representing sequence.
 *
 */
#ifndef Sequence_HPP
#define Sequence_HPP

#include <algorithm>
#include <string>
#include <memory>
#include "SequenceId.hpp"

/**
 * @brief Struct representing sequence Identifier.
 */
struct Sequence {
    const std::string read; /**< raw sequence data. */
    const SequenceId id; /**< ordinal number in input stream. */

    /**
     * Creates Sequence object by copying given parameters.
     * @param seq raw sequence data.
     * @param id_ ordinal number in input stream.
     */
    Sequence(const std::string &seq, const SequenceId &id_) : read(seq), id(id_) {}

    /**
     * Creates Sequence object by moving given parameters.
     * @param seq raw sequence data.
     * @param id_ ordinal number in input stream.
     */
    Sequence(std::string &&seq, SequenceId &&id_) : read(seq), id(id_) {}

    /**
     * Generic function creating reverse-compliment string.
     * @tparam T sequence raw data type
     * (tested with std::string and std::string_view).
     * @param seq raw data object
     * @return string containing reverse-compliment data.
     */
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

    /**
     * Creates reverse-compliment Sequence object.
     * @return reverse-compliment Sequence object.
     */
    Sequence getReverseCompliment() { return Sequence(revCompSeq(read), id.complimentId()); }

    /**
     * Returns length of the underlying data.
     * @return length of sequence string.
     */
    size_t size() const { return read.size(); }
};

using SequencePtr = std::unique_ptr<Sequence>;

#endif //Sequence_HPP
