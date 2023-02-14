#include "structure/opening_key.h"

namespace kzg::structure {
kzg::structure::OpeningKey::OpeningKey(
        const bls12_381::group::G1Affine &g, // NOLINT(modernize-pass-by-value)
        const bls12_381::group::G2Affine &h,
        const bls12_381::group::G2Affine &h_beta)
        : g{g}, h{h}, h_beta{h_beta},
          h_prepared{bls12_381::group::G2Prepared{h}},
          h_beta_prepared{bls12_381::group::G2Prepared{h_beta}} {}

} // namespace kzg::structure