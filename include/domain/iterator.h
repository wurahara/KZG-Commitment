#ifndef KZG_COMMITMENT_ITERATOR_H
#define KZG_COMMITMENT_ITERATOR_H

#include <cassert>
#include <cstdint>
#include <iterator>
#include <optional>

#include "scalar/scalar.h"

#include "domain/domain.h"

namespace kzg::domain {

class ElementIterator {
private:
    EvaluationDomain domain;

    uint64_t power;
    uint64_t init_power;

    bls12_381::scalar::Scalar value;
    bls12_381::scalar::Scalar init_value;

public:
    ElementIterator() = delete;
    ElementIterator(const ElementIterator &element);
    ElementIterator(ElementIterator &&element) noexcept;
    ElementIterator(EvaluationDomain domain, uint64_t init_power, const bls12_381::scalar::Scalar &init_value);

public:
    using difference_type = uint64_t;
    using value_type = bls12_381::scalar::Scalar;
    using pointer = const bls12_381::scalar::Scalar *;
    using reference = const bls12_381::scalar::Scalar &;
    using iterator_category = std::forward_iterator_tag;

public:
    [[nodiscard]] size_t size() const;

public:
    [[nodiscard]] ElementIterator begin() const;
    [[nodiscard]] ElementIterator end() const;

public:
    ElementIterator &operator++();
    ElementIterator operator++(int); // NOLINT(cert-dcl21-cpp)

    bls12_381::scalar::Scalar operator*();

    ElementIterator &operator=(const ElementIterator &rhs);
    ElementIterator &operator=(ElementIterator &&rhs) noexcept;

public:
    friend inline bool operator==(const ElementIterator &a, const ElementIterator &b) {
        assert(a.domain == b.domain && a.init_power == b.init_power && a.init_value == b.init_value);
        return a.power == b.power;
    }
    friend inline bool operator!=(const ElementIterator &a, const ElementIterator &b) {
        assert(a.domain == b.domain && a.init_power == b.init_power && a.init_value == b.init_value);
        return a.power != b.power;
    }
};

} // namespace kzg::domain

#endif //KZG_COMMITMENT_ITERATOR_H