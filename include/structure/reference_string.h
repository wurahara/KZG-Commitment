#ifndef KZG_COMMITMENT_REFERENCE_STRING_H
#define KZG_COMMITMENT_REFERENCE_STRING_H

#include <cstdint>
#include <tuple>
#include <optional>
#include <vector>

#include "core/rng.h"

#include "structure/commit_key.h"
#include "structure/opening_key.h"

namespace kzg::structure {

/**
 * @brief The public parameters can also be referred to as the structured reference string (SRS). It is available to
 *          the prover and the verifier. It allows the prover to make claims about polynomials up to a specific degree,
 *          and allows the verifier to efficiently verify the claims.
 */
class ReferenceString {
private:
    /// the key used to generate proofs.
    CommitKey commit_key;
    /// the key used to verify proofs.
    OpeningKey opening_key;

private:
    ReferenceString(CommitKey commit_key, OpeningKey opening_key);

public:
    /**
     * @brief Generates the public parameters using a random number generator.
     * @remark This method will in most cases be used for testing and exploration. In reality, a trusted third party
     *          or a setup ceremony will be used to generate the SRS.
     * @param max_degree the max degree of the arithmetic circuit.
     * @return the generated reference string.
     * @exception CIRCUIT_DEGREE_IS_ZERO the configured degree is less than one.
     */
    static auto setup(size_t max_degree, rng::core::RngCore &rng) -> ReferenceString;

    /**
     * @brief Trims the public parameters to get one with lower degree.
     * @param truncated_degree the new degree of the truncated reference string.
     * @return a truncated reference string.
     */
    auto trim(size_t truncated_degree) -> std::tuple<CommitKey, OpeningKey>;

    [[nodiscard]] auto to_var_bytes() const -> std::vector<uint8_t>;
    [[nodiscard]] auto to_raw_var_bytes() const -> std::vector<uint8_t>;
    static auto from_slice(const std::vector<uint8_t> &bytes) -> std::optional<ReferenceString>;
    static auto from_slice_unchecked(const std::vector<uint8_t> &bytes) -> ReferenceString;
};

} // namespace kzg::structure

#endif //KZG_COMMITMENT_REFERENCE_STRING_H