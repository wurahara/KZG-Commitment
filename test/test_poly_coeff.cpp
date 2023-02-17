#include <gtest/gtest.h>

#include "scalar/scalar.h"
#include "polynomial/coefficient.h"

using bls12_381::scalar::Scalar;
using kzg::polynomial::CoefficientForm;

TEST(Coefficient, Evaluation) {
    CoefficientForm poly{{Scalar{5}, -Scalar{7}, Scalar::one()}};
    Scalar point = Scalar{3};
    Scalar evaluation = poly.evaluate(point);
    EXPECT_EQ(evaluation, -Scalar{7});
}

TEST(Coefficient, Ruffini) {
    CoefficientForm quadratic{{Scalar{4}, -Scalar{4}, Scalar::one()}};
    CoefficientForm quotient = quadratic.ruffini(Scalar{2});
    CoefficientForm expected{{-Scalar{2}, Scalar::one()}};
    EXPECT_EQ(quotient, expected);
}

TEST(Coefficient, RuffiniZero) {
    // case 1: zero polynomial
    CoefficientForm zero_poly = CoefficientForm::zero();
    CoefficientForm quotient = zero_poly.ruffini(Scalar{100});
    EXPECT_EQ(quotient, CoefficientForm::zero());

    // case 2: zero constant term
    CoefficientForm polynomial{{Scalar::zero(), Scalar::one(), Scalar::one()}};
    CoefficientForm quotient2 = polynomial.ruffini(Scalar::zero());
    CoefficientForm expected{{Scalar::one(), Scalar::one()}};
    EXPECT_EQ(quotient2, expected);
}