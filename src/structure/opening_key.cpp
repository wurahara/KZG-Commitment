#include "structure/opening_key.h"

namespace kzg::structure {

using bls12_381::group::G1Affine;
using bls12_381::group::G2Affine;
using bls12_381::group::G2Prepared;
using kzg::structure::OpeningKey;

OpeningKey::OpeningKey(const G1Affine &g, const G2Affine &h, const G2Affine &h_beta) // NOLINT(modernize-pass-by-value)
        : g{g}, h{h}, h_beta{h_beta}, h_prepared{G2Prepared{h}}, h_beta_prepared{G2Prepared{h_beta}} {}

} // namespace kzg::structure