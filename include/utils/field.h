#ifndef KZG_COMMITMENT_FIELD_H
#define KZG_COMMITMENT_FIELD_H

#include <vector>

#include "core/rng.h"
#include "scalar/scalar.h"

namespace kzg::util::field {

/**
 * @brief Generates a random <tt>Scalar</tt> using an RNG seed.
 * @param rng the random number generator
 * @return a random <tt>Scalar</tt>
 */
auto random_scalar(rng::core::RngCore &rng) -> bls12_381::scalar::Scalar;

std::vector<bls12_381::scalar::Scalar> generate_vec_powers(const bls12_381::scalar::Scalar &value, size_t max_degree);
void batch_inversion(std::vector<bls12_381::scalar::Scalar> &scalars);

} // namespace kzg::util::field

#endif //KZG_COMMITMENT_FIELD_H