#include <gtest/gtest.h>

#include "scalar/scalar.h"
#include "polynomial/coefficient.h"

TEST(Coefficient, Evaluation) {
    kzg::polynomial::CoefficientForm poly{{
                                                  bls12_381::scalar::Scalar{5},
                                                  -bls12_381::scalar::Scalar{7},
                                                  bls12_381::scalar::Scalar::one(),
                                          }};
    bls12_381::scalar::Scalar point = bls12_381::scalar::Scalar{3};
    bls12_381::scalar::Scalar evaluation = poly.evaluate(point);
    EXPECT_EQ(evaluation, -bls12_381::scalar::Scalar{7});
}

TEST(Coefficient, Ruffini) {
    kzg::polynomial::CoefficientForm quadratic{{
                                                       bls12_381::scalar::Scalar{4},
                                                       -bls12_381::scalar::Scalar{4},
                                                       bls12_381::scalar::Scalar::one(),
                                               }};
    kzg::polynomial::CoefficientForm quotient = quadratic.ruffini(bls12_381::scalar::Scalar{2});
    kzg::polynomial::CoefficientForm expected{{
                                                      -bls12_381::scalar::Scalar{2},
                                                      bls12_381::scalar::Scalar::one()
                                              }};
    EXPECT_EQ(quotient, expected);
}

TEST(Coefficient, RuffiniZero) {
    // case 1: zero polynomial
    kzg::polynomial::CoefficientForm zero_poly = kzg::polynomial::CoefficientForm::zero();
    kzg::polynomial::CoefficientForm quotient = zero_poly.ruffini(bls12_381::scalar::Scalar{100});
    EXPECT_EQ(quotient, kzg::polynomial::CoefficientForm::zero());

    // case 2: zero constant term
    kzg::polynomial::CoefficientForm polynomial{{
                                                        bls12_381::scalar::Scalar::zero(),
                                                        bls12_381::scalar::Scalar::one(),
                                                        bls12_381::scalar::Scalar::one(),
                                                }};
    kzg::polynomial::CoefficientForm quotient2 = polynomial.ruffini(bls12_381::scalar::Scalar::zero());
    kzg::polynomial::CoefficientForm expected{{
                                                      bls12_381::scalar::Scalar::one(),
                                                      bls12_381::scalar::Scalar::one(),
                                              }};
    EXPECT_EQ(quotient2, expected);
}