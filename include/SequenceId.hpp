/**
 * \file SequenceId.hpp
 * \brief the C++ file with declaration of class representing
 * sequence Identifier.
 *
 */
#ifndef SequenceId_HPP
#define SequenceId_HPP

#include <limits>
#include <string>

/**
 * @brief Struct representing sequence Identifier.
 */
struct SequenceId {
    std::size_t id; /**< unrderlying id (ordinal number in input stream). */
    bool isFwd; /**< flag determining whether sequence is reverse-compliment. */

    /**
     * Creates a SequenceId object based on given parameters.
     * @param id_ ordinal sequence number in input stream.
     * @param isFwd_ flag determining if sequence is reverse-compliment.
     */
    SequenceId(const std::size_t &id_, bool isFwd_) : id(id_), isFwd(isFwd_) {}

    /**
     * Creates SequenceId object assuming that sequence is forward.
     * @param id ordinal sequence number in input stream.
     */
    SequenceId(const std::size_t &id) : SequenceId(id, true) {} //default forward

    /**
     * Comparison operator for SequenceId class.
     * Tests whether id and orientation matches between identifiers.
     * @param rhs right-hand side Sequence id object.
     * @return if objects are referring to the same sequence.
     */
    bool operator==(const SequenceId &rhs) const {
        return id == rhs.id && isFwd == rhs.isFwd;
    }

    /**
     * Creates reverse-compliment identfier.
     * @return reverse-compliment SequenceId object.
     */
    SequenceId complimentId() const { return SequenceId(id, !isFwd); }

    /**
     * "toString" method applied on ordinal number.
     * @return identifier as string.
     */
    std::string getHeader() const { return std::to_string(id); }
};

/**
 * @brief Functor used to hash SequenceId class objects.
 */
struct SequenceIdHasher {
    /**
     * Functional call operator used by collections to hash SequenceId object.
     * @param toHash SequenceId object which will be hashed.
     * @return hash value represented by unsigned 32-bit number.
     */
    uint32_t operator()(const SequenceId &toHash) const {
        auto id = static_cast<uint32_t>(toHash.id);

        return toHash.isFwd ? std::numeric_limits<uint32_t>::max() - id : id;
    }
};

#endif //SequenceId_HPP
