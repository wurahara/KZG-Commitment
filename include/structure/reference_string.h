#ifndef KZG_COMMITMENT_REFERENCE_STRING_H
#define KZG_COMMITMENT_REFERENCE_STRING_H

#include <cstdint>
#include <tuple>

#include "structure/commit_key.h"
#include "structure/opening_key.h"

namespace kzg::structure {

class ReferenceString {
public:
    static constexpr size_t EXTRA_BLINDING_DEGREE = 6;
private:
    CommitKey commit_key;
    OpeningKey opening_key;

private:
    ReferenceString(CommitKey commit_key, OpeningKey opening_key);

public:
    static auto setup(size_t max_degree) -> ReferenceString;
    auto trim(size_t truncated_degree) -> std::tuple<CommitKey, OpeningKey>;
};

} // namespace kzg::structure

#endif //KZG_COMMITMENT_REFERENCE_STRING_H