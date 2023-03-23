#ifndef KZG_COMMITMENT_OPENING_KEY_H
#define KZG_COMMITMENT_OPENING_KEY_H

#include <array>
#include <optional>

#include "group/g1_affine.h"
#include "group/g2_affine.h"
#include "group/g2_prepared.h"

namespace kzg::structure {

/**
 * @brief <tt>OpeningKey</tt> is used to verify opening proofs about a committed polynomial.
 */
class OpeningKey {
public:
    static constexpr int32_t BYTE_SIZE =
            bls12_381::group::G1Affine::BYTE_SIZE + bls12_381::group::G2Affine::BYTE_SIZE * 2;

public:
    /// the generator of group G1.
    bls12_381::group::G1Affine g;
    /// the generator of group G2.
    bls12_381::group::G2Affine h;
    /// beta times the generator of group G2.
    bls12_381::group::G2Affine h_beta;
    /// the generator of group G2, prepared for use in pairings.
    bls12_381::group::G2Prepared h_prepared;
    /// beta times the generator of group G2, prepared for use in pairings.
    bls12_381::group::G2Prepared h_beta_prepared;

public:
    OpeningKey() = delete;
    OpeningKey(
            const bls12_381::group::G1Affine &g,
            const bls12_381::group::G2Affine &h,
            const bls12_381::group::G2Affine &h_beta
    );

    [[nodiscard]] auto to_bytes() const -> std::array<uint8_t, OpeningKey::BYTE_SIZE>;
    static std::optional<OpeningKey> from_bytes(const std::array<uint8_t, OpeningKey::BYTE_SIZE> &bytes);
};

} // namespace kzg::structure

#endif //KZG_COMMITMENT_OPENING_KEY_H