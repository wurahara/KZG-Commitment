#ifndef KZG_COMMITMENT_VERIFY_H
#define KZG_COMMITMENT_VERIFY_H

#include <tuple>
#include <vector>

#include "scalar/scalar.h"

#include "structure/commitment.h"
#include "structure/opening_key.h"
#include "structure/proofs.h"

namespace kzg::process::verify {

/**
 * @brief Verifies the opening value of a polynomial evaluated at a specific point.
 * @param opening_key the opening key.
 * @param commitment the commitment of the polynomial.
 * @param proof the proof, contains the point, the evaluation and the witness.
 * @return the verification result.
 */
auto verify_single_polynomial(
        const structure::OpeningKey &opening_key,
        const structure::Commitment &commitment,
        const structure::Proof &proof
) -> bool;

/**
 * @brief Verifies the opening value of a batch of polynomials evaluated at a same point.
 * @param opening_key the opening key.
 * @param commitments the commitments of the polynomials.
 * @param proof the aggregated proof, contains the point, the evaluations and the witness.
 * @param gamma_challenge the challenge for linear combination.
 * @return the verification result.
 */
auto verify_multiple_polynomials(
        const structure::OpeningKey &opening_key,
        const std::vector<structure::Commitment> &commitments,
        const structure::AggregatedProof &proof,
        const bls12_381::scalar::Scalar &gamma_challenge
) -> bool;

/**
 * @brief Verifies a batch of polynomials evaluated at different points.
 * @param opening_key the opening key.
 * @param commitments the commitments of the polynomials.
 * @param proof the batched proof, contains points, evaluations and corresponding witnesses.
 * @param u_challenge the challenge for linear combination.
 * @return the verification result.
 */
auto verify_multiple_points(
        const structure::OpeningKey &opening_key,
        const std::vector<structure::Commitment> &commitments,
        const structure::BatchProof &proof,
        const bls12_381::scalar::Scalar &u_challenge
) -> bool;

auto verify_aggregation(
        const std::vector<structure::Commitment> &commitments,
        const std::vector<bls12_381::scalar::Scalar> &evaluations,
        const bls12_381::scalar::Scalar &gamma_challenge
) -> std::tuple<structure::Commitment, bls12_381::scalar::Scalar>;

} // namespace kzg::process::verify

#endif //KZG_COMMITMENT_VERIFY_H