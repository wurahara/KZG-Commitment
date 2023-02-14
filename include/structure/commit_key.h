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

    [[nodiscard]] size_t max_degree() const;
    [[nodiscard]] CommitKey truncate(size_t new_degree) const;
    [[nodiscard]] std::vector<bls12_381::group::G1Affine> get_vec() const;

    void check_polynomial_degree(const polynomial::CoefficientForm &polynomial) const;
};

} // namespace kzg::structure

#endif //KZG_COMMITMENT_COMMIT_KEY_H