#ifndef KZG_COMMITMENT_GROUP_H
#define KZG_COMMITMENT_GROUP_H

#include <vector>

#include "group/g1_projective.h"
#include "group/g2_projective.h"

namespace kzg::util::group {

bls12_381::group::G1Projective generate_random_g1_point();
bls12_381::group::G2Projective generate_random_g2_point();
std::vector<bls12_381::group::G1Projective> slow_multi_scalar_mul_single_base(
        const std::vector<bls12_381::scalar::Scalar> &scalars,
        const bls12_381::group::G1Projective &base
);

} // namespace kzg::util::group

#endif //KZG_COMMITMENT_GROUP_H