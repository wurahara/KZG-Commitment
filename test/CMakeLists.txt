# Test suite provided by google test (GTest)
INCLUDE(FetchContent)
FETCHCONTENT_DECLARE(
        googletest
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
SET(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FETCHCONTENT_MAKEAVAILABLE(googletest)
ENABLE_TESTING()

FILE(GLOB_RECURSE TEST_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)
ADD_EXECUTABLE(
        KZG_TEST
        ${TEST_FILES}
)

TARGET_LINK_LIBRARIES(
        KZG_TEST
        GTest::gtest_main
        KZG_Commitment
)

INCLUDE(GoogleTest)
GTEST_DISCOVER_TESTS(KZG_TEST)