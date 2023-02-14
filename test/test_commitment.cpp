#include <gtest/gtest.h>

#include <tuple>
#include <vector>

#include "polynomial/coefficient.h"
#include "process/commit.h"
#include "process/evaluate.h"
#include "process/verify.h"
#include "structure/commit_key.h"
#include "structure/opening_key.h"
#include "structure/reference_string.h"

std::tuple<kzg::structure::CommitKey, kzg::structure::OpeningKey> setup_test(size_t degree) {
    auto srs = kzg::structure::ReferenceString::setup(degree);
    return srs.trim(degree);
}

TEST(TestCommitment, CommitSingle) {
    // 1. setup
    const size_t degree = 25;
    const auto [commit_key, opening_key] = setup_test(degree);

    // 2. commit
    const auto polynomial = kzg::polynomial::CoefficientForm::random(degree);
    const auto commitment = kzg::process::commit(commit_key, polynomial);

    // 3. evaluate
    const auto point = bls12_381::scalar::Scalar{10};
    const auto proof = kzg::process::create_witness_single(commit_key, polynomial, point);

    // 4. verify
    const bool verify = kzg::process::verify_single_polynomial(opening_key, commitment, proof);
    EXPECT_TRUE(verify);
}

TEST(TestCommitment, CommitMultiple) {
    // 1. setup
    const size_t degree = 27;
    const auto [commit_key, opening_key] = setup_test(degree);

    // 2. commit
    const auto poly_1 = kzg::polynomial::CoefficientForm::random(25);
    const auto poly_2 = kzg::polynomial::CoefficientForm::random(27);
    const auto poly_3 = kzg::polynomial::CoefficientForm::random(27);
    const auto comm_1 = kzg::process::commit(commit_key, poly_1);
    const auto comm_2 = kzg::process::commit(commit_key, poly_2);
    const auto comm_3 = kzg::process::commit(commit_key, poly_3);
    const std::vector<kzg::structure::Commitment> commitments = {comm_1, comm_2, comm_3};
    const std::vector<kzg::polynomial::CoefficientForm> polynomials = {poly_1, poly_2, poly_3};

    // 3. evaluate
    const auto point = bls12_381::scalar::Scalar{10};
    auto prover_transcript = kzg::challenge::TranscriptProtocol{"agg_flatten"};
    const auto aggregated_proof = kzg::process::create_witness_multiple_polynomials(
            commit_key, polynomials, point, prover_transcript
    );

    // 4. verify
    auto verifier_transcript = kzg::challenge::TranscriptProtocol{"agg_flatten"};
    const bool verify = kzg::process::verify_multiple_polynomials(
            opening_key, commitments, aggregated_proof, verifier_transcript
    );
    EXPECT_TRUE(verify);
}

TEST(TestCommitment, CommitTwoPoints) {
    // 1. setup
    const size_t degree = 25;
    const auto [commit_key, opening_key] = setup_test(degree);

    // 2. commit
    const auto poly_1 = kzg::polynomial::CoefficientForm::random(degree);
    const auto poly_2 = kzg::polynomial::CoefficientForm::random(degree);
    const auto comm_1 = kzg::process::commit(commit_key, poly_1);
    const auto comm_2 = kzg::process::commit(commit_key, poly_2);
    const std::vector<kzg::structure::Commitment> commitments = {comm_1, comm_2};
    const std::vector<kzg::polynomial::CoefficientForm> polynomials = {poly_1, poly_2};

    // 3. evaluate
    const auto point_1 = bls12_381::scalar::Scalar{10};
    const auto point_2 = bls12_381::scalar::Scalar{11};
    const auto proof_1 = kzg::process::create_witness_single(commit_key, poly_1, point_1);
    const auto proof_2 = kzg::process::create_witness_single(commit_key, poly_2, point_2);

    // 4. separate verify
    const bool verify_1 = kzg::process::verify_single_polynomial(opening_key, comm_1, proof_1);
    const bool verify_2 = kzg::process::verify_single_polynomial(opening_key, comm_2, proof_2);
    EXPECT_TRUE(verify_1);
    EXPECT_TRUE(verify_2);

    // 5. batch verify
    const std::vector<bls12_381::scalar::Scalar> points = {point_1, point_2};
    const std::vector<bls12_381::scalar::Scalar> evaluations = {proof_1.evaluation, proof_2.evaluation};
    const std::vector<kzg::structure::Commitment> witnesses = {proof_1.witness, proof_2.witness};
    const auto batch_proof = kzg::structure::BatchProof{points, evaluations, witnesses};

    auto transcript = kzg::challenge::TranscriptProtocol{"??"};
    const bool verify_batch = kzg::process::verify_multiple_points(opening_key, commitments, batch_proof, transcript);
    EXPECT_TRUE(verify_batch);
}

TEST(TestCommitment, CommitMultiplePointsWithAggregation) {
    // 1. setup
    const size_t degree = 28;
    const auto [commit_key, opening_key] = setup_test(degree);

    // 2. commit
    const auto poly_1 = kzg::polynomial::CoefficientForm::random(degree);
    const auto poly_2 = kzg::polynomial::CoefficientForm::random(degree);
    const auto poly_3 = kzg::polynomial::CoefficientForm::random(degree);
    const auto poly_4 = kzg::polynomial::CoefficientForm::random(degree);
    const auto comm_1 = kzg::process::commit(commit_key, poly_1);
    const auto comm_2 = kzg::process::commit(commit_key, poly_2);
    const auto comm_3 = kzg::process::commit(commit_key, poly_3);
    const auto comm_4 = kzg::process::commit(commit_key, poly_4);
    const std::vector<kzg::structure::Commitment> commitment_vec = {comm_1, comm_2, comm_3};

    // 3. evaluate
    const auto point_1 = bls12_381::scalar::Scalar{10};
    const auto point_2 = bls12_381::scalar::Scalar{11};
    auto prover_transcript = kzg::challenge::TranscriptProtocol{"agg_batch"};
    const std::vector<kzg::polynomial::CoefficientForm> polynomials = {poly_1, poly_2, poly_3};
    const auto aggregated_proof = kzg::process::create_witness_multiple_polynomials(
            commit_key, polynomials, point_1, prover_transcript
    );
    const auto proof = kzg::process::create_witness_single(commit_key, poly_4, point_2);

    // 4. verify
    auto verifier_transcript = kzg::challenge::TranscriptProtocol{"agg_batch"};
    const auto [commitment_agg, evaluation_agg] =
            kzg::process::verify_aggregation(commitment_vec, aggregated_proof.evaluations, verifier_transcript);
    const std::vector<kzg::structure::Commitment> commitments = {commitment_agg, comm_4};
    const std::vector<bls12_381::scalar::Scalar> points = {point_1, point_2};
    const std::vector<bls12_381::scalar::Scalar> evaluations = {evaluation_agg, proof.evaluation};
    const std::vector<kzg::structure::Commitment> witnesses = {aggregated_proof.witness, proof.witness};

    const auto batch_proof = kzg::structure::BatchProof{points, evaluations, witnesses};
    const bool verify = kzg::process::verify_multiple_points(opening_key, commitments, batch_proof, verifier_transcript);
    EXPECT_TRUE(verify);
}