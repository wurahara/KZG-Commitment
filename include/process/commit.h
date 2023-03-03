#ifndef KZG_COMMITMENT_COMMIT_H
#define KZG_COMMITMENT_COMMIT_H

#include "polynomial/coefficient.h"
#include "structure/commit_key.h"
#include "structure/commitment.h"

namespace kzg::process::commit {

auto commit(const structure::CommitKey &commit_key, const polynomial::CoefficientForm &polynomial)
-> structure::Commitment;

} // namespace kzg::process::commit

#endif //KZG_COMMITMENT_COMMIT_H