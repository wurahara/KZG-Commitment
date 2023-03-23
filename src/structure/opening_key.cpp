#include "structure/opening_key.h"

namespace kzg::structure {

using bls12_381::group::G1Affine;
using bls12_381::group::G2Affine;
using bls12_381::group::G2Prepared;
using kzg::structure::OpeningKey;

OpeningKey::OpeningKey(const G1Affine &g, const G2Affine &h, const G2Affine &h_beta) // NOLINT(modernize-pass-by-value)
        : g{g}, h{h}, h_beta{h_beta}, h_prepared{G2Prepared{h}}, h_beta_prepared{G2Prepared{h_beta}} {}

auto OpeningKey::to_bytes() const -> std::array<uint8_t, OpeningKey::BYTE_SIZE> {
    const auto bytes_g = this->g.to_compressed();
    const auto bytes_h = this->h.to_compressed();
    const auto bytes_h_beta = this->h_beta.to_compressed();

    std::array<uint8_t, OpeningKey::BYTE_SIZE> res{};
    std::copy(bytes_g.begin(), bytes_g.end(), res.begin());
    std::copy(bytes_h.begin(), bytes_h.end(), res.begin() + G1Affine::BYTE_SIZE);
    std::copy(bytes_h_beta.begin(), bytes_h_beta.end(), res.begin() + G1Affine::BYTE_SIZE + G2Affine::BYTE_SIZE);

    return res;
}

std::optional<OpeningKey> OpeningKey::from_bytes(const std::array<uint8_t, OpeningKey::BYTE_SIZE> &bytes) {
    std::array<uint8_t, G1Affine::BYTE_SIZE> bytes_g{};
    std::array<uint8_t, G2Affine::BYTE_SIZE> bytes_h{};
    std::array<uint8_t, G2Affine::BYTE_SIZE> bytes_h_beta{};

    std::copy(bytes.begin(), bytes.begin() + G1Affine::BYTE_SIZE, bytes_g.begin());
    std::copy(bytes.begin() + G1Affine::BYTE_SIZE, bytes.begin() + G1Affine::BYTE_SIZE + G2Affine::BYTE_SIZE,
              bytes_h.begin());
    std::copy(bytes.begin() + G1Affine::BYTE_SIZE + G2Affine::BYTE_SIZE, bytes.end(), bytes_h_beta.begin());

    const auto g_opt = G1Affine::from_compressed(bytes_g);
    const auto h_opt = G2Affine::from_compressed(bytes_h);
    const auto h_beta_opt = G2Affine::from_compressed(bytes_h_beta);

    if (!g_opt.has_value() || !h_opt.has_value() || !h_beta_opt.has_value())
        return std::nullopt;

    return OpeningKey{g_opt.value(), h_opt.value(), h_beta_opt.value()};
}

} // namespace kzg::structure