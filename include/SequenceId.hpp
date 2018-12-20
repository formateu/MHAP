#ifndef SequenceId_HPP
#define SequenceId_HPP

#include <limits>

struct SequenceId {
    const std::size_t id;
    const bool isFwd;

    SequenceId(const std::size_t &id_, bool isFwd_) : id(id_), isFwd(isFwd_) {}
    SequenceId(const std::size_t &id) : SequenceId(id, true) {} //default forward

    bool operator==(const SequenceId &rhs) const {
        return id == rhs.id && isFwd == rhs.isFwd;
    }

    SequenceId complimentId() const { return SequenceId(id, !isFwd); }
    std::string getHeader() const { return std::to_string(id); }
};

struct SequenceIdHasher {
    uint32_t operator()(const SequenceId &toHash) const {
        uint32_t id = static_cast<uint32_t>(toHash.id);

        return toHash.isFwd ? std::numeric_limits<uint32_t>::max() - id : id;
    }
};

#endif //SequenceId_HPP
