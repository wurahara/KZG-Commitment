#include <gtest/gtest.h>

#include "impl/os_rng.h"
#include "scalar/scalar.h"
#include "polynomial/evaluation.h"
#include "domain/domain.h"

using bls12_381::scalar::Scalar;
using kzg::polynomial::EvaluationForm;
using kzg::domain::EvaluationDomain;

TEST(Evaluation, Serialization) {
    std::vector<Scalar> evals;
    evals.reserve(100);

    rng::impl::OsRng rng;
    for (int i = 0; i < 100; ++i)
        evals.push_back(Scalar::random(rng));

    const EvaluationDomain domain{1000};
    const EvaluationForm poly{evals, domain};
    const auto poly_bytes = poly.to_var_bytes();
    const EvaluationForm poly_decoded = EvaluationForm::from_slice(poly_bytes).value();
    EXPECT_EQ(poly, poly_decoded);
}