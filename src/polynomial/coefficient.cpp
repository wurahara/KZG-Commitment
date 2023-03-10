#include "polynomial/coefficient.h"

#include <cassert>

#include "utils/field.h"

#include "domain/domain.h"
#include "polynomial/evaluation.h"

namespace kzg::polynomial {

using bls12_381::scalar::Scalar;
using rng::core::RngCore;

using domain::EvaluationDomain;
using util::field::generate_vec_powers;

CoefficientForm::CoefficientForm() : coefficients{} {}

CoefficientForm::CoefficientForm(const CoefficientForm &rhs) = default;

CoefficientForm::CoefficientForm(CoefficientForm &&rhs) noexcept = default;

CoefficientForm::CoefficientForm(const std::vector<Scalar> &coefficients) : coefficients{coefficients} {
    this->trim_leading_zeros();
    assert(this->coefficients.empty() || !this->coefficients.back().is_zero());
}

CoefficientForm::CoefficientForm(std::vector<Scalar> &&coefficients) : coefficients{std::move(coefficients)} {
    this->trim_leading_zeros();
    assert(this->coefficients.empty() || !this->coefficients.back().is_zero());
}

CoefficientForm CoefficientForm::zero() {
    return CoefficientForm{{}};
}

CoefficientForm CoefficientForm::random(size_t degree, RngCore &rng) {
    std::vector<Scalar> rand_coeffs;
    rand_coeffs.reserve(degree + 1);
    for (int i = 0; i <= degree; ++i)
        rand_coeffs.push_back(Scalar::random(rng));
    return CoefficientForm{rand_coeffs};
}

void CoefficientForm::trim_leading_zeros() {
    while (!this->coefficients.empty() && this->coefficients.back().is_zero())
        this->coefficients.pop_back();
}

bool CoefficientForm::is_zero() const {
    bool flag = true;
    for (const Scalar &coeff: this->coefficients)
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

auto CoefficientForm::size() const -> size_t {
    return this->coefficients.size();
}

Scalar CoefficientForm::evaluate(const Scalar &point) const {
    if (this->is_zero())
        return Scalar::zero();
    auto monomials = generate_vec_powers(point, this->degree());
    assert(monomials.size() == this->coefficients.size());

    Scalar res = Scalar::zero();
    for (int i = 0; i < this->coefficients.size(); ++i)
        res += (this->coefficients[i] * monomials[i]);
    return res;
}

CoefficientForm CoefficientForm::ruffini(const Scalar &point) const {
    std::vector<Scalar> quotient;
    quotient.reserve(this->degree());
    Scalar k = Scalar::zero();
    for (auto iter = this->coefficients.rbegin(); // NOLINT(modernize-loop-convert)
         iter != this->coefficients.rend(); iter++) {
        Scalar t = *iter + k;
        quotient.push_back(t);
        k = point * t;
    }
    if (!quotient.empty()) {
        quotient.pop_back();
        std::reverse(quotient.begin(), quotient.end());
    }
    return CoefficientForm{quotient};
}

std::vector<Scalar> CoefficientForm::get_coefficients() const {
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
        this->coefficients.resize(polynomial.coefficients.size(), Scalar::zero());
        for (int i = 0; i < this->coefficients.size(); ++i)
            this->coefficients[i] += polynomial.coefficients[i];
        this->trim_leading_zeros();
    }
    return *this;
}

CoefficientForm &CoefficientForm::operator-=(const CoefficientForm &polynomial) {
    if (this->is_zero()) {
        this->coefficients.resize(polynomial.coefficients.size(), Scalar::zero());
        for (int i = 0; i < polynomial.coefficients.size(); ++i)
            this->coefficients[i] -= polynomial.coefficients[i];
    } else if (polynomial.is_zero()) {

    } else if (this->degree() >= polynomial.degree()) {
        for (int i = 0; i < this->coefficients.size(); ++i)
            this->coefficients[i] -= polynomial.coefficients[i];
    } else {
        this->coefficients.resize(polynomial.coefficients.size(), Scalar::zero());
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

    const EvaluationDomain domain{this->coefficients.size() + polynomial.coefficients.size()};
    EvaluationForm self_evaluations{domain.fast_fourier(this->coefficients), domain};
    const EvaluationForm poly_evaluations{domain.fast_fourier(poly_coefficients), domain};

    self_evaluations *= poly_evaluations;
    *this = self_evaluations.interpolate();
    return *this;
}

CoefficientForm &CoefficientForm::operator+=(const Scalar &value) {
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

CoefficientForm &CoefficientForm::operator-=(const Scalar &value) {
    *this += (-value);
    return *this;
}

CoefficientForm &CoefficientForm::operator*=(const Scalar &value) {
    if (this->is_zero() || value.is_zero()) {
        *this = CoefficientForm::zero();
        return *this;
    }
    std::vector<Scalar> scaled_coeffs;
    scaled_coeffs.reserve(this->coefficients.size());
    for (const auto &coefficient: this->coefficients)
        scaled_coeffs.push_back(coefficient * value);
    *this = CoefficientForm{scaled_coeffs};
    return *this;
}

bls12_381::scalar::Scalar &CoefficientForm::operator[](size_t index) {
    assert(index < this->coefficients.size());
    return this->coefficients[index];
}

bls12_381::scalar::Scalar CoefficientForm::operator[](size_t index) const {
    assert(index < this->coefficients.size());
    return this->coefficients[index];
}

std::optional<CoefficientForm> CoefficientForm::from_slice(const std::vector<uint8_t> &bytes) {
    const size_t coeff_count = bytes.size() / Scalar::BYTE_SIZE;
    std::vector<Scalar> coeffs;
    coeffs.reserve(coeff_count);

    for (int i = 0; i < bytes.size(); i += Scalar::BYTE_SIZE) {
        std::array<uint8_t, Scalar::BYTE_SIZE> coeff_byte{};
        std::copy(bytes.begin() + i, bytes.begin() + i + Scalar::BYTE_SIZE, coeff_byte.begin());
        const auto scalar_opt = Scalar::from_bytes(coeff_byte);
        if (!scalar_opt.has_value()) return std::nullopt;
        coeffs.push_back(scalar_opt.value());
    }

    return CoefficientForm{coeffs};
}

std::vector<uint8_t> CoefficientForm::to_var_bytes() const {
    std::vector<uint8_t> res{};
    res.reserve(this->coefficients.size() * Scalar::BYTE_SIZE);
    for (const Scalar &coeff: this->coefficients) {
        const std::array<uint8_t, Scalar::BYTE_SIZE> coeff_bytes = coeff.to_bytes();
        res.insert(res.end(), coeff_bytes.begin(), coeff_bytes.end());
    }
    return res;
}

} // namespace kzg::polynomial