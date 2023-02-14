#ifndef KZG_COMMITMENT_COMMITMENT_H
#define KZG_COMMITMENT_COMMITMENT_H

#include "group/g1_affine.h"
#include "group/g1_projective.h"

namespace kzg::structure {

class Commitment {
private:
    bls12_381::group::G1Affine content;

public:
    Commitment();
    explicit Commitment(const bls12_381::group::G1Affine &element);
    explicit Commitment(bls12_381::group::G1Affine &&element) noexcept;
    explicit Commitment(const bls12_381::group::G1Projective &element);

    static Commitment identity() noexcept;

    [[nodiscard]] bls12_381::group::G1Affine get_content() const;
};

}

#endif //KZG_COMMITMENT_COMMITMENT_H