INCLUDE(FetchContent)
FETCHCONTENT_DECLARE(
        BLS12_381
        GIT_REPOSITORY https://github.com/herculas/bls12-381.git
        GIT_TAG main
)

FETCHCONTENT_MAKEAVAILABLE(BLS12_381)