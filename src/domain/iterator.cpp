#include "domain/iterator.h"

#include <utility>

namespace kzg::domain {

using bls12_381::scalar::Scalar;

ElementIterator::ElementIterator(const ElementIterator &element) = default;

ElementIterator::ElementIterator(ElementIterator &&element) noexcept = default;

ElementIterator::ElementIterator(EvaluationDomain domain, uint64_t init_power, const Scalar &init_value)
        : domain{std::move(domain)}, power{init_power}, init_power{init_power}, value{init_value},
          init_value{init_value} {}

size_t ElementIterator::size() const {
    size_t size = 0;
    for (const Scalar &ele: *this) size++;
    return size;
}

ElementIterator ElementIterator::begin() const {
    ElementIterator temp{*this};
    temp.power = temp.init_power;
    temp.value = temp.init_value;
    return temp;
}

ElementIterator ElementIterator::end() const {
    ElementIterator temp{*this};
    temp.power = temp.domain.size();
    return temp;
}

ElementIterator &ElementIterator::operator++() {
    this->power += 1;
    this->value *= this->domain.group_generator();
    return *this;
}

ElementIterator ElementIterator::operator++(int) { // NOLINT(cert-dcl21-cpp)
    ElementIterator res{*this};
    ++(*this);
    return res;
}

Scalar ElementIterator::operator*() {
    return this->value;
}

ElementIterator &ElementIterator::operator=(const ElementIterator &rhs) = default;

ElementIterator &ElementIterator::operator=(ElementIterator &&rhs) noexcept = default;

} // namespace kzg::domain