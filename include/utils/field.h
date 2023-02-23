#ifndef KZG_COMMITMENT_FIELD_H
#define KZG_COMMITMENT_FIELD_H

#include <vector>

#include "scalar/scalar.h"

namespace kzg::util::field {

bls12_381::scalar::Scalar generate_random_scalar();
std::vector<bls12_381::scalar::Scalar> generate_vec_powers(const bls12_381::scalar::Scalar &value, size_t max_degree);
void batch_inversion(std::vector<bls12_381::scalar::Scalar> &scalars);

} // namespace kzg::util::field

#endif //KZG_COMMITMENT_FIELD_H