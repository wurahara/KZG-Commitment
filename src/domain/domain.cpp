#include "domain/domain.h"

#include <cassert>
#include <cstdint>
#include <utility>

#include "scalar/constant.h"

#include "domain/iterator.h"
#include "polynomial/evaluation.h"
#include "utils/field.h"
#include "utils/bit.h"

namespace kzg::domain {

using rng::util::bit::to_le_bytes;
using rng::util::bit::from_le_bytes;
using bls12_381::scalar::Scalar;

const uint64_t TWO_ADACITY = 32;

EvaluationDomain::EvaluationDomain() : group_gen{Scalar::zero()}, domain_size{} {}

EvaluationDomain::EvaluationDomain(const EvaluationDomain &domain) = default;

EvaluationDomain::EvaluationDomain(EvaluationDomain &&domain) noexcept = default;

EvaluationDomain::EvaluationDomain(Scalar generator, uint64_t size)
        : group_gen{std::move(generator)}, domain_size{size} {}

constexpr uint64_t next_power_of_two(uint64_t x) noexcept {
    return x == 1 ? 1 : 1 << (64 - __builtin_clzl(x - 1));
}

constexpr uint64_t trailing_zeros(uint64_t x) noexcept {
    return __builtin_ctzl(x);
}

EvaluationDomain::EvaluationDomain(uint64_t num_of_coefficients) {
    const uint64_t size = next_power_of_two(num_of_coefficients);
    const uint64_t log_size = trailing_zeros(size);

    assert(log_size <= TWO_ADACITY);

    Scalar group_generator = bls12_381::scalar::constant::ROOT_OF_UNITY;
    for (uint64_t i = log_size; i < TWO_ADACITY; ++i)
        group_generator = group_generator.square();
    this->domain_size = size;
    this->group_gen = group_generator;
}

size_t EvaluationDomain::size() const noexcept {
    return static_cast<size_t>(this->domain_size);
}

size_t EvaluationDomain::log_size() const noexcept {
    return static_cast<size_t>(trailing_zeros(this->domain_size));
}

Scalar EvaluationDomain::size_as_field_element() const {
    return Scalar{this->domain_size};
}

Scalar EvaluationDomain::size_inverse() const {
    const auto invert = this->size_as_field_element().invert();
    assert(invert.has_value());
    return invert.value();
}

Scalar EvaluationDomain::group_generator() const {
    return this->group_gen;
}

Scalar EvaluationDomain::group_generator_inverse() const {
    const auto invert = this->group_gen.invert();
    assert(invert.has_value());
    return invert.value();
}

std::vector<Scalar> EvaluationDomain::fast_fourier(std::vector<Scalar> &coefficients) const {
    this->fast_fourier_in_place(coefficients);
    return coefficients;
}

std::vector<Scalar> EvaluationDomain::inverse_fast_fourier(std::vector<Scalar> &evaluations) const {
    this->inverse_fast_fourier_in_place(evaluations);
    return evaluations;
}

std::vector<Scalar> EvaluationDomain::coset_fast_fourier(std::vector<Scalar> &coefficients) const {
    this->coset_fast_fourier_in_place(coefficients);
    return coefficients;
}

std::vector<Scalar> EvaluationDomain::coset_inverse_fast_fourier(std::vector<Scalar> &evaluations) const {
    this->coset_inverse_fast_fourier_in_place(evaluations);
    return evaluations;
}

void EvaluationDomain::fast_fourier_in_place(std::vector<Scalar> &coefficients) const {
    coefficients.resize(this->domain_size, Scalar::zero());
    serial_fast_fourier(coefficients, this->group_gen, this->log_size());
}

void EvaluationDomain::inverse_fast_fourier_in_place(std::vector<Scalar> &evaluations) const {
    evaluations.resize(this->domain_size, Scalar::zero());
    serial_fast_fourier(evaluations, this->group_generator_inverse(), this->log_size());
    for (auto &evaluation: evaluations) evaluation *= this->size_inverse();
}

void EvaluationDomain::coset_fast_fourier_in_place(std::vector<Scalar> &coefficients) const {
    distribute_powers(coefficients, bls12_381::scalar::constant::GENERATOR);
    this->fast_fourier_in_place(coefficients);
}

void EvaluationDomain::coset_inverse_fast_fourier_in_place(std::vector<Scalar> &evaluations) const {
    this->inverse_fast_fourier_in_place(evaluations);
    distribute_powers(evaluations, bls12_381::scalar::constant::GENERATOR.invert().value());
}

polynomial::EvaluationForm EvaluationDomain::evaluate_vanishing_polynomial_over_coset(uint64_t poly_degree) const {
    assert(this->domain_size > poly_degree);
    const Scalar coset_generator = bls12_381::scalar::constant::GENERATOR.pow({poly_degree, 0, 0, 0});
    std::vector<Scalar> v_h;
    v_h.reserve(this->domain_size);
    for (int i = 0; i < this->domain_size; ++i) {
        v_h.push_back(
                coset_generator
                * this->group_gen.pow({poly_degree * i, 0, 0, 0})
                - Scalar::one()
        );
    }
    return polynomial::EvaluationForm{v_h, *this};
}

Scalar EvaluationDomain::evaluate_vanishing_polynomial(const Scalar &tau) const {
    return tau.pow({this->domain_size, 0, 0, 0}) - Scalar::one();
}

std::vector<Scalar>
EvaluationDomain::evaluate_all_lagrange_coefficients(const Scalar &tau) const {
    const size_t size = this->domain_size;
    const Scalar t_size = tau.pow({this->domain_size, 0, 0, 0});
    const Scalar one = Scalar::one();

    std::vector<Scalar> u(size, Scalar::zero());

    if (t_size == one) {
        Scalar omega_i = one;
        for (int i = 0; i < size; ++i) {
            if (omega_i == tau) {
                u[i] = one;
                break;
            }
            omega_i *= this->group_gen;
        }
    } else {
        Scalar l = (t_size - one) * this->size_inverse();
        Scalar r = one;
        std::vector<Scalar> ls(size, Scalar::zero());
        for (int i = 0; i < size; ++i) {
            u[i] = tau - r;
            ls[i] = l;
            l *= this->group_gen;
            r *= this->group_gen;
        }
        util::field::batch_inversion(u);
        for (int i = static_cast<int32_t>(size) - 1; i >= 0; --i) {
            u[i] *= ls[i];
        }
    }
    return u;
}

ElementIterator EvaluationDomain::iter() const {
    return ElementIterator{*this, 0, Scalar::one()};
}

std::optional<EvaluationDomain>
EvaluationDomain::from_bytes(const std::array<uint8_t, EvaluationDomain::BYTE_SIZE> &bytes) {
    std::array<uint8_t, 8> size_bytes{};
    std::array<uint8_t, 32> gen_bytes{};
    std::copy(bytes.begin(), bytes.begin() + 8, size_bytes.begin());
    std::copy(bytes.begin() + 8, bytes.end(), gen_bytes.begin());
    const auto size = from_le_bytes<uint64_t>(size_bytes);
    const auto gen_opt = Scalar::from_bytes(gen_bytes);
    if (!gen_opt.has_value()) return std::nullopt;
    return EvaluationDomain{gen_opt.value(), size};
}

std::array<uint8_t, EvaluationDomain::BYTE_SIZE> EvaluationDomain::to_bytes() const {
    const std::array<uint8_t, 8> size_bytes = to_le_bytes<uint64_t>(this->domain_size);
    const std::array<uint8_t, 32> gen_bytes = this->group_gen.to_bytes();
    std::array<uint8_t, 40> bytes{};
    std::copy(size_bytes.begin(), size_bytes.end(), bytes.begin());
    std::copy(gen_bytes.begin(), gen_bytes.end(), bytes.begin() + 8);
    return bytes;
}

EvaluationDomain &EvaluationDomain::operator=(const EvaluationDomain &rhs) = default;

EvaluationDomain &EvaluationDomain::operator=(EvaluationDomain &&rhs) noexcept = default;

} // namespace kzg::domain