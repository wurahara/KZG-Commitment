#include <gtest/gtest.h>
#include <vector>

TEST(TestUtil, ZipSkip) {
    std::vector<uint64_t> a = {1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<uint64_t> b = {1, 3, 5, 6};

    int32_t sentinel = static_cast<int32_t>(b.size()) - 2;
    for (auto iter = a.rbegin(); iter != a.rend(); iter++) { // NOLINT(modernize-loop-convert)
        if (*iter == 1) continue;
        auto s = (sentinel >= 0) ? b[sentinel] : 200000;
        if (sentinel >= 0) {
            *iter *= s;
            sentinel--;
        }
    }
    for (unsigned long long i : a) std::cout << i << " ";
    std::cout << std::endl;
}