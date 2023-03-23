#include "structure/commit_key.h"

#include "utils/bit.h"

#include "exception/exception.h"

namespace kzg::structure {

using rng::util::bit::to_le_bytes;
using rng::util::bit::from_le_bytes;
using bls12_381::group::G1Affine;

using exception::Exception;
using exception::Type;
using polynomial::CoefficientForm;

CommitKey::CommitKey(const std::vector<G1Affine> &vec) : powers_of_g{vec} {}

size_t CommitKey::max_degree() const {
    return this->powers_of_g.size() - 1;
}

CommitKey CommitKey::truncate(size_t new_degree) const {
    if (new_degree == 0)
        throw Exception(Type::TRUNCATED_DEGREE_IS_ZERO, "the input degree is zero.");
    if (new_degree > this->max_degree())
        throw Exception(Type::TRUNCATED_DEGREE_TOO_LARGE, "the input degree is too large.");

    if (new_degree == 1) new_degree += 1;
    std::vector<G1Affine> new_vec{this->powers_of_g.begin(), this->powers_of_g.begin() + static_cast<long>(new_degree) + 1};
    return CommitKey{new_vec};
}

const std::vector<bls12_381::group::G1Affine> &CommitKey::get_powers_of_g() const {
    return this->powers_of_g;
}

void CommitKey::check_polynomial_degree(const CoefficientForm &polynomial) const {
    size_t poly_degree = polynomial.degree();
    if (poly_degree == 0)
        throw Exception(Type::POLY_DEGREE_IS_ZERO, "the committed polynomial has zero degree.");
    if (poly_degree > this->max_degree())
        throw Exception(Type::POLY_DEGREE_TOO_LARGE, "degree of the committed polynomial is too large.");
}

std::vector<uint8_t> CommitKey::to_raw_var_bytes() const {
    std::vector<uint8_t> bytes{};
    bytes.reserve(sizeof(uint64_t) + this->powers_of_g.size() * G1Affine::RAW_SIZE);

    const auto size = static_cast<uint64_t>(this->powers_of_g.size());
    const auto size_bytes = to_le_bytes<uint64_t>(size);
    bytes.insert(bytes.end(), size_bytes.begin(), size_bytes.end());

    for (const G1Affine &point: this->powers_of_g) {
        const auto point_bytes = point.to_raw_bytes();
        bytes.insert(bytes.end(), point_bytes.begin(), point_bytes.end());
    }

    return bytes;
}

std::vector<uint8_t> CommitKey::to_var_bytes() const {
    std::vector<uint8_t> bytes{};
    bytes.reserve(this->powers_of_g.size() * G1Affine::BYTE_SIZE);

    for (const G1Affine &point: this->powers_of_g) {
        const auto point_bytes = point.to_compressed();
        bytes.insert(bytes.end(), point_bytes.begin(), point_bytes.end());
    }

    return bytes;
}

CommitKey CommitKey::from_slice_unchecked(const std::vector<uint8_t> &bytes) {
    std::array<uint8_t, sizeof(uint64_t)> size_bytes{};
    std::copy(bytes.begin(), bytes.begin() + sizeof(uint64_t), size_bytes.begin());
    const auto size = from_le_bytes<uint64_t>(size_bytes);

    std::vector<G1Affine> powers_of_g;
    powers_of_g.reserve(size);

    for (int i = sizeof(uint64_t); i < bytes.size(); i += G1Affine::RAW_SIZE) {
        std::vector<uint8_t> point_bytes{};
        point_bytes.reserve(G1Affine::RAW_SIZE);
        std::copy(bytes.begin() + i, bytes.begin() + i + G1Affine::RAW_SIZE, point_bytes.begin());
        const G1Affine point = G1Affine::from_slice_unchecked(point_bytes);
        powers_of_g.push_back(point);
    }

    return CommitKey{powers_of_g};
}

std::optional<CommitKey> CommitKey::from_slice(const std::vector<uint8_t> &bytes) {
    const uint64_t size = bytes.size() / G1Affine::BYTE_SIZE;

    std::vector<G1Affine> powers_of_g;
    powers_of_g.reserve(size);

    for (int i = 0; i < bytes.size(); i += G1Affine::BYTE_SIZE) {
        std::array<uint8_t, G1Affine::BYTE_SIZE> point_bytes{};
        std::copy(bytes.begin() + i, bytes.begin() + i + G1Affine::BYTE_SIZE, point_bytes.begin());
        const auto point_opt = G1Affine::from_compressed(point_bytes);
        if (!point_opt.has_value()) return std::nullopt;
        powers_of_g.push_back(point_opt.value());
    }

    return CommitKey{powers_of_g};
}

} // namespace kzg::structure