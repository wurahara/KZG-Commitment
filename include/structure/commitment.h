#ifndef KZG_COMMITMENT_COMMITMENT_H
#define KZG_COMMITMENT_COMMITMENT_H

#include "group/g1_affine.h"
#include "group/g1_projective.h"

namespace kzg::structure {

class Commitment {
public:
    static constexpr int32_t BYTE_SIZE = bls12_381::group::G1Affine::BYTE_SIZE;

private:
    bls12_381::group::G1Affine content;

public:
    Commitment();
    explicit Commitment(const bls12_381::group::G1Affine &element);
    explicit Commitment(bls12_381::group::G1Affine &&element) noexcept;
    explicit Commitment(const bls12_381::group::G1Projective &element);

    static Commitment identity() noexcept;
    [[nodiscard]] auto get_content() const -> bls12_381::group::G1Affine;

    [[nodiscard]] auto to_bytes() const -> std::array<uint8_t, BYTE_SIZE>;
    static auto from_bytes(const std::array<uint8_t, BYTE_SIZE> &bytes) -> std::optional<Commitment>;
};

}

#endif //KZG_COMMITMENT_COMMITMENT_H