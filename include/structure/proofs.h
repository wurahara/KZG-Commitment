#ifndef KZG_COMMITMENT_PROOFS_H
#define KZG_COMMITMENT_PROOFS_H

#include <vector>

#include "scalar/scalar.h"

#include "structure/commitment.h"

namespace kzg::structure {

struct Proof {
    bls12_381::scalar::Scalar point;
    bls12_381::scalar::Scalar evaluation;
    structure::Commitment witness;
};

struct AggregatedProof {
    bls12_381::scalar::Scalar point;
    std::vector<bls12_381::scalar::Scalar> evaluations;
    structure::Commitment witness;
};

struct BatchProof {
    std::vector<bls12_381::scalar::Scalar> points;
    std::vector<bls12_381::scalar::Scalar> evaluations;
    std::vector<structure::Commitment> witnesses;
};

} // namespace kzg::structure

#endif //KZG_COMMITMENT_PROOFS_H