#include "structure/commit_key.h"

#include <cassert>

namespace kzg::structure {

CommitKey::CommitKey(const std::vector<bls12_381::group::G1Affine> &vec) : vec{vec} {}

size_t CommitKey::max_degree() const {
    return this->vec.size() - 1;
}

CommitKey CommitKey::truncate(size_t new_degree) const {
    assert(new_degree != 0);
    assert(new_degree <= this->max_degree());

    if (new_degree == 1) new_degree += 1;

    std::vector<bls12_381::group::G1Affine> new_vec{
        this->vec.begin(), this->vec.begin() + static_cast<long>(new_degree) + 1
    };
    return CommitKey{new_vec};
}

std::vector<bls12_381::group::G1Affine> CommitKey::get_vec() const {
    return this->vec;
}

void CommitKey::check_polynomial_degree(const polynomial::CoefficientForm &polynomial) const {
    size_t poly_degree = polynomial.degree();
    assert(poly_degree != 0);
    assert(poly_degree <= this->max_degree());
}

} // namespace kzg::structure