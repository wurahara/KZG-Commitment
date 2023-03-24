#ifndef KZG_COMMITMENT_COEFFICIENT_H
#define KZG_COMMITMENT_COEFFICIENT_H

#include <vector>

#include "core/rng.h"
#include "scalar/scalar.h"

namespace kzg::polynomial {

/**
 * @brief Represents a polynomial in coefficient form.
 */
class CoefficientForm {
private:
    /// The coefficient of x ^ i is stored at index i.
    std::vector<bls12_381::scalar::Scalar> coefficients;

public:
    CoefficientForm();
    CoefficientForm(const CoefficientForm &rhs);
    CoefficientForm(CoefficientForm &&rhs) noexcept;

    explicit CoefficientForm(const std::vector<bls12_381::scalar::Scalar> &coefficients);
    explicit CoefficientForm(std::vector<bls12_381::scalar::Scalar> &&coefficients);

    static CoefficientForm zero();
    static CoefficientForm random(size_t degree, rng::core::RngCore &rng);

    void trim_leading_zeros();

    [[nodiscard]] auto is_zero() const -> bool;
    [[nodiscard]] auto degree() const -> size_t;

    /**
     * @brief Divides the polynomial by (X - point) using Ruffini's method.
     * @param point the point to divide by.
     * @return the quotient polynomial.
     */
    [[nodiscard]] auto ruffini(const bls12_381::scalar::Scalar &point) const -> CoefficientForm;
    [[nodiscard]] auto evaluate(const bls12_381::scalar::Scalar &point) const -> bls12_381::scalar::Scalar;
    [[nodiscard]] auto get_coefficients() const -> std::vector<bls12_381::scalar::Scalar>;

    static std::optional<CoefficientForm> from_slice(const std::vector<uint8_t> &bytes);
    [[nodiscard]] std::vector<uint8_t> to_var_bytes() const;

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

    bls12_381::scalar::Scalar &operator[](size_t index);
    bls12_381::scalar::Scalar operator[](size_t index) const;

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