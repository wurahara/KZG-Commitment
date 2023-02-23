#include <gtest/gtest.h>

#include <tuple>
#include <vector>

#include "impl/os_rng.h"

#include "polynomial/coefficient.h"
#include "process/commit.h"
#include "process/evaluate.h"
#include "process/verify.h"
#include "structure/commit_key.h"
#include "structure/opening_key.h"
#include "structure/reference_string.h"

using bls12_381::scalar::Scalar;
using rng::impl::OsRng;

using kzg::challenge::TranscriptProtocol;
using kzg::structure::Commitment;
using kzg::structure::CommitKey;
using kzg::structure::OpeningKey;
using kzg::structure::ReferenceString;
using kzg::process::commit::commit;
using kzg::process::evaluate::create_witness_single;
using kzg::process::evaluate::create_witness_multiple_polynomials;
using kzg::process::verify::verify_aggregation;
using kzg::process::verify::verify_multiple_points;
using kzg::process::verify::verify_single_polynomial;
using kzg::process::verify::verify_multiple_polynomials;
using kzg::structure::BatchProof;
using kzg::polynomial::CoefficientForm;

std::tuple<CommitKey, OpeningKey> setup_test(size_t degree) {
    auto srs = ReferenceString::setup(degree);
    return srs.trim(degree);
}

TEST(Commitment, CommitSingle) {
    // 1. setup
    const size_t degree = 25;
    const auto [commit_key, opening_key] = setup_test(degree);

    // 2. commit
    OsRng osRng;
    const auto polynomial = CoefficientForm::random(degree, osRng);
    const auto commitment = commit(commit_key, polynomial);

    // 3. evaluate
    const auto point = Scalar{10};
    const auto proof = create_witness_single(commit_key, polynomial, point);

    // 4. verify
    const bool verify = verify_single_polynomial(opening_key, commitment, proof);
    EXPECT_TRUE(verify);
}

TEST(Commitment, CommitMultiple) {
    // 1. setup
    const size_t degree = 27;
    const auto [commit_key, opening_key] = setup_test(degree);

    // 2. commit
    OsRng osRng;
    const auto poly_1 = CoefficientForm::random(25, osRng);
    const auto poly_2 = CoefficientForm::random(27, osRng);
    const auto poly_3 = CoefficientForm::random(27, osRng);
    const auto comm_1 = commit(commit_key, poly_1);
    const auto comm_2 = commit(commit_key, poly_2);
    const auto comm_3 = commit(commit_key, poly_3);
    const std::vector<Commitment> commitments = {comm_1, comm_2, comm_3};
    const std::vector<CoefficientForm> polynomials = {poly_1, poly_2, poly_3};

    // 3. evaluate
    const auto point = Scalar{10};
    auto prover_transcript = TranscriptProtocol{"agg_flatten"};
    const auto aggregated_proof = create_witness_multiple_polynomials(
            commit_key, polynomials, point, prover_transcript
    );

    // 4. verify
    auto verifier_transcript = TranscriptProtocol{"agg_flatten"};
    const bool verify = verify_multiple_polynomials(
            opening_key, commitments, aggregated_proof, verifier_transcript
    );
    EXPECT_TRUE(verify);
}

TEST(Commitment, CommitTwoPoints) {
    // 1. setup
    const size_t degree = 25;
    const auto [commit_key, opening_key] = setup_test(degree);

    // 2. commit
    OsRng osRng;
    const auto poly_1 = CoefficientForm::random(degree, osRng);
    const auto poly_2 = CoefficientForm::random(degree, osRng);
    const auto comm_1 = commit(commit_key, poly_1);
    const auto comm_2 = commit(commit_key, poly_2);
    const std::vector<Commitment> commitments = {comm_1, comm_2};
    const std::vector<CoefficientForm> polynomials = {poly_1, poly_2};

    // 3. evaluate
    const auto point_1 = Scalar{10};
    const auto point_2 = Scalar{11};
    const auto proof_1 = create_witness_single(commit_key, poly_1, point_1);
    const auto proof_2 = create_witness_single(commit_key, poly_2, point_2);

    // 4. separate verify
    const bool verify_1 = verify_single_polynomial(opening_key, comm_1, proof_1);
    const bool verify_2 = verify_single_polynomial(opening_key, comm_2, proof_2);
    EXPECT_TRUE(verify_1);
    EXPECT_TRUE(verify_2);

    // 5. batch verify
    const std::vector<Scalar> points = {point_1, point_2};
    const std::vector<Scalar> evaluations = {proof_1.evaluation, proof_2.evaluation};
    const std::vector<Commitment> witnesses = {proof_1.witness, proof_2.witness};
    const auto batch_proof = BatchProof{points, evaluations, witnesses};

    auto transcript = TranscriptProtocol{"??"};
    const bool verify_batch = verify_multiple_points(opening_key, commitments, batch_proof, transcript);
    EXPECT_TRUE(verify_batch);
}

TEST(Commitment, CommitMultiplePointsWithAggregation) {
    // 1. setup
    const size_t degree = 28;
    const auto [commit_key, opening_key] = setup_test(degree);

    // 2. commit
    OsRng osRng;
    const auto poly_1 = CoefficientForm::random(degree, osRng);
    const auto poly_2 = CoefficientForm::random(degree, osRng);
    const auto poly_3 = CoefficientForm::random(degree, osRng);
    const auto poly_4 = CoefficientForm::random(degree, osRng);
    const auto comm_1 = commit(commit_key, poly_1);
    const auto comm_2 = commit(commit_key, poly_2);
    const auto comm_3 = commit(commit_key, poly_3);
    const auto comm_4 = commit(commit_key, poly_4);
    const std::vector<Commitment> commitment_vec = {comm_1, comm_2, comm_3};

    // 3. evaluate
    const auto point_1 = Scalar{10};
    const auto point_2 = Scalar{11};
    auto prover_transcript = TranscriptProtocol{"agg_batch"};
    const std::vector<CoefficientForm> polynomials = {poly_1, poly_2, poly_3};
    const auto aggregated_proof = create_witness_multiple_polynomials(
            commit_key, polynomials, point_1, prover_transcript
    );
    const auto proof = create_witness_single(commit_key, poly_4, point_2);

    // 4. verify
    auto verifier_transcript = TranscriptProtocol{"agg_batch"};
    const auto [commitment_agg, evaluation_agg] =
            verify_aggregation(commitment_vec, aggregated_proof.evaluations, verifier_transcript);
    const std::vector<Commitment> commitments = {commitment_agg, comm_4};
    const std::vector<Scalar> points = {point_1, point_2};
    const std::vector<Scalar> evaluations = {evaluation_agg, proof.evaluation};
    const std::vector<Commitment> witnesses = {aggregated_proof.witness, proof.witness};

    const auto batch_proof = BatchProof{points, evaluations, witnesses};
    const bool verify = verify_multiple_points(opening_key, commitments, batch_proof, verifier_transcript);
    EXPECT_TRUE(verify);
}