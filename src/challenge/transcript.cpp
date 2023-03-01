#include "challenge/transcript.h"

#include <array>
#include <vector>

namespace kzg::challenge {

using bls12_381::scalar::Scalar;
using structure::Commitment;

TranscriptProtocol::TranscriptProtocol(const std::string_view &label) : Transcript(label) {}

void TranscriptProtocol::append_commitment(const std::string_view &label, const Commitment &commitment) {
    auto bytes = commitment.get_content().to_compressed();
    std::vector<uint8_t> commitment_vec{bytes.begin(), bytes.end()};
    this->append_message(label, commitment_vec);
}

void TranscriptProtocol::append_scalar(const std::string_view &label, const Scalar &scalar) {
    auto bytes = scalar.to_bytes();
    std::vector<uint8_t> scalar_vec{bytes.begin(), bytes.end()};
    this->append_message(label, scalar_vec);
}

Scalar TranscriptProtocol::challenge_scalar(const std::string_view &label) {
    std::vector<uint8_t> buffer;
    buffer.resize(64);
    this->challenge_bytes(label, buffer);

    std::array<uint8_t, Scalar::BYTE_SIZE * 2> bytes{};
    std::copy(buffer.begin(), buffer.begin() + Scalar::BYTE_SIZE * 2, bytes.begin());

    return Scalar::from_bytes_wide(bytes);
}

} // namespace kzg::challenge