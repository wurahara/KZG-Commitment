#ifndef KZG_COMMITMENT_VERIFY_H
#define KZG_COMMITMENT_VERIFY_H

#include <tuple>
#include <vector>

#include "scalar/scalar.h"

#include "challenge/transcript.h"
#include "structure/commitment.h"
#include "structure/opening_key.h"
#include "structure/proofs.h"

namespace kzg::process::verify {

bool verify_single_polynomial(
        const structure::OpeningKey &opening_key,
        const structure::Commitment &commitment,
        const structure::Proof &proof
);

bool verify_multiple_polynomials(
        const structure::OpeningKey &opening_key,
        const std::vector<structure::Commitment> &commitments,
        const structure::AggregatedProof &proof,
        challenge::TranscriptProtocol &transcript
);

bool verify_multiple_points(
        const structure::OpeningKey &opening_key,
        const std::vector<structure::Commitment> &commitments,
        const structure::BatchProof &proof,
        challenge::TranscriptProtocol &transcript
);

std::tuple<structure::Commitment, bls12_381::scalar::Scalar> verify_aggregation(
        const std::vector<structure::Commitment> &commitments,
        const std::vector<bls12_381::scalar::Scalar> &evaluations,
        challenge::TranscriptProtocol &transcript
);

} // namespace kzg::process::verify

#endif //KZG_COMMITMENT_VERIFY_H