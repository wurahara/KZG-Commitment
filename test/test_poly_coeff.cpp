#include <gtest/gtest.h>

#include "impl/os_rng.h"
#include "scalar/scalar.h"
#include "polynomial/coefficient.h"

using bls12_381::scalar::Scalar;
using kzg::polynomial::CoefficientForm;

TEST(Coefficient, Evaluation) {
    const CoefficientForm poly{{Scalar{5}, -Scalar{7}, Scalar::one()}};
    const Scalar point = Scalar{3};
    const Scalar evaluation = poly.evaluate(point);
    EXPECT_EQ(evaluation, -Scalar{7});
}

TEST(Coefficient, Ruffini) {
    const CoefficientForm quadratic{{Scalar{4}, -Scalar{4}, Scalar::one()}};
    const CoefficientForm quotient = quadratic.ruffini(Scalar{2});
    const CoefficientForm expected{{-Scalar{2}, Scalar::one()}};
    EXPECT_EQ(quotient, expected);
}

TEST(Coefficient, RuffiniZero) {
    // case 1: zero polynomial
    const CoefficientForm zero_poly = CoefficientForm::zero();
    const CoefficientForm quotient = zero_poly.ruffini(Scalar{100});
    EXPECT_EQ(quotient, CoefficientForm::zero());

    // case 2: zero constant term
    const CoefficientForm polynomial{{Scalar::zero(), Scalar::one(), Scalar::one()}};
    const CoefficientForm quotient2 = polynomial.ruffini(Scalar::zero());
    const CoefficientForm expected{{Scalar::one(), Scalar::one()}};
    EXPECT_EQ(quotient2, expected);
}

TEST(Coefficient, Serialization) {
    std::vector<Scalar> coeffs;
    coeffs.reserve(1000);

    rng::impl::OsRng rng;
    for (int i = 0; i < 1000; ++i)
        coeffs.push_back(Scalar::random(rng));

    const CoefficientForm poly{coeffs};
    const auto poly_bytes = poly.to_var_bytes();
    const CoefficientForm poly_decoded = CoefficientForm::from_slice(poly_bytes).value();
    EXPECT_EQ(poly, poly_decoded);
}