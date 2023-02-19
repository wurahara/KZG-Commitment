#include "structure/commitment.h"

namespace kzg::structure {

using bls12_381::group::G1Affine;
using bls12_381::group::G1Projective;

Commitment::Commitment() : content{G1Affine::identity()} {}

Commitment::Commitment(const G1Affine &element) : content{element} {}

Commitment::Commitment(G1Affine &&element) noexcept: content{element} {}

Commitment::Commitment(const G1Projective &element) : content{G1Projective{element}} {}

Commitment Commitment::identity() noexcept {
    return Commitment{};
}

G1Affine Commitment::get_content() const {
    return this->content;
}

} // namespace kzg::structure