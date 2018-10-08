#ifndef SequenceSketchStreamer_HPP
#define SequenceSketchStreamer_HPP

#include "SequenceStreamer.hpp"
#include <string>

class SequenceSketchStreamer {
public:
    SequenceSketchStreamer(const std::string &filePath,
                           size_t minOlapLength,
                           size_t kmerSize,
                           size_t numHashes,
                           size_t orderedKmerSize,
                           size_t orderedSketchSize,
                           bool doReverseCompliment,
                           double repeatWeight);
private:
    SequenceStreamer fastaData_;
    size_t minOlapLength_;
    size_t kmerSize_;
    size_t numHashes_;
    size_t orderedKmerSize_;
    size_t orderedSketchSize_;
    bool doReverseCompliment_;
    double repeatWeight_;

};

#endif //SequenceSketchStreamer_HPP
