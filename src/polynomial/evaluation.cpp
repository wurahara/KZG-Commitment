#include "polynomial/evaluation.h"

#include <cassert>

namespace kzg::polynomial {

EvaluationForm::EvaluationForm(const EvaluationForm &poly) = default;

EvaluationForm::EvaluationForm(EvaluationForm &&poly) noexcept = default;

EvaluationForm::EvaluationForm(const std::vector<bls12_381::scalar::Scalar> &evaluations,
                               const domain::EvaluationDomain &domain)
        : evaluations{evaluations}, domain{domain} {}

EvaluationForm::EvaluationForm(std::vector<bls12_381::scalar::Scalar> &&evaluations, domain::EvaluationDomain &&domain)
        : evaluations{std::move(evaluations)}, domain{std::move(domain)} {}

CoefficientForm EvaluationForm::interpolate() const {
    auto temp_eval = this->evaluations;
    this->domain.inverse_fast_fourier_in_place(temp_eval);
    return CoefficientForm{evaluations};
}

EvaluationForm &EvaluationForm::operator=(const EvaluationForm &rhs) = default;

EvaluationForm &EvaluationForm::operator=(EvaluationForm &&rhs) noexcept = default;

EvaluationForm &EvaluationForm::operator+=(const EvaluationForm &polynomial) {
    assert(this->domain == polynomial.domain);
    for (int i = 0; i < this->evaluations.size(); ++i)
        this->evaluations[i] += polynomial.evaluations[i];
    return *this;
}

EvaluationForm &EvaluationForm::operator-=(const EvaluationForm &polynomial) {
    assert(this->domain == polynomial.domain);
    for (int i = 0; i < this->evaluations.size(); ++i)
        this->evaluations[i] -= polynomial.evaluations[i];
    return *this;
}

EvaluationForm &EvaluationForm::operator*=(const EvaluationForm &polynomial) {
    assert(this->domain == polynomial.domain);
    for (int i = 0; i < this->evaluations.size(); ++i)
        this->evaluations[i] *= polynomial.evaluations[i];
    return *this;
}

EvaluationForm &EvaluationForm::operator/=(const EvaluationForm &polynomial) {
    assert(this->domain == polynomial.domain);
    for (int i = 0; i < this->evaluations.size(); ++i)
        this->evaluations[i] *= polynomial.evaluations[i].invert().value();
    return *this;
}

} // namespace kzg::polynomial