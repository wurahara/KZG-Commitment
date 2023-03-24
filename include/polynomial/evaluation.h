#ifndef KZG_COMMITMENT_EVALUATION_H
#define KZG_COMMITMENT_EVALUATION_H

#include <vector>

#include "scalar/scalar.h"

#include "domain/domain.h"
#include "polynomial/coefficient.h"

namespace kzg::polynomial {

/**
 * @brief Represents a polynomial in evaluation form.
 */
class EvaluationForm {
private:
    /// The evaluations of the polynomial over a specific domain.
    std::vector<bls12_381::scalar::Scalar> evaluations;
    /// The evaluation domain of the polynomial.
    domain::EvaluationDomain domain;

public:
    EvaluationForm() = delete;
    EvaluationForm(const EvaluationForm &poly);
    EvaluationForm(EvaluationForm &&poly) noexcept;

    EvaluationForm(const std::vector<bls12_381::scalar::Scalar> &evaluations, const domain::EvaluationDomain &domain);
    EvaluationForm(std::vector<bls12_381::scalar::Scalar> &&evaluations, domain::EvaluationDomain &&domain);

    /**
     * @brief Interpolates the polynomial from its evaluations.
     * @return the interpolated polynomial in coefficient form.
     */
    [[nodiscard]] CoefficientForm interpolate() const;

public:
    EvaluationForm &operator=(const EvaluationForm &rhs);
    EvaluationForm &operator=(EvaluationForm &&rhs) noexcept;

    EvaluationForm &operator+=(const EvaluationForm &polynomial);
    EvaluationForm &operator-=(const EvaluationForm &polynomial);
    EvaluationForm &operator*=(const EvaluationForm &polynomial);
    EvaluationForm &operator/=(const EvaluationForm &polynomial);

    bls12_381::scalar::Scalar &operator[](size_t index);
    bls12_381::scalar::Scalar operator[](size_t index) const;

    [[nodiscard]] const std::vector<bls12_381::scalar::Scalar> &get_evaluations() const;
    [[nodiscard]] const domain::EvaluationDomain &get_domain() const;

    static std::optional<EvaluationForm> from_slice(const std::vector<uint8_t> &bytes);
    [[nodiscard]] std::vector<uint8_t> to_var_bytes() const;

public:
    friend inline EvaluationForm operator+(const EvaluationForm &a, const EvaluationForm &b) { return EvaluationForm(a) += b; }
    friend inline EvaluationForm operator-(const EvaluationForm &a, const EvaluationForm &b) { return EvaluationForm(a) -= b; }
    friend inline EvaluationForm operator*(const EvaluationForm &a, const EvaluationForm &b) { return EvaluationForm(a) *= b; }
    friend inline EvaluationForm operator/(const EvaluationForm &a, const EvaluationForm &b) { return EvaluationForm(a) /= b; }

    friend inline bool operator==(const EvaluationForm &a, const EvaluationForm &b) {
        return a.domain == b.domain && a.evaluations == b.evaluations;
    }
    friend inline bool operator!=(const EvaluationForm &a, const EvaluationForm &b) {
        return a.domain != b.domain || a.evaluations != b.evaluations;
    }
};

} // namespace kzg::polynomial

#endif //KZG_COMMITMENT_EVALUATION_H
