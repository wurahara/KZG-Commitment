#include "util/field.h"

#include <array>

#include "util/random.h"

namespace kzg::util::field {

bls12_381::scalar::Scalar generate_random_scalar() {
    std::array<uint8_t, bls12_381::scalar::Scalar::BYTE_SIZE * 2> bytes{};
    for (uint8_t &byte: bytes)
        byte = random::get_random<uint8_t>();
    return bls12_381::scalar::Scalar::from_bytes_wide(bytes);
}

std::vector<bls12_381::scalar::Scalar> generate_vec_powers(const bls12_381::scalar::Scalar &value, size_t max_degree) {
    std::vector<bls12_381::scalar::Scalar> monomials;
    monomials.reserve(max_degree + 1);
    monomials.push_back(bls12_381::scalar::Scalar::one());

    for (int i = 1; i <= max_degree; ++i)
        monomials.push_back(monomials[i - 1] * value);

    return monomials;
}

void batch_inversion(std::vector<bls12_381::scalar::Scalar> &scalars) {
    std::vector<bls12_381::scalar::Scalar> prod;
    prod.reserve(scalars.size());
    bls12_381::scalar::Scalar temp = bls12_381::scalar::Scalar::one();

    for (const auto &scalar: scalars) {
        if (scalar != bls12_381::scalar::Scalar::zero()) {
            temp *= scalar;
            prod.push_back(temp);
        }
    }

    temp = temp.invert().value();

    int32_t sentinel = static_cast<int32_t>(prod.size()) - 2;
    for (auto iter = scalars.rbegin(); iter != scalars.rend(); iter++) { // NOLINT(modernize-loop-convert)
        if (*iter == bls12_381::scalar::Scalar::zero()) continue;
        if (sentinel < -1) break;

        bls12_381::scalar::Scalar new_temp = temp * *iter;
        *iter = temp * ((sentinel != -1) ? prod[sentinel] : bls12_381::scalar::Scalar::one());
        temp = new_temp;
        sentinel--;
    }

}


} // namespace kzg::util::field