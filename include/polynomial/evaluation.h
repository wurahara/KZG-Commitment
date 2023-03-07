#ifndef KZG_COMMITMENT_EVALUATION_H
#define KZG_COMMITMENT_EVALUATION_H

#include <vector>

#include "scalar/scalar.h"

#include "domain/domain.h"
#include "polynomial/coefficient.h"

namespace kzg::polynomial {

class EvaluationForm {
private:
    std::vector<bls12_381::scalar::Scalar> evaluations;
    domain::EvaluationDomain domain;

public:
    EvaluationForm() = delete;
    EvaluationForm(const EvaluationForm &poly);
    EvaluationForm(EvaluationForm &&poly) noexcept;

    EvaluationForm(const std::vector<bls12_381::scalar::Scalar> &evaluations, const domain::EvaluationDomain &domain);
    EvaluationForm(std::vector<bls12_381::scalar::Scalar> &&evaluations, domain::EvaluationDomain &&domain);

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
