#ifndef IOError_HPP
#define IOError_HPP

#include <ios>

using std::ifstream;

class IOError : public ifstream::failure {
public:
    explicit IOError(const char *msg) noexcept : failure(msg) {}
    virtual const char *what() noexcept { return failure::what(); }
};

#endif //IOError_HPP
