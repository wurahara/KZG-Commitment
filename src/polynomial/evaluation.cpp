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

std::optional<EvaluationForm> EvaluationForm::from_slice(const std::vector<uint8_t> &bytes) {
    std::array<uint8_t, EvaluationDomain::BYTE_SIZE> domain_bytes{};

    const size_t eval_count = (bytes.size() - EvaluationDomain::BYTE_SIZE) / Scalar::BYTE_SIZE;
    std::vector<Scalar> evals;
    evals.reserve(eval_count);

    for (int i = 0; i < bytes.size() - EvaluationDomain::BYTE_SIZE; i += Scalar::BYTE_SIZE) {
        std::array<uint8_t, Scalar::BYTE_SIZE> eval_byte{};
        std::copy(bytes.begin() + i, bytes.begin() + i + Scalar::BYTE_SIZE, eval_byte.begin());
        const auto scalar_opt = Scalar::from_bytes(eval_byte);
        if (!scalar_opt.has_value())
            return std::nullopt;
        evals.push_back(scalar_opt.value());
    }

    std::copy(bytes.begin() + static_cast<uint32_t>(eval_count) * Scalar::BYTE_SIZE, bytes.end(), domain_bytes.begin());
    const auto domain_opt = EvaluationDomain::from_bytes(domain_bytes);
    if (!domain_opt.has_value()) return std::nullopt;

    return EvaluationForm{evals, domain_opt.value()};
}

std::vector<uint8_t> EvaluationForm::to_var_bytes() const {
    std::vector<uint8_t> res{};
    res.reserve(EvaluationDomain::BYTE_SIZE + this->evaluations.size() * Scalar::BYTE_SIZE);

    for (const Scalar &eval: this->evaluations) {
        const std::array<uint8_t, Scalar::BYTE_SIZE> eval_bytes = eval.to_bytes();
        res.insert(res.end(), eval_bytes.begin(), eval_bytes.end());
    }

    const std::array<uint8_t, EvaluationDomain::BYTE_SIZE> domain_bytes = this->domain.to_bytes();
    res.insert(res.end(), domain_bytes.begin(), domain_bytes.end());

    return res;
}

} // namespace kzg::polynomial