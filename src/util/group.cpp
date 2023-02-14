#include "util/group.h"

#include "group/g1_affine.h"
#include "group/g2_affine.h"

#include "util/field.h"

namespace kzg::util::group {

bls12_381::group::G1Projective generate_random_g1_point() {
    return bls12_381::group::G1Affine::generator() * field::generate_random_scalar();
}

bls12_381::group::G2Projective generate_random_g2_point() {
    return bls12_381::group::G2Affine::generator() * field::generate_random_scalar();
}

std::vector<bls12_381::group::G1Projective>
slow_multi_scalar_mul_single_base(const std::vector<bls12_381::scalar::Scalar> &scalars,
                                  const bls12_381::group::G1Projective &base) {
    std::vector<bls12_381::group::G1Projective> res;
    res.reserve(scalars.size());

    for (const auto &scalar: scalars)
        res.push_back(base * scalar);

    return res;
}

} // namespace kzg::util::group