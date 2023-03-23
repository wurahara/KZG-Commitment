#ifndef KZG_COMMITMENT_GROUP_H
#define KZG_COMMITMENT_GROUP_H

#include <vector>

#include "core/rng.h"
#include "group/g1_projective.h"
#include "group/g2_projective.h"

namespace kzg::util::group {

/**
 * @brief Generates a random point on <tt>G1Projective</tt> using an RNG seed.
 * @param rng the random number generator
 * @return a random <tt>G1Projective</tt>
 */
auto random_g1_point(rng::core::RngCore &rng) -> bls12_381::group::G1Projective;

/**
 * @brief Generates a random point on <tt>G2Projective</tt> using an RNG seed.
 * @param rng the random number generator
 * @return a random <tt>G2Projective</tt>
 */
auto random_g2_point(rng::core::RngCore &rng) -> bls12_381::group::G2Projective;

std::vector<bls12_381::group::G1Projective> slow_multi_scalar_mul_single_base(
        const std::vector<bls12_381::scalar::Scalar> &scalars,
        const bls12_381::group::G1Projective &base
);

} // namespace kzg::util::group

#endif //KZG_COMMITMENT_GROUP_H