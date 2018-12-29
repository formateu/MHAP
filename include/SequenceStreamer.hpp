/**
 * \file SequenceStreamer.hpp
 * \brief the C++ file with declaration of class responsible for
 * handling sequence input stream operations.
 *
 */
#ifndef SequenceStreamer_HPP
#define SequenceStreamer_HPP

#include <utility>
#include <vector>
#include <memory>
#include <fstream>
#include "Sequence.hpp"

using IstreamPtr = std::unique_ptr<std::istream>;

/**
 * @brief SequenceStreamer class. Class responsible for
 * handling sequence input stream operations.
 */
class SequenceStreamer {
public:
    /**
     * Creates SequenceStreamer basing on input file path,
     * Sequences are enumerated starting from startIndex
     * @param filePath path to input file containing sequences.
     * @param startIndex first sequence in file index
     */
    explicit SequenceStreamer(const std::string &filePath, size_t startIndex);

    /**
     * Creates SequenceStreamer basing on input file path.
     * Sequences are enumerated starting from 0.
     * @param filePath path to input file containing sequences.
     */
    explicit SequenceStreamer(const std::string &filePath);

    /**
     * Deleted copy constructor for SequenceStreamer class.
     * @param oth const reference to other SequenceStreamer object.
     */
    SequenceStreamer(const SequenceStreamer &oth) = delete;

    /**
     * Deleted move constructor for SequenceStreamer class.
     * @param oth r-value reference to other SequenceStreamer object;
     */
    SequenceStreamer(SequenceStreamer &&oth) = delete;

    /**
     * Loads next sequence from input stream into internal sequence vector.
     * @return if sequence load succeed.
     */
    bool enqueueNextSequence();

    /**
     * Loads all sequences from input stream into internal sequence vector.
     */
    void enqueueAll();

    /**
     * Removes last sequence from internal sequence vector.
     * If vector is empty, tries to load next sequence from stream.
     * If vector and stream are empty, returns wrapped nullptr.
     * @return std::unique_ptr with Sequence object or nullptr.
     */
    SequencePtr dequeue();

    /**
     * Checks whether there are no more sequences to load.
     * @return if there are no more sequences to load.
     */
    bool empty() const { return sequences_.empty() && readAll_; }

private:
    IstreamPtr seqStream_; /**< pointer to input stream. */
protected:
    std::vector<SequencePtr> sequences_; /**< internal sequence vector. */
    bool readAll_; /**< input steram availability. */
    size_t index_; /**< current sequence id.*/
};

#endif //SequenceStreamer_HPP
