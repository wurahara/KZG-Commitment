#include "polynomial/evaluation.h"

#include <cassert>

namespace kzg::polynomial {

using bls12_381::scalar::Scalar;
using domain::EvaluationDomain;

EvaluationForm::EvaluationForm(const EvaluationForm &poly) = default;

EvaluationForm::EvaluationForm(EvaluationForm &&poly) noexcept = default;

EvaluationForm::EvaluationForm(const std::vector<Scalar> &evaluations, const EvaluationDomain &domain)
        : evaluations{evaluations}, domain{domain} {}

EvaluationForm::EvaluationForm(std::vector<Scalar> &&evaluations, EvaluationDomain &&domain)
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

bls12_381::scalar::Scalar &EvaluationForm::operator[](size_t index) {
    assert(index < this->evaluations.size());
    return this->evaluations[index];
}

bls12_381::scalar::Scalar EvaluationForm::operator[](size_t index) const {
    assert(index < this->evaluations.size());
    return this->evaluations[index];
}

const std::vector<bls12_381::scalar::Scalar> &EvaluationForm::get_evaluations() const {
    return this->evaluations;
}

const EvaluationDomain &EvaluationForm::get_domain() const {
    return this->domain;
}

} // namespace kzg::polynomial