#ifndef KZG_COMMITMENT_EXCEPTION_H
#define KZG_COMMITMENT_EXCEPTION_H

#include <exception>
#include <string>

namespace kzg::exception {

enum class Type {
    INVALID_EVALUATION_DOMAIN_SIZE,
    TRUNCATED_DEGREE_IS_ZERO,
    TRUNCATED_DEGREE_TOO_LARGE,
    POLY_DEGREE_IS_ZERO,
    POLY_DEGREE_TOO_LARGE,
    CIRCUIT_DEGREE_IS_ZERO,
    SIZE_MISMATCH,
    SERIALIZE_NO_ENOUGH_BYTES,
};

class Exception: public std::exception {
private:
    Type type;
    std::string message;

public:
    Exception(Type type, std::string message);
    ~Exception() noexcept override;

    [[nodiscard]] Type get_type() const;
    [[nodiscard]] const std::string &get_message() const;
    [[nodiscard]] const char *what() const noexcept override;
    void set_type(Type type);
    void set_message(const std::string &message);
};

} // namespace kzg::exception

#endif //KZG_COMMITMENT_EXCEPTION_H