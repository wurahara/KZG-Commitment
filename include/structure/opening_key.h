#ifndef KZG_COMMITMENT_OPENING_KEY_H
#define KZG_COMMITMENT_OPENING_KEY_H

#include "group/g1_affine.h"
#include "group/g2_affine.h"
#include "group/g2_prepared.h"

namespace kzg::structure {

class OpeningKey {
public:
    bls12_381::group::G1Affine g;
    bls12_381::group::G2Affine h;
    bls12_381::group::G2Affine h_beta;
    bls12_381::group::G2Prepared h_prepared;
    bls12_381::group::G2Prepared h_beta_prepared;
    
public:
    OpeningKey() = delete;
    OpeningKey(const bls12_381::group::G1Affine &g, const bls12_381::group::G2Affine &h,
               const bls12_381::group::G2Affine &h_beta);
};

} // namespace kzg::structure

#endif //KZG_COMMITMENT_OPENING_KEY_H