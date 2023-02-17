#include <gtest/gtest.h>

#include <vector>

#include "domain/domain.h"
#include "domain/iterator.h"

using bls12_381::scalar::Scalar;
using kzg::domain::EvaluationDomain;

TEST(TestDomain, Fourier) {
    EvaluationDomain domain{4};
    std::vector<Scalar> coefficients = {Scalar{2}, Scalar{5}, Scalar{7}, Scalar{8}};
    auto evaluations = domain.fast_fourier(coefficients);
    auto rep_coeffs = domain.inverse_fast_fourier(evaluations);

    EXPECT_EQ(Scalar{2}, rep_coeffs[0]);
    EXPECT_EQ(Scalar{5}, rep_coeffs[1]);
    EXPECT_EQ(Scalar{7}, rep_coeffs[2]);
    EXPECT_EQ(Scalar{8}, rep_coeffs[3]);
}

TEST(TestDomain, ElementSize) {
    for (int i = 1; i < 10; ++i) {
        uint64_t size = 1 << i;
        EvaluationDomain domain{size};
        size_t domain_size = domain.size();
        EXPECT_EQ(domain_size, domain.iter().size());
    }
}

TEST(TestDomain, ElementContents) {
    for (int i = 1; i < 10; ++i) {
        uint64_t size = 1 << i;
        EvaluationDomain domain{size};
        size_t sentinel = 0;
        for (const auto &element: domain.iter()) {
            EXPECT_EQ(element, domain.group_generator().pow({sentinel, 0, 0, 0}));
            sentinel++;
        }
    }
}