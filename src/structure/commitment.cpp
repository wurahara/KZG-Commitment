#include "structure/commitment.h"

namespace kzg::structure {

Commitment::Commitment() : content{bls12_381::group::G1Affine::identity()} {}

Commitment::Commitment(const bls12_381::group::G1Affine &element) : content{element} {}

Commitment::Commitment(bls12_381::group::G1Affine &&element) noexcept: content{element} {}

Commitment::Commitment(const bls12_381::group::G1Projective &element)
        : content{bls12_381::group::G1Projective{element}} {}

Commitment Commitment::identity() noexcept {
    return Commitment{};
}

bls12_381::group::G1Affine Commitment::get_content() const {
    return this->content;
}

} // namespace kzg::structure