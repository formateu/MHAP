#ifndef SequenceStreamer_HPP
#define SequenceStreamer_HPP

#include <utility>
#include <vector>
#include <memory>
#include <fstream>
#include "Sequence.hpp"

using IstreamPtr = std::unique_ptr<std::istream>;

class SequenceStreamer {
public:
    //SequenceStreamer();
    SequenceStreamer(const std::string &filePath);
    SequenceStreamer(const SequenceStreamer &oth) = delete;
    SequenceStreamer(SequenceStreamer &&oth) = delete;

    bool enqueueNextSequence();

    void enqueueAll();

    SequencePtr dequeue();

    bool empty() const { return sequences_.empty() && readAll_; }

private:
    IstreamPtr seqStream_;
protected:
    std::vector<SequencePtr> sequences_;
    bool readAll_;
    size_t index_;
};

#endif //SequenceStreamer_HPP
