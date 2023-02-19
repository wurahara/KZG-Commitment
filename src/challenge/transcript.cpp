#include "challenge/transcript.h"

#include <array>
#include <vector>

namespace kzg::challenge {

TranscriptProtocol::TranscriptProtocol(const std::string_view &label) : Transcript(label) {}

void
TranscriptProtocol::append_commitment(const std::string_view &label, const bls12_381::group::G1Affine &commitment) {
    auto bytes = commitment.to_compressed();
    std::vector<uint8_t> commitment_vec{bytes.begin(), bytes.end()};
    this->append_message(label, commitment_vec);
}

void TranscriptProtocol::append_scalar(const std::string_view &label, const bls12_381::scalar::Scalar &scalar) {
    auto bytes = scalar.to_bytes();
    std::vector<uint8_t> scalar_vec{bytes.begin(), bytes.end()};
    this->append_message(label, scalar_vec);
}

bls12_381::scalar::Scalar TranscriptProtocol::challenge_scalar(const std::string_view &label) {
    using bls12_381::scalar::Scalar;

    std::vector<uint8_t> buffer;
    buffer.resize(64);
    this->challenge_bytes(label, buffer);

    std::array<uint8_t, Scalar::BYTE_SIZE * 2> bytes{};
    std::copy(buffer.begin(), buffer.begin() + Scalar::BYTE_SIZE * 2, bytes.begin());

    return Scalar::from_bytes_wide(bytes);
}

} // namespace kzg::challenge