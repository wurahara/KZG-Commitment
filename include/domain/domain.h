#ifndef KZG_COMMITMENT_DOMAIN_H
#define KZG_COMMITMENT_DOMAIN_H

#include <cstdint>
#include <vector>

#include "scalar/scalar.h"

#include "domain/fourier.h"

namespace kzg::polynomial { class EvaluationForm; }
namespace kzg::domain { class ElementIterator; }

namespace kzg::domain {

class EvaluationDomain {
private:
    bls12_381::scalar::Scalar group_gen;
    uint64_t domain_size;

public:
    EvaluationDomain();
    EvaluationDomain(const EvaluationDomain &domain);
    EvaluationDomain(EvaluationDomain &&domain) noexcept;
    explicit EvaluationDomain(uint64_t num_of_coefficients);

    [[nodiscard]] size_t size() const noexcept;
    [[nodiscard]] size_t log_size() const noexcept;

    [[nodiscard]] bls12_381::scalar::Scalar size_as_field_element() const;
    [[nodiscard]] bls12_381::scalar::Scalar size_inverse() const;
    [[nodiscard]] bls12_381::scalar::Scalar group_generator() const;
    [[nodiscard]] bls12_381::scalar::Scalar group_generator_inverse() const;

    [[nodiscard]] ElementIterator iter() const;

    [[nodiscard]] std::vector<bls12_381::scalar::Scalar> fast_fourier(std::vector<bls12_381::scalar::Scalar> &coefficients) const;
    [[nodiscard]] std::vector<bls12_381::scalar::Scalar> inverse_fast_fourier(std::vector<bls12_381::scalar::Scalar> &evaluations) const;
    [[nodiscard]] std::vector<bls12_381::scalar::Scalar> coset_fast_fourier(std::vector<bls12_381::scalar::Scalar> &coefficients) const;
    [[nodiscard]] std::vector<bls12_381::scalar::Scalar> coset_inverse_fast_fourier(std::vector<bls12_381::scalar::Scalar> &evaluations) const;

    void fast_fourier_in_place(std::vector<bls12_381::scalar::Scalar> &coefficients) const;
    void inverse_fast_fourier_in_place(std::vector<bls12_381::scalar::Scalar> &evaluations) const;
    void coset_fast_fourier_in_place(std::vector<bls12_381::scalar::Scalar> &coefficients) const;
    void coset_inverse_fast_fourier_in_place(std::vector<bls12_381::scalar::Scalar> &evaluations) const;

    [[nodiscard]] bls12_381::scalar::Scalar evaluate_vanishing_polynomial(const bls12_381::scalar::Scalar &tau) const;
    [[nodiscard]] polynomial::EvaluationForm evaluate_vanishing_polynomial_over_coset(uint64_t poly_degree) const;
    [[nodiscard]] std::vector<bls12_381::scalar::Scalar> evaluate_all_lagrange_coefficients(const bls12_381::scalar::Scalar &tau) const;

public:
    EvaluationDomain &operator=(const EvaluationDomain &rhs);
    EvaluationDomain &operator=(EvaluationDomain &&rhs) noexcept;

    friend inline bool operator==(const EvaluationDomain &a, const EvaluationDomain &b) {
        return a.group_gen == b.group_gen && a.domain_size == b.domain_size;
    }
    friend inline bool operator!=(const EvaluationDomain &a, const EvaluationDomain &b) {
        return a.group_gen != b.group_gen || a.domain_size != b.domain_size;
    }
};

} // namespace kzg::domain

#endif //KZG_COMMITMENT_DOMAIN_H