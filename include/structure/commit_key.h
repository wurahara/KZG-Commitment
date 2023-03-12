#ifndef KZG_COMMITMENT_COMMIT_KEY_H
#define KZG_COMMITMENT_COMMIT_KEY_H

#include <cstdint>
#include <vector>

#include "group/g1_affine.h"
#include "polynomial/coefficient.h"

namespace kzg::structure {

class CommitKey {
private:
    std::vector<bls12_381::group::G1Affine> vec;

public:
    CommitKey() = delete;
    explicit CommitKey(const std::vector<bls12_381::group::G1Affine> &vec);

    [[nodiscard]] auto max_degree() const -> size_t;
    [[nodiscard]] auto truncate(size_t new_degree) const -> CommitKey;
    [[nodiscard]] auto get_vec() const -> std::vector<bls12_381::group::G1Affine>;

    void check_polynomial_degree(const polynomial::CoefficientForm &polynomial) const;

    [[nodiscard]] std::vector<uint8_t> to_raw_var_bytes() const;
    [[nodiscard]] std::vector<uint8_t> to_var_bytes() const;

    static CommitKey from_slice_unchecked(const std::vector<uint8_t> &bytes);
    static std::optional<CommitKey> from_slice(const std::vector<uint8_t> &bytes);
};

} // namespace kzg::structure

#endif //KZG_COMMITMENT_COMMIT_KEY_H