#include <SequenceStreamer.hpp>
#include <IOError.hpp>

SequenceStreamer::SequenceStreamer(const std::string &filePath)
    : seqStream_(std::make_unique<std::ifstream>(filePath)), readAll_(false), index_(0U) {
    if (!seqStream_->good()) {
        throw IOError("IO Error during reading from input file.");
    }
}

bool SequenceStreamer::enqueueNextSequence() {
    std::string lastLine, read;

    if (readAll_) {
        return false;
    }

    while (std::getline(*seqStream_.get(), lastLine)) {
        if (lastLine[0] == '>') {
            if (read.empty()) {
                continue;
            } else {
                sequences_.emplace_back(std::make_unique<Sequence>(read, SequenceId(index_)));
                ++index_;
                return true;
            }
        } else {
            read += lastLine;
            lastLine.clear();
        }
    }

    if (!read.empty()) {
        sequences_.emplace_back(std::make_unique<Sequence>(read, SequenceId(index_)));
        readAll_ = true;
        return true;
    }

    return false;
}

void SequenceStreamer::enqueueAll() { while (enqueueNextSequence()) {}}

SequencePtr SequenceStreamer::dequeue() {
    if (sequences_.empty()) {
        enqueueNextSequence();
    }

    SequencePtr seq{nullptr};

    if (!sequences_.empty()) {
        seq = std::move(sequences_.back());
        sequences_.pop_back();
    }

    return seq;
}