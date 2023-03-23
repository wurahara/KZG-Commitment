#include <gtest/gtest.h>

#include "impl/os_rng.h"

#include "structure/reference_string.h"

using kzg::structure::ReferenceString;

TEST(ReferenceString, Serialize) {
    rng::impl::OsRng rng{};
    const ReferenceString pp = ReferenceString::setup(1 << 7, rng);
    const auto bytes = pp.to_raw_var_bytes();
    const auto pp_p = ReferenceString::from_slice_unchecked(bytes);
    const auto bytes_2 = pp_p.to_raw_var_bytes();

    EXPECT_EQ(bytes, bytes_2);
}

TEST(ReferenceString, Serialize2) {
    rng::impl::OsRng rng{};
    const ReferenceString pp = ReferenceString::setup(1 << 7, rng);
    const auto bytes = pp.to_var_bytes();
    const auto pp_p = ReferenceString::from_slice(bytes);
    const auto bytes_2 = pp_p->to_var_bytes();

    EXPECT_EQ(bytes, bytes_2);
}