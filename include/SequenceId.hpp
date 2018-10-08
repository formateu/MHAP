#ifndef SequenceId_HPP
#define SequenceId_HPP

struct SequenceId {
    const std::size_t id;
    const bool isFwd;

    SequenceId(const std::size_t &id_, bool isFwd_) : id(id_), isFwd(isFwd_) {}
    SequenceId(const std::size_t &id) : SequenceId(id, true) {} //default forward

    bool operator==(const SequenceId &rhs) const {
        return id == rhs.id && isFwd == rhs.isFwd;
    }
    //TODO: hashCode?

    SequenceId complimentId() const { return SequenceId(id, !isFwd); }
};

#endif //SequenceId_HPP
