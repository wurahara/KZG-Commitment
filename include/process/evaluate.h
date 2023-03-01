#ifndef KZG_COMMITMENT_EVALUATE_H
#define KZG_COMMITMENT_EVALUATE_H

#include <vector>

#include "scalar/scalar.h"

#include "challenge/transcript.h"
#include "polynomial/coefficient.h"
#include "structure/commit_key.h"
#include "structure/commitment.h"
#include "structure/proofs.h"

namespace kzg::process::evaluate {

structure::Proof create_witness_single(
        const structure::CommitKey &commit_key,
        const polynomial::CoefficientForm &polynomial,
        const bls12_381::scalar::Scalar &point
);

structure::AggregatedProof create_witness_multiple_polynomials(
        const structure::CommitKey &commit_key,
        const std::vector<polynomial::CoefficientForm> &polynomials,
        const bls12_381::scalar::Scalar &point,
        challenge::BaseTranscript &transcript
);

} // namespace kzg::process

#endif //KZG_COMMITMENT_EVALUATE_H