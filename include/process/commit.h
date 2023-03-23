#ifndef KZG_COMMITMENT_COMMIT_H
#define KZG_COMMITMENT_COMMIT_H

#include "polynomial/coefficient.h"
#include "structure/commit_key.h"
#include "structure/commitment.h"

namespace kzg::process::commit {

/**
 * @brief Commits to a polynomial in coefficient form.
 * @param commit_key the committing key.
 * @param polynomial the to-be-committed polynomial in coefficient form.
 * @return the corresponding commitment.
 */
auto commit(
        const structure::CommitKey &commit_key,
        const polynomial::CoefficientForm &polynomial
) -> structure::Commitment;

} // namespace kzg::process::commit

#endif //KZG_COMMITMENT_COMMIT_H