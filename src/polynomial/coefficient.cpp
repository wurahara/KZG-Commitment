#include "polynomial/coefficient.h"

#include <cassert>

#include "util/field.h"

#include "domain/domain.h"
#include "polynomial/evaluation.h"

namespace kzg::polynomial {

CoefficientForm::CoefficientForm() : coefficients{} {}

CoefficientForm::CoefficientForm(const CoefficientForm &rhs) = default;

CoefficientForm::CoefficientForm(CoefficientForm &&rhs) noexcept = default;

CoefficientForm::CoefficientForm(const std::vector<bls12_381::scalar::Scalar> &coefficients)
        : coefficients{coefficients} {
    this->trim_leading_zeros();
    assert(this->coefficients.empty() || !this->coefficients.back().is_zero());
}

CoefficientForm::CoefficientForm(std::vector<bls12_381::scalar::Scalar> &&coefficients)
        : coefficients{std::move(coefficients)} {
    this->trim_leading_zeros();
    assert(this->coefficients.empty() || !this->coefficients.back().is_zero());
}

CoefficientForm CoefficientForm::zero() {
    return CoefficientForm{{}};
}

CoefficientForm CoefficientForm::random(size_t degree) {
    std::vector<bls12_381::scalar::Scalar> rand_coeffs;
    rand_coeffs.reserve(degree + 1);
    for (int i = 0; i <= degree; ++i) {
        rand_coeffs.push_back(bls12_381::scalar::Scalar::random());
    }
    return CoefficientForm{rand_coeffs};
}

void CoefficientForm::trim_leading_zeros() {
    while (!this->coefficients.empty() && this->coefficients.back().is_zero())
        this->coefficients.pop_back();
}

bool CoefficientForm::is_zero() const {
    bool flag = true;
    for (const bls12_381::scalar::Scalar &coeff: this->coefficients)
        if (!coeff.is_zero())
            flag = false;
    return this->coefficients.empty() || flag;
}

size_t CoefficientForm::degree() const {
    if (this->is_zero())
        return 0;
    assert(!this->coefficients.back().is_zero());
    return this->coefficients.size() - 1;
}

bls12_381::scalar::Scalar CoefficientForm::evaluate(const bls12_381::scalar::Scalar &point) const {
    if (this->is_zero())
        return bls12_381::scalar::Scalar::zero();
    auto monomials = util::field::generate_vec_powers(point, this->degree());
    assert(monomials.size() == this->coefficients.size());

    bls12_381::scalar::Scalar res = bls12_381::scalar::Scalar::zero();
    for (int i = 0; i < this->coefficients.size(); ++i)
        res += (this->coefficients[i] * monomials[i]);
    return res;
}

CoefficientForm CoefficientForm::ruffini(const bls12_381::scalar::Scalar &point) const {
    std::vector<bls12_381::scalar::Scalar> quotient;
    quotient.reserve(this->degree());
    bls12_381::scalar::Scalar k = bls12_381::scalar::Scalar::zero();
    for (auto iter = this->coefficients.rbegin(); // NOLINT(modernize-loop-convert)
         iter != this->coefficients.rend(); iter++) {
        bls12_381::scalar::Scalar t = *iter + k;
        quotient.push_back(t);
        k = point * t;
    }
    if (!quotient.empty()) {
        quotient.pop_back();
        std::reverse(quotient.begin(), quotient.end());
    }
    return CoefficientForm{quotient};
}

std::vector<bls12_381::scalar::Scalar> CoefficientForm::get_coefficients() const {
    return this->coefficients;
}

CoefficientForm &CoefficientForm::operator=(CoefficientForm &&rhs) noexcept = default;

CoefficientForm &CoefficientForm::operator=(const CoefficientForm &rhs) = default;

CoefficientForm CoefficientForm::operator-() const {
    CoefficientForm res{*this};
    for (auto &coefficient: res.coefficients)
        coefficient = -coefficient;
    return res;
}

CoefficientForm &CoefficientForm::operator+=(const CoefficientForm &polynomial) {
    if (this->is_zero()) {
        this->coefficients.clear();
        this->coefficients.assign(polynomial.coefficients.begin(), polynomial.coefficients.end());
    } else if (polynomial.is_zero()) {

    } else if (this->degree() >= polynomial.degree()) {
        for (int i = 0; i < this->coefficients.size(); ++i)
            this->coefficients[i] += polynomial.coefficients[i];
    } else {
        this->coefficients.resize(polynomial.coefficients.size(), bls12_381::scalar::Scalar::zero());
        for (int i = 0; i < this->coefficients.size(); ++i)
            this->coefficients[i] += polynomial.coefficients[i];
        this->trim_leading_zeros();
    }
    return *this;
}

CoefficientForm &CoefficientForm::operator-=(const CoefficientForm &polynomial) {
    if (this->is_zero()) {
        this->coefficients.resize(polynomial.coefficients.size(), bls12_381::scalar::Scalar::zero());
        for (int i = 0; i < polynomial.coefficients.size(); ++i)
            this->coefficients[i] -= polynomial.coefficients[i];
    } else if (polynomial.is_zero()) {

    } else if (this->degree() >= polynomial.degree()) {
        for (int i = 0; i < this->coefficients.size(); ++i)
            this->coefficients[i] -= polynomial.coefficients[i];
    } else {
        this->coefficients.resize(polynomial.coefficients.size(), bls12_381::scalar::Scalar::zero());
        for (int i = 0; i < this->coefficients.size(); ++i)
            this->coefficients[i] -= polynomial.coefficients[i];
        this->trim_leading_zeros();
    }
    return *this;
}

CoefficientForm &CoefficientForm::operator*=(const CoefficientForm &polynomial) {
    if (this->is_zero() || polynomial.is_zero()) {
        *this = CoefficientForm::zero();
        return *this;
    }
    auto poly_coefficients = polynomial.coefficients;

    const domain::EvaluationDomain domain{this->coefficients.size() + polynomial.coefficients.size()};
    EvaluationForm self_evaluations{domain.fast_fourier(this->coefficients), domain};
    const EvaluationForm poly_evaluations{domain.fast_fourier(poly_coefficients), domain};

    self_evaluations *= poly_evaluations;
    *this = self_evaluations.interpolate();
    return *this;
}

CoefficientForm &CoefficientForm::operator+=(const bls12_381::scalar::Scalar &value) {
    if (this->is_zero()) {
        *this = CoefficientForm{{value}};
        return *this;
    }
    if (value.is_zero()) return *this;

    CoefficientForm res{*this};
    res.coefficients[0] += value;
    *this = res;
    return *this;
}

CoefficientForm &CoefficientForm::operator-=(const bls12_381::scalar::Scalar &value) {
    *this += (-value);
    return *this;
}

CoefficientForm &CoefficientForm::operator*=(const bls12_381::scalar::Scalar &value) {
    if (this->is_zero() || value.is_zero()) {
        *this = CoefficientForm::zero();
        return *this;
    }
    std::vector<bls12_381::scalar::Scalar> scaled_coeffs;
    scaled_coeffs.reserve(this->coefficients.size());
    for (const auto &coefficient: this->coefficients)
        scaled_coeffs.push_back(coefficient * value);
    *this = CoefficientForm{scaled_coeffs};
    return *this;
}

} // namespace kzg::polynomial