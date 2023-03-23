#ifndef KZG_COMMITMENT_PROOFS_H
#define KZG_COMMITMENT_PROOFS_H

#include <vector>

#include "scalar/scalar.h"

#include "structure/commitment.h"

namespace kzg::structure {

/**
 * @brief Proof about a polynomial evaluated at a given <tt>point</tt> gives a value <tt>evaluation</tt>.
 */
struct Proof {
    /// the point where the polynomial is evaluated
    bls12_381::scalar::Scalar point;
    /// the evaluation result of the polynomial at the <tt>point</tt>
    bls12_381::scalar::Scalar evaluation;
    /// the witness for the evaluation
    structure::Commitment witness;
};

/**
 * @brief Proof about multiple polynomials evaluated at a same <tt>point</tt> gives a batch of <tt>evaluations</tt>.
 */
struct AggregatedProof {
    /// the point where the polynomials are evaluated
    bls12_381::scalar::Scalar point;
    /// the evaluation results of the polynomials at the <tt>point</tt>
    std::vector<bls12_381::scalar::Scalar> evaluations;
    /// the witness for the evaluations
    structure::Commitment witness;
};

/**
 * Proof about multiple polynomials evaluated at multiple <tt>points</tt> gives a batch of <tt>evaluations</tt>.
 */
struct BatchProof {
    /// the points where the polynomials are evaluated
    std::vector<bls12_381::scalar::Scalar> points;
    /// the evaluation results of the polynomials at each point in <tt>points</tt>
    std::vector<bls12_381::scalar::Scalar> evaluations;
    /// the witnesses for the evaluations
    std::vector<structure::Commitment> witnesses;
};

} // namespace kzg::structure

#endif //KZG_COMMITMENT_PROOFS_H