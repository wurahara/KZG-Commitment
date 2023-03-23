#ifndef KZG_COMMITMENT_EVALUATE_H
#define KZG_COMMITMENT_EVALUATE_H

#include <vector>

#include "scalar/scalar.h"

#include "polynomial/coefficient.h"
#include "structure/commit_key.h"
#include "structure/commitment.h"
#include "structure/proofs.h"

namespace kzg::process::evaluate {

/**
 * @brief Computes a single witness for a polynomial committed at a given point.
 * @param commit_key the committing key.
 * @param polynomial the committed polynomial in coefficient form.
 * @param point the point for the polynomial to be evaluated.
 * @return the witness for evaluation.
 */
auto create_witness_single(
        const structure::CommitKey &commit_key,
        const polynomial::CoefficientForm &polynomial,
        const bls12_381::scalar::Scalar &point
) -> structure::Proof;

/**
 * @brief Computes a single witness for multiple polynomials committed at a same point by taking a random linear
 *          combination of the individual witnesses.
 * @param commit_key the committing key.
 * @param polynomials the committed polynomials in coefficient form.
 * @param point the point for the polynomials to be evaluated.
 * @param challenge_gamma the parameter for the linear combination.
 * @return the witness for evaluation.
 */
auto create_witness_multiple_polynomials(
        const structure::CommitKey &commit_key,
        const std::vector<polynomial::CoefficientForm> &polynomials,
        const bls12_381::scalar::Scalar &point,
        const bls12_381::scalar::Scalar &challenge_gamma
) -> structure::AggregatedProof;

} // namespace kzg::process

#endif //KZG_COMMITMENT_EVALUATE_H