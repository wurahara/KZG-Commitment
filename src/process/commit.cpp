#include "process/commit.h"

#include "group/g1_projective.h"

namespace kzg::process::commit {

using bls12_381::group::G1Projective;
using polynomial::CoefficientForm;
using structure::Commitment;

structure::Commitment commit(const structure::CommitKey &commit_key, const CoefficientForm &polynomial) {
    commit_key.check_polynomial_degree(polynomial);

    const auto coefficients = polynomial.get_coefficients();
    const auto vec = commit_key.get_vec();

    G1Projective res{};
    for (int i = 0; i < coefficients.size(); ++i)
        res += vec[i] * coefficients[i];

    return Commitment{res};
}

} // namespace kzg::process::commit