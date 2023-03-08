#include <gtest/gtest.h>

#include <vector>

#include "domain/domain.h"
#include "domain/iterator.h"

using bls12_381::scalar::Scalar;
using kzg::domain::EvaluationDomain;

TEST(Domain, Fourier) {
    const EvaluationDomain domain{4};
    std::vector<Scalar> coefficients = {Scalar{2}, Scalar{5}, Scalar{7}, Scalar{8}};
    auto evaluations = domain.fast_fourier(coefficients);
    auto rep_coeffs = domain.inverse_fast_fourier(evaluations);

    EXPECT_EQ(Scalar{2}, rep_coeffs[0]);
    EXPECT_EQ(Scalar{5}, rep_coeffs[1]);
    EXPECT_EQ(Scalar{7}, rep_coeffs[2]);
    EXPECT_EQ(Scalar{8}, rep_coeffs[3]);
}

TEST(Domain, ElementSize) {
    for (int i = 1; i < 10; ++i) {
        const uint64_t size = 1 << i;
        const EvaluationDomain domain{size};
        const size_t domain_size = domain.size();
        EXPECT_EQ(domain_size, domain.iter().size());
    }
}

TEST(Domain, ElementContents) {
    for (int i = 1; i < 10; ++i) {
        const uint64_t size = 1 << i;
        const EvaluationDomain domain{size};
        size_t sentinel = 0;
        for (const auto &element: domain.iter()) {
            EXPECT_EQ(element, domain.group_generator().pow({sentinel, 0, 0, 0}));
            sentinel++;
        }
    }
}

TEST(Domain, Serialization) {
    for (int i = 1; i < 100; ++i) {
        const uint64_t size = 1 << i;
        const EvaluationDomain domain{size};
        const auto domain_bytes = domain.to_bytes();
        const EvaluationDomain domain_recovered = EvaluationDomain::from_bytes(domain_bytes).value();
        EXPECT_EQ(domain, domain_recovered);
    }
}