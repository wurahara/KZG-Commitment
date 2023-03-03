#include "process/evaluate.h"

#include <cassert>
#include <vector>

#include "process/commit.h"
#include "utils/field.h"

namespace kzg::process::evaluate {

using bls12_381::scalar::Scalar;
using polynomial::CoefficientForm;
using structure::CommitKey;
using structure::Proof;
using structure::AggregatedProof;

auto create_witness_single(const CommitKey &commit_key, const CoefficientForm &polynomial, const Scalar &point)
-> Proof {
    const auto evaluation = polynomial.evaluate(point);
    const auto diff = polynomial - evaluation;
    const auto quotient = diff.ruffini(point);
    auto witness = commit::commit(commit_key, quotient);
    return Proof{point, evaluation, witness};
}

auto create_witness_multiple_polynomials(
        const CommitKey &commit_key,
        const std::vector<CoefficientForm> &polynomials,
        const Scalar &point,
        const Scalar &challenge_gamma
) -> AggregatedProof {
    const auto gamma_powers = util::field::generate_vec_powers(challenge_gamma, polynomials.size() - 1);

    assert(gamma_powers.size() == polynomials.size());

    std::vector<Scalar> evaluations;
    evaluations.reserve(polynomials.size());
    for (const auto &polynomial: polynomials)
        evaluations.push_back(polynomial.evaluate(point));

    CoefficientForm psi_poly_numerator{};
    for (int i = 0; i < polynomials.size(); ++i)
        psi_poly_numerator += polynomials[i] * gamma_powers[i];

    const auto quotient = psi_poly_numerator.ruffini(point);
    const auto witness = commit::commit(commit_key, quotient);

    return AggregatedProof{point, evaluations, witness};
}

} // namespace kzg::process::evaluate