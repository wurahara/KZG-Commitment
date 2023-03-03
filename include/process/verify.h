#ifndef KZG_COMMITMENT_VERIFY_H
#define KZG_COMMITMENT_VERIFY_H

#include <tuple>
#include <vector>

#include "scalar/scalar.h"

#include "structure/commitment.h"
#include "structure/opening_key.h"
#include "structure/proofs.h"

namespace kzg::process::verify {

auto verify_single_polynomial(
        const structure::OpeningKey &opening_key,
        const structure::Commitment &commitment,
        const structure::Proof &proof
) -> bool;

auto verify_multiple_polynomials(
        const structure::OpeningKey &opening_key,
        const std::vector<structure::Commitment> &commitments,
        const structure::AggregatedProof &proof,
        const bls12_381::scalar::Scalar &challenge_gamma
) -> bool;

auto verify_multiple_points(
        const structure::OpeningKey &opening_key,
        const std::vector<structure::Commitment> &commitments,
        const structure::BatchProof &proof,
        const bls12_381::scalar::Scalar &challenge_u
) -> bool;

auto verify_aggregation(
        const std::vector<structure::Commitment> &commitments,
        const std::vector<bls12_381::scalar::Scalar> &evaluations,
        const bls12_381::scalar::Scalar &challenge_gamma
) -> std::tuple<structure::Commitment, bls12_381::scalar::Scalar>;

} // namespace kzg::process::verify

#endif //KZG_COMMITMENT_VERIFY_H