#include "structure/reference_string.h"

#include <cassert>
#include <utility>

#include "group/g1_projective.h"
#include "group/g2_affine.h"

#include "exception/exception.h"
#include "utils/field.h"
#include "utils/group.h"

namespace kzg::structure {

using bls12_381::group::G1Affine;
using bls12_381::group::G1Projective;
using bls12_381::group::G2Affine;

using exception::Exception;
using exception::Type;
using util::field::random_scalar;
using util::field::generate_vec_powers;
using util::group::random_g1_point;
using util::group::random_g2_point;
using util::group::slow_multi_scalar_mul_single_base;

/**
 * the maximum degree is the degree of the constraint system + 6, because adding the blinding factors requires some
 * extra elements for the SRS:
 *      - +1 per wire (4 wires in total);
 *      - +2 for the permutation polynomial.
 */
const size_t EXTRA_BLINDING_DEGREE = 6;

ReferenceString::ReferenceString(CommitKey commit_key, OpeningKey opening_key)
        : commit_key{std::move(commit_key)}, opening_key{std::move(opening_key)} {}

ReferenceString ReferenceString::setup(size_t max_degree, rng::core::RngCore &rng) {
    if (max_degree < 1)
        throw Exception(Type::CIRCUIT_DEGREE_IS_ZERO, "cannot commit to constants.");

    max_degree = max_degree + EXTRA_BLINDING_DEGREE;
    const auto x = random_scalar(rng);
    const auto powers_x = generate_vec_powers(x, max_degree);
    const auto g = random_g1_point(rng);
    const auto powers_g = slow_multi_scalar_mul_single_base(powers_x, g);

    assert(powers_g.size() == max_degree + 1);

    const auto normalized_g = G1Projective::batch_normalize(powers_g);
    const auto h = G2Affine{random_g2_point(rng)};
    const auto x_2 = G2Affine{h * x};

    return ReferenceString{CommitKey{normalized_g}, OpeningKey{G1Affine{g}, h, x_2}};
}

std::tuple<CommitKey, OpeningKey> ReferenceString::trim(size_t truncated_degree) {
    auto truncated_prover_key = this->commit_key.truncate(truncated_degree + EXTRA_BLINDING_DEGREE);
    return {truncated_prover_key, this->opening_key};
}

auto ReferenceString::to_var_bytes() const -> std::vector<uint8_t> {
    const auto bytes_opening = this->opening_key.to_bytes();
    const auto bytes_commit = this->commit_key.to_var_bytes();

    std::vector<uint8_t> res{};
    res.reserve(240 + bytes_commit.size());

    res.insert(res.end(), bytes_opening.begin(), bytes_opening.end());
    res.insert(res.end(), bytes_commit.begin(), bytes_commit.end());

    return res;
}

auto ReferenceString::to_raw_var_bytes() const -> std::vector<uint8_t> {
    const auto bytes_opening = this->opening_key.to_bytes();
    const auto bytes_commit = this->commit_key.to_raw_var_bytes();

    std::vector<uint8_t> res{};
    res.reserve(240 + bytes_commit.size());

    res.insert(res.end(), bytes_opening.begin(), bytes_opening.end());
    res.insert(res.end(), bytes_commit.begin(), bytes_commit.end());

    return res;
}

auto ReferenceString::from_slice(const std::vector<uint8_t> &bytes) -> std::optional<ReferenceString> {
    if (bytes.size() <= OpeningKey::BYTE_SIZE)
        throw Exception(Type::SERIALIZE_NO_ENOUGH_BYTES, "input bytes not long enough.");

    std::array<uint8_t, OpeningKey::BYTE_SIZE> bytes_opening{};
    std::vector<uint8_t> bytes_commit{};
    bytes_commit.reserve(bytes.size() - OpeningKey::BYTE_SIZE);

    std::copy(bytes.begin(), bytes.begin() + OpeningKey::BYTE_SIZE, bytes_opening.begin());
    bytes_commit.insert(bytes_commit.end(), bytes.begin() + OpeningKey::BYTE_SIZE, bytes.end());


    const auto opening_key_opt = OpeningKey::from_bytes(bytes_opening);
    const auto commit_key_opt = CommitKey::from_slice(bytes_commit);

    if (!opening_key_opt.has_value() || !commit_key_opt.has_value()) return std::nullopt;
    return ReferenceString{commit_key_opt.value(), opening_key_opt.value()};
}

auto ReferenceString::from_slice_unchecked(const std::vector<uint8_t> &bytes) -> ReferenceString {
    std::array<uint8_t, OpeningKey::BYTE_SIZE> bytes_opening{};
    std::vector<uint8_t> bytes_commit{};
    bytes_commit.reserve(bytes.size() - OpeningKey::BYTE_SIZE);

    std::copy(bytes.begin(), bytes.begin() + OpeningKey::BYTE_SIZE, bytes_opening.begin());
    bytes_commit.insert(bytes_commit.end(), bytes.begin() + OpeningKey::BYTE_SIZE, bytes.end());

    const auto opening_key = OpeningKey::from_bytes(bytes_opening).value();
    const auto commit_key = CommitKey::from_slice_unchecked(bytes_commit);

    return ReferenceString{commit_key, opening_key};
}

} // namespace kzg::structure