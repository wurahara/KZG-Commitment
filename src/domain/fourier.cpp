#include "domain/fourier.h"

#include <cassert>

namespace kzg::domain {

using bls12_381::scalar::Scalar;

constexpr uint32_t bit_reverse(uint32_t num, uint32_t length) {
    uint32_t res = 0;
    for (int i = 0; i < length; ++i) {
        res = (res << 1) | (num & 1);
        num >>= 1;
    }
    return res;
}

void distribute_powers(const std::vector<Scalar> &coefficients,
                       const Scalar &generator) {
    Scalar power = Scalar::one();
    for (auto coeff: coefficients) {
        coeff *= power;
        power *= generator;
    }
}

void serial_fast_fourier(std::vector<Scalar> &a, const Scalar &omega, uint32_t log_size) {
    const auto n = static_cast<uint32_t>(a.size());
    assert(n == (1 << log_size));

    for (int k = 0; k < n; ++k) {
        const uint32_t rk = bit_reverse(k, log_size);
        if (k < rk) std::swap(a[rk], a[k]);
    }
    uint32_t m = 1;
    for (int i = 0; i < log_size; ++i) {
        const Scalar omega_m = omega.pow({n / (2 * m), 0, 0, 0});
        uint32_t k = 0;
        while (k < n) {
            Scalar omega_i = Scalar::one();
            for (int j = 0; j < m; ++j) {
                Scalar t = a[k + j + m];
                Scalar temp = a[k + j];
                t *= omega_i;
                temp -= t;
                a[k + j + m] = temp;
                a[k + j] += t;
                omega_i *= omega_m;
            }
            k += 2 * m;
        }
        m *= 2;
    }
}

} // namespace kzg::domain