#include "structure/reference_string.h"

#include <cassert>
#include <utility>

#include "group/g1_projective.h"
#include "group/g2_affine.h"

#include "utils/field.h"
#include "utils/group.h"

namespace kzg::structure {

using bls12_381::group::G1Affine;
using bls12_381::group::G1Projective;
using bls12_381::group::G2Affine;
using util::field::generate_random_scalar;
using util::field::generate_vec_powers;
using util::group::generate_random_g1_point;
using util::group::generate_random_g2_point;
using util::group::slow_multi_scalar_mul_single_base;

ReferenceString::ReferenceString(CommitKey commit_key, OpeningKey opening_key)
        : commit_key{std::move(commit_key)}, opening_key{std::move(opening_key)} {}

ReferenceString ReferenceString::setup(size_t max_degree) {
    assert(max_degree >= 1);

    max_degree = max_degree + ReferenceString::EXTRA_BLINDING_DEGREE;
    const auto x = generate_random_scalar();
    const auto powers_x = generate_vec_powers(x, max_degree);
    const auto g = generate_random_g1_point();
    const auto powers_g = slow_multi_scalar_mul_single_base(powers_x, g);

    assert(powers_g.size() == max_degree + 1);

    const auto normalized_g = G1Projective::batch_normalize(powers_g);
    const auto h = G2Affine{generate_random_g2_point()};
    const auto x_2 = G2Affine{h * x};

    return ReferenceString{CommitKey{normalized_g}, OpeningKey{G1Affine{g}, h, x_2}};
}

std::tuple<CommitKey, OpeningKey> ReferenceString::trim(size_t truncated_degree) {
    auto truncated_prover_key = this->commit_key.truncate(truncated_degree + ReferenceString::EXTRA_BLINDING_DEGREE);
    return {truncated_prover_key, this->opening_key};
}

} // namespace kzg::structure