#ifndef KZG_COMMITMENT_DOMAIN_H
#define KZG_COMMITMENT_DOMAIN_H

#include <cstdint>
#include <vector>
#include <optional>

#include "scalar/scalar.h"

#include "domain/fourier.h"

namespace kzg::polynomial { class EvaluationForm; }
namespace kzg::domain { class ElementIterator; }

namespace kzg::domain {

/**
 * @brief Defines a domain over which finite field FFTs can be efficiently performed.
 *          Works only for fields which have a large multiplicative subgroup of size which is a power of 2.
 */
class EvaluationDomain {
public:
    static constexpr int32_t BYTE_SIZE = bls12_381::scalar::Scalar::BYTE_SIZE + sizeof(uint64_t);

private:
    /// a generator of the subgroup
    bls12_381::scalar::Scalar group_gen;
    /// the size of the domain
    uint64_t domain_size;

public:
    EvaluationDomain();
    EvaluationDomain(const EvaluationDomain &domain);
    EvaluationDomain(EvaluationDomain &&domain) noexcept;
    explicit EvaluationDomain(uint64_t num_of_coefficients);
    EvaluationDomain(bls12_381::scalar::Scalar generator, uint64_t size);

    [[nodiscard]] auto size() const noexcept -> size_t;

    /// Log of the size of the domain.
    [[nodiscard]] auto log_size() const noexcept -> size_t;
    /// Size of the domain (as a field element).
    [[nodiscard]] auto size_as_field_element() const -> bls12_381::scalar::Scalar;
    /// Inverse of the size of the domain (as a field element).
    [[nodiscard]] auto size_inverse() const -> bls12_381::scalar::Scalar;
    /// A generator of the multiplicative subgroup.
    [[nodiscard]] auto group_generator() const -> bls12_381::scalar::Scalar;
    /// Inverse of the generator of the subgroup.
    [[nodiscard]] auto group_generator_inverse() const -> bls12_381::scalar::Scalar;

    [[nodiscard]] auto iter() const -> ElementIterator;

    [[nodiscard]] auto fast_fourier(std::vector<bls12_381::scalar::Scalar> &coefficients) const -> std::vector<bls12_381::scalar::Scalar>;
    [[nodiscard]] auto inverse_fast_fourier(std::vector<bls12_381::scalar::Scalar> &evaluations) const -> std::vector<bls12_381::scalar::Scalar>;
    [[nodiscard]] auto coset_fast_fourier(std::vector<bls12_381::scalar::Scalar> &coefficients) const -> std::vector<bls12_381::scalar::Scalar>;
    [[nodiscard]] auto coset_inverse_fast_fourier(std::vector<bls12_381::scalar::Scalar> &evaluations) const -> std::vector<bls12_381::scalar::Scalar>;

    void fast_fourier_in_place(std::vector<bls12_381::scalar::Scalar> &coefficients) const;
    void inverse_fast_fourier_in_place(std::vector<bls12_381::scalar::Scalar> &evaluations) const;
    void coset_fast_fourier_in_place(std::vector<bls12_381::scalar::Scalar> &coefficients) const;
    void coset_inverse_fast_fourier_in_place(std::vector<bls12_381::scalar::Scalar> &evaluations) const;

    /**
     * @brief Evaluates the vanishing polynomial defined by this domain at a given point.
     * @details For a multiplicative subgroup, the vanishing polynomial should be in the form of z(X) = X ^ size - 1.
     * @param tau the given point to be evaluated
     * @return the evaluation results.
     */
    [[nodiscard]] auto evaluate_vanishing_polynomial(const bls12_381::scalar::Scalar &tau) const -> bls12_381::scalar::Scalar;

    /**
     * @brief Evaluates the vanishing polynomial defined by this domain at S points, where S is the size of the domain.
     * @param poly_degree degree of the vanishing polynomial
     * @return the evaluation results in evaluation form
     */
    [[nodiscard]] auto evaluate_vanishing_polynomial_over_coset(uint64_t poly_degree) const -> polynomial::EvaluationForm;

    /**
     * @brief Evaluates all the Lagrange polynomials defined by this domain at a given point.
     * @param tau the given point to be evaluated
     * @return the evaluation results
     */
    [[nodiscard]] auto evaluate_all_lagrange_coefficients(const bls12_381::scalar::Scalar &tau) const -> std::vector<bls12_381::scalar::Scalar>;

    static std::optional<EvaluationDomain> from_bytes(const std::array<uint8_t, EvaluationDomain::BYTE_SIZE> &bytes);
    [[nodiscard]] std::array<uint8_t, EvaluationDomain::BYTE_SIZE> to_bytes() const;

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