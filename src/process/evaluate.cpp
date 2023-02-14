#include "process/evaluate.h"

#include <cassert>
#include <vector>

#include "process/commit.h"
#include "util/field.h"

namespace kzg::process {

structure::Proof create_witness_single(
        const structure::CommitKey &commit_key,
        const polynomial::CoefficientForm &polynomial,
        const bls12_381::scalar::Scalar &point) {
    const auto evaluation = polynomial.evaluate(point);
    const auto diff = polynomial - evaluation;
    const auto quotient = diff.ruffini(point);
    auto witness = process::commit(commit_key, quotient);
    return structure::Proof{point, evaluation, witness};
}

structure::AggregatedProof create_witness_multiple_polynomials(
        const structure::CommitKey &commit_key,
        const std::vector<polynomial::CoefficientForm> &polynomials,
        const bls12_381::scalar::Scalar &point,
        challenge::TranscriptProtocol &transcript) {
    const auto challenge_gamma = transcript.challenge_scalar("challenge_gamma");
    const auto gamma_powers = util::field::generate_vec_powers(challenge_gamma, polynomials.size() - 1);

    assert(gamma_powers.size() == polynomials.size());

    std::vector<bls12_381::scalar::Scalar> evaluations;
    evaluations.reserve(polynomials.size());
    for (const auto &polynomial: polynomials)
        evaluations.push_back(polynomial.evaluate(point));

    polynomial::CoefficientForm psi_poly_numerator{};
    for (int i = 0; i < polynomials.size(); ++i)
        psi_poly_numerator += polynomials[i] * gamma_powers[i];

    const auto quotient = psi_poly_numerator.ruffini(point);
    const auto witness = process::commit(commit_key, quotient);

    return structure::AggregatedProof{point, evaluations, witness};
}


} // namespace kzg::process