#include "process/verify.h"

#include <cassert>

#include "group/g1_affine.h"
#include "group/g1_projective.h"
#include "group/g2_affine.h"
#include "group/g2_prepared.h"
#include "group/gt.h"
#include "pairing/pairing.h"

#include "util/field.h"

namespace kzg::process::verify {

using bls12_381::group::G1Affine;
using bls12_381::group::G1Projective;
using bls12_381::group::G2Affine;
using bls12_381::group::G2Prepared;
using bls12_381::group::Gt;
using bls12_381::pairing::multi_miller_loop;
using bls12_381::scalar::Scalar;
using challenge::TranscriptProtocol;
using structure::Commitment;
using structure::OpeningKey;
using structure::Proof;
using structure::BatchProof;
using structure::AggregatedProof;
using util::field::generate_vec_powers;

bool verify_single_polynomial(const OpeningKey &opening_key, const Commitment &commitment, const Proof &proof) {
    const auto inner_a = G1Affine{commitment.get_content() - proof.evaluation * opening_key.g};
    const auto inner_b = G2Affine{opening_key.h * proof.point - opening_key.h_beta};
    const auto inner_b_prepared = G2Prepared{inner_b};
    const auto pairing = multi_miller_loop({
                                                   {inner_a,                     opening_key.h_prepared},
                                                   {proof.witness.get_content(), inner_b_prepared}
                                           })
            .final_exponentiation();
    return pairing == Gt::identity();
}

bool verify_multiple_polynomials(const OpeningKey &opening_key,
                                 const std::vector<Commitment> &commitments,
                                 const AggregatedProof &proof, TranscriptProtocol &transcript) {
    const auto [flattened_poly_commitments, flattened_poly_evaluations] =
            verify_aggregation(commitments, proof.evaluations, transcript);
    return verify_single_polynomial(opening_key, flattened_poly_commitments, Proof{
            proof.point, flattened_poly_evaluations, proof.witness
    });
}

bool verify_multiple_points(const OpeningKey &opening_key,
                            const std::vector<Commitment> &commitments,
                            const BatchProof &proof, TranscriptProtocol &transcript) {
    assert(commitments.size() == proof.points.size());
    assert(commitments.size() == proof.evaluations.size());
    assert(commitments.size() == proof.witnesses.size());

    G1Projective total_c{};
    G1Projective total_w{};

    const auto challenge_u = transcript.challenge_scalar("challenge_u");
    const auto power_u = util::field::generate_vec_powers(challenge_u, commitments.size() - 1);

    Scalar multiplier_g{};
    G1Projective c;
    Commitment w;

    for (int i = 0; i < commitments.size(); ++i) {
        c = G1Projective{commitments[i].get_content()};
        w = proof.witnesses[i];
        c += w.get_content() * proof.points[i];
        multiplier_g += power_u[i] * proof.evaluations[i];
        total_c += c * power_u[i];
        total_w += w.get_content() * power_u[i];
    }
    total_c -= opening_key.g * multiplier_g;

    const G1Affine affine_total_w{-total_w};
    const G1Affine affine_total_c{total_c};

    const auto pairing = multi_miller_loop(
            {
                    {affine_total_w, opening_key.h_beta_prepared},
                    {affine_total_c, opening_key.h_prepared}
            }
    ).final_exponentiation();
    return pairing == Gt::identity();
}

std::tuple<Commitment, Scalar> verify_aggregation(const std::vector<Commitment> &commitments,
                                                  const std::vector<Scalar> &evaluations,
                                                  TranscriptProtocol &transcript) {
    const auto challenge_gamma = transcript.challenge_scalar("challenge_gamma");
    const auto powers_gamma = generate_vec_powers(challenge_gamma, commitments.size() - 1);

    G1Projective flattened_poly_commitments{};
    Scalar flattened_poly_evaluations{};

    for (int i = 0; i < commitments.size(); ++i)
        flattened_poly_commitments += commitments[i].get_content() * powers_gamma[i];
    for (int i = 0; i < evaluations.size(); ++i) flattened_poly_evaluations += evaluations[i] * powers_gamma[i];

    return {Commitment{flattened_poly_commitments}, flattened_poly_evaluations};
}

} // namespace kzg::process::verify