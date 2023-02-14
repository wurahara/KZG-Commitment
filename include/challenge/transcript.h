#ifndef KZG_COMMITMENT_TRANSCRIPT_H
#define KZG_COMMITMENT_TRANSCRIPT_H

#include <string_view>

#include "group/g1_affine.h"
#include "scalar/scalar.h"
#include "builder/transcript.h"
#include "strobe/constant.h"

namespace kzg::challenge {

class TranscriptProtocol : public transcript::builder::Transcript {
public:
    explicit TranscriptProtocol(const std::string_view &label);

    void append_commitment(const std::string_view &label, const bls12_381::group::G1Affine &commitment);
    void append_scalar(const std::string_view &label, const bls12_381::scalar::Scalar &scalar);
    bls12_381::scalar::Scalar challenge_scalar(const std::string_view &label);
};

} // namespace kzg::challenge

#endif //KZG_COMMITMENT_TRANSCRIPT_H