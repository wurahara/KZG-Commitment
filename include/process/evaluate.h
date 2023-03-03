#ifndef KZG_COMMITMENT_EVALUATE_H
#define KZG_COMMITMENT_EVALUATE_H

#include <vector>

#include "scalar/scalar.h"

#include "polynomial/coefficient.h"
#include "structure/commit_key.h"
#include "structure/commitment.h"
#include "structure/proofs.h"

namespace kzg::process::evaluate {

auto create_witness_single(
        const structure::CommitKey &commit_key,
        const polynomial::CoefficientForm &polynomial,
        const bls12_381::scalar::Scalar &point
) -> structure::Proof;

auto create_witness_multiple_polynomials(
        const structure::CommitKey &commit_key,
        const std::vector<polynomial::CoefficientForm> &polynomials,
        const bls12_381::scalar::Scalar &point,
        const bls12_381::scalar::Scalar &challenge_gamma
) -> structure::AggregatedProof;

} // namespace kzg::process

#endif //KZG_COMMITMENT_EVALUATE_H