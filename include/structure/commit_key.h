#ifndef KZG_COMMITMENT_COMMIT_KEY_H
#define KZG_COMMITMENT_COMMIT_KEY_H

#include <cstdint>
#include <vector>

#include "group/g1_affine.h"
#include "polynomial/coefficient.h"

namespace kzg::structure {

/**
 * @brief <tt>CommitKey</tt> is used to commit to a polynomial which is bounded by the <tt>max_degree</tt>.
 */
class CommitKey {
private:
    /// Group elements of the form `beta ^ i * g`, where `i` ranges from 0 to <tt>degree</tt>.
    std::vector<bls12_381::group::G1Affine> powers_of_g;

public:
    CommitKey() = delete;
    explicit CommitKey(const std::vector<bls12_381::group::G1Affine> &vec);
    [[nodiscard]] const std::vector<bls12_381::group::G1Affine> &get_powers_of_g() const;

    /**
     * @return the maximum degree of the polynomial that can be committed to.
     */
    [[nodiscard]] auto max_degree() const -> size_t;

    /**
     * Truncates the <tt>CommitKey</tt> to one with smaller <tt>max_degree</tt>.
     * @param new_degree the new value of <tt>max_degree</tt>.
     * @return The truncated <tt>CommitKey</tt>.
     * @exception TRUNCATED_DEGREE_IS_ZERO the <tt>new_degree</tt> is zero.
     * @exception TRUNCATED_DEGREE_TOO_LARGE the <tt>new_degree</tt> is larger than current <tt>max_degree</tt>.
     */
    [[nodiscard]] auto truncate(size_t new_degree) const -> CommitKey;

    /**
     * Checks the degree of the committed polynomial.
     * @param polynomial a to-be-committed polynomial in coefficient form.
     * @exception PolyDegreeIsZero the polynomial has zero degree.
     * @exception PolyDegreeTooLarge degree of the polynomial is larger than <tt>max_degree</tt>.
     */
    void check_polynomial_degree(const polynomial::CoefficientForm &polynomial) const;

    [[nodiscard]] std::vector<uint8_t> to_raw_var_bytes() const;
    [[nodiscard]] std::vector<uint8_t> to_var_bytes() const;

    static CommitKey from_slice_unchecked(const std::vector<uint8_t> &bytes);
    static std::optional<CommitKey> from_slice(const std::vector<uint8_t> &bytes);
};

} // namespace kzg::structure

#endif //KZG_COMMITMENT_COMMIT_KEY_H