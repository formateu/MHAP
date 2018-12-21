/**
 * \file SequenceSketch.hpp
 * \brief the C++ file with declaration of class representing
 * sequence sketch (minhash and k-bottom sketch).
 *
 */
#ifndef SequenceSketch_HPP
#define SequenceSketch_HPP

#include "Sequence.hpp"
#include "MinHashSketch.hpp"
#include "BottomOverlapSketch.hpp"

/**
 * @brief Class representing
 * sequence sketch (minhash and k-bottom sketch).
 */
class SequenceSketch {
public:
    /**
     * Creates SequenceSketch object based on given parameters.
     * @param seq sequence which sketch will be representing.
     * @param kmerSize size of the k-mer in minhash sketch.
     * @param numHashes number of k-mers representing sequence in minhash sketch.
     * @param orderedKmerSize size of the k-mer in k-bottom sketch.
     * @param orderedSketchSize number of hash,position pairs in k-bottom sketch.
     * @param doReverseCompliment flag determining if reverse-compliment sequences TODO: UNUSED
     * will also be processed
     * @param repeatWeight TODO: fill here
     */
    SequenceSketch(const Sequence &seq,
                   size_t kmerSize,
                   size_t numHashes,
                   size_t orderedKmerSize,
                   size_t orderedSketchSize,
//                   bool doReverseCompliment,
                   double repeatWeight);

    /**
     * Deleted copy constructor for SequenceSketch class.
     * @param oth const reference to other SequenceSketch object.
     */
    SequenceSketch(const SequenceSketch &oth) = delete;

    /**
     * Deleted move constructor for SequenceSketch class.
     * @param oth r-valie reference to other SequenceSketch object.
     */
    SequenceSketch(SequenceSketch &&oth) = default;

    /**
     * Accessor to internal MinHash object (read-only).
     * @return reference to internal MinHash object (read-only).
     */
    const MinHashSketch &getMinHashes() const { return mainHashes_; }

    /**
     * Accessor to internal k-bottom-sketch object (read-only).
     * @return reference to internal k-bottom-sketch object (read-only).
     */
    const BottomOverlapSketch &getOrderedHashes() const { return orderedHashes_; }

    /**
     * Accessor to sequence Id (read-only).
     * @return reference to sequence id (read-only).
     */
    const SequenceId &getSequenceId() const { return id_; }
    /**
     * Returns size of the original sequence represented by sketch.
     * @return size of the original sequence.
     */
    size_t getSequenceSize() const { return sequenceSize_; }
private:
    size_t sequenceSize_; /**< size of the original sequence.*/
    SequenceId id_; /**< Id of the sequence.*/
    MinHashSketch mainHashes_; /**< MinHash sketch of the sequence. */
    BottomOverlapSketch orderedHashes_; /**< K-Bottom sketch of the sequence. */
};

using SequenceSketchPtr = std::unique_ptr<SequenceSketch>;

#endif //SequenceSketch_HPP
