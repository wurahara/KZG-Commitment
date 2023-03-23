#include "exception/exception.h"

#include <utility>

namespace kzg::exception {

Exception::Exception(Type type, std::string message) : type{type}, message{std::move(message)} {}

Exception::~Exception() noexcept = default;

const char *Exception::what() const noexcept {
    return this->message.c_str();
}

Type Exception::get_type() const {
    return this->type;
}

const std::string &Exception::get_message() const {
    return this->message;
}

void Exception::set_type(Type new_type) {
    this->type = new_type;
}

void Exception::set_message(const std::string &new_message) {
    this->message = new_message;
}

} // namespace kzg::exception