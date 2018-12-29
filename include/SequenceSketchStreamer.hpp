/**
 * \file SequenceSketchStreamer.hpp
 * \brief the C++ file with declaration of class responsible for
 * providing queue producer mechanism for sequence sketches.
 *
 */
#ifndef SequenceSketchStreamer_HPP
#define SequenceSketchStreamer_HPP

#include <deque>
#include <string>
#include "SequenceStreamer.hpp"
#include "SequenceSketch.hpp"

/**
 * @brief SequenceSketchStreamer class. Class responsible for
 * providing queue producer mechanism for sequence sketches.
 */
class SequenceSketchStreamer {
public:
    /**
     * Creates SequenceSketchStreamer object with given parameters.
     * @param filePath path to input sequence file.
     * @param startId Ordinal number of first sequence in file (sequence Id).
     * @param minOlapLength minimum length of the overlap between sequences.
     * @param kmerSize size of the k-mer in minhash sketch.
     * @param numHashes number of minhash sketches for sequence representation.
     * @param orderedKmerSize size of the k-mer in k-bottom sketch.
     * @param orderedSketchSize number of hash, position pairs in k-bottom sketch.
     * @param doReverseCompliment flag determining if reverse compliment sequences are processed.
     * @param repeatWeight TODO: fill.
     */
    SequenceSketchStreamer(const std::string &filePath,
                           size_t startId,
                           size_t minOlapLength,
                           size_t kmerSize,
                           size_t numHashes,
                           size_t orderedKmerSize,
                           size_t orderedSketchSize,
                           bool doReverseCompliment,
                           double repeatWeight);

    /**
     * Creates SequenceSketchStreamer object with given parameters.
     * First sequence id = 0.
     * @param filePath path to input sequence file.
     * @param minOlapLength minimum length of the overlap between sequences.
     * @param kmerSize size of the k-mer in minhash sketch.
     * @param numHashes number of minhash sketches for sequence representation.
     * @param orderedKmerSize size of the k-mer in k-bottom sketch.
     * @param orderedSketchSize number of hash, position pairs in k-bottom sketch.
     * @param doReverseCompliment flag determining if reverse compliment sequences are processed.
     * @param repeatWeight TODO: fill.
     */
    SequenceSketchStreamer(const std::string &filePath,
                           size_t minOlapLength,
                           size_t kmerSize,
                           size_t numHashes,
                           size_t orderedKmerSize,
                           size_t orderedSketchSize,
                           bool doReverseCompliment,
                           double repeatWeight);

    /**
     * Removes first element from sequence sketch queue.
     * Always tries to load sequence from SequenceStreamer according to fwdOnly param.
     * If there are no more sequence sketches, returns wrapped nullptr.
     * @param fwdOnly flag deremining if newly loaded sequence will
     * also be reverse-compliment(two sketches).
     * @return pointer to SequenceSketch object or nullptr.
     */
    SequenceSketchPtr dequeue(bool fwdOnly);

    /**
     * Loads all sequences from input stream and creates sketches.
     * @param fwdOnly flag determining if reverse-compliment sequence is also processed.
     */
    void enqueueFull(bool fwdOnly);

    /**
     * Wrapper function for creating SequenceSketch from Sequence based
     * on SequenceSketchStreamer parameters.
     * @param seq const reference to Sequence object.
     * @return pointer to SequenceSketch object created based on given Sequence.
     */
    SequenceSketchPtr getSketch(const Sequence &seq);
protected:
    /**
     * Loads sequence sketches for a single sequence.
     * Function can fail if there are no more sequences.
     * @param fwdOnly flag determining if reverse-compliment sequence is also processed.
     * @return if enqueue succeed.
     */
    bool enqueue(bool fwdOnly);
    //TODO: maybe remove this function?
    bool enqueueUntilFound(bool fwdOnly);

    std::deque<SequenceSketchPtr> sequenceHashDeque_; /**< internal queue with sequence sketches. */
    SequenceStreamer fastaData_; /**< object processing sequences from input stream.*/
    size_t minOlapLength_; /**< minimum length of the overlap between sequences. */
    size_t kmerSize_; /**< kmerSize size of the k-mer in minhash sketch. */
    size_t numHashes_; /**< number of minhash sketches for sequence representation. */
    size_t orderedKmerSize_; /**< size of the k-mer in k-bottom sketch.*/
    size_t orderedSketchSize_; /**< number of hash, position pairs in k-bottom sketch. */
    bool
        doReverseCompliment_; /**< flag determining if reverse compliment sequences are processed.*/
    double repeatWeight_; /**< TODO: fill.*/
};

#endif //SequenceSketchStreamer_HPP
