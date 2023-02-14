#ifndef KZG_COMMITMENT_COEFFICIENT_H
#define KZG_COMMITMENT_COEFFICIENT_H

#include <vector>

#include "scalar/scalar.h"

namespace kzg::polynomial {

class CoefficientForm {
private:
    std::vector<bls12_381::scalar::Scalar> coefficients;

public:
    CoefficientForm();
    CoefficientForm(const CoefficientForm &rhs);
    CoefficientForm(CoefficientForm &&rhs) noexcept;

    explicit CoefficientForm(const std::vector<bls12_381::scalar::Scalar> &coefficients);
    explicit CoefficientForm(std::vector<bls12_381::scalar::Scalar> &&coefficients);

    static CoefficientForm zero();
    static CoefficientForm random(size_t degree);

    void trim_leading_zeros();

    [[nodiscard]] bool is_zero() const;
    [[nodiscard]] size_t degree() const;

    [[nodiscard]] CoefficientForm ruffini(const bls12_381::scalar::Scalar &point) const;
    [[nodiscard]] bls12_381::scalar::Scalar evaluate(const bls12_381::scalar::Scalar &point) const;
    [[nodiscard]] std::vector<bls12_381::scalar::Scalar> get_coefficients() const;

public:
    CoefficientForm &operator=(const CoefficientForm &rhs);
    CoefficientForm &operator=(CoefficientForm &&rhs) noexcept;

    CoefficientForm operator-() const;

    CoefficientForm &operator+=(const CoefficientForm &polynomial);
    CoefficientForm &operator-=(const CoefficientForm &polynomial);
    CoefficientForm &operator*=(const CoefficientForm &polynomial);

    CoefficientForm &operator+=(const bls12_381::scalar::Scalar &value);
    CoefficientForm &operator-=(const bls12_381::scalar::Scalar &value);
    CoefficientForm &operator*=(const bls12_381::scalar::Scalar &value);

public:
    friend inline CoefficientForm operator+(const CoefficientForm &a, const CoefficientForm &b) { return CoefficientForm(a) += b; }
    friend inline CoefficientForm operator-(const CoefficientForm &a, const CoefficientForm &b) { return CoefficientForm(a) -= b; }
    friend inline CoefficientForm operator*(const CoefficientForm &a, const CoefficientForm &b) { return CoefficientForm(a) *= b; }

    friend inline CoefficientForm operator+(const CoefficientForm &a, const bls12_381::scalar::Scalar &b) { return CoefficientForm(a) += b; }
    friend inline CoefficientForm operator-(const CoefficientForm &a, const bls12_381::scalar::Scalar &b) { return CoefficientForm(a) -= b; }
    friend inline CoefficientForm operator*(const CoefficientForm &a, const bls12_381::scalar::Scalar &b) { return CoefficientForm(a) *= b; }

    friend inline bool operator==(const CoefficientForm &a, const CoefficientForm &b) { return a.coefficients == b.coefficients; }
    friend inline bool operator!=(const CoefficientForm &a, const CoefficientForm &b) { return a.coefficients != b.coefficients; }
};

} // namespace kzg::polynomial

#endif //KZG_COMMITMENT_COEFFICIENT_H