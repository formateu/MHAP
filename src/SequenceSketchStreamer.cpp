#include "../include/SequenceSketchStreamer.hpp"
SequenceSketchStreamer::SequenceSketchStreamer(const std::string &filePath,
                                               size_t minOlapLength,
                                               size_t kmerSize,
                                               size_t numHashes,
                                               size_t orderedKmerSize,
                                               size_t orderedSketchSize,
                                               bool doReverseCompliment,
                                               double repeatWeight)
    : fastaData_(filePath)
      , minOlapLength_(minOlapLength)
      , kmerSize_(kmerSize)
      , numHashes_(numHashes)
      , orderedKmerSize_(orderedKmerSize)
      , orderedSketchSize_(orderedSketchSize)
      , doReverseCompliment_(doReverseCompliment)
      , repeatWeight_(repeatWeight) {

}