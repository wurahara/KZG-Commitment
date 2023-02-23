#include "utils/group.h"

#include "group/g1_affine.h"
#include "group/g2_affine.h"

#include "utils/field.h"

namespace kzg::util::group {

using bls12_381::group::G1Affine;
using bls12_381::group::G1Projective;
using bls12_381::group::G2Affine;
using bls12_381::group::G2Projective;
using bls12_381::scalar::Scalar;
using field::generate_random_scalar;

G1Projective generate_random_g1_point() {
    return G1Affine::generator() * generate_random_scalar();
}

G2Projective generate_random_g2_point() {
    return G2Affine::generator() * generate_random_scalar();
}

std::vector<G1Projective> slow_multi_scalar_mul_single_base(const std::vector<Scalar> &scalars, const G1Projective &base) {
    std::vector<G1Projective> res;
    res.reserve(scalars.size());

    for (const auto &scalar: scalars)
        res.push_back(base * scalar);

    return res;
}

} // namespace kzg::util::group