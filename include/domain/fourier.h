#ifndef KZG_COMMITMENT_FOURIER_H
#define KZG_COMMITMENT_FOURIER_H

#include <cstdint>
#include <vector>

#include "scalar/scalar.h"

namespace kzg::domain {

void distribute_powers(const std::vector<bls12_381::scalar::Scalar> &coefficients, const bls12_381::scalar::Scalar &generator);
void serial_fast_fourier(std::vector<bls12_381::scalar::Scalar> &a, const bls12_381::scalar::Scalar &omega, uint32_t log_size);

} // namespace kzg::domain

#endif //KZG_COMMITMENT_FOURIER_H
