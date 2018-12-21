/**
 * @file IOError.hpp
 * @brief the C++ file with declaration of IO exception class.
 */
#ifndef IOError_HPP
#define IOError_HPP

#include <ios>

using std::ifstream;

/**
 * @brief IO exception class.
 */
class IOError : public ifstream::failure {
public:
    /**
     * Creates IOError exception object with passed message.
     * @param msg exception error message.
     */
    explicit IOError(const char *msg) noexcept : failure(msg) {}

    using failure::what; //prevent -Woverloaded-virtual
    /**
     * Method called when exception occurs.
     * @return error message.
     */
    const char *what() noexcept { return failure::what(); }
};

#endif //IOError_HPP
