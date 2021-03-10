// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "gtest/gtest.h"
#include "test/common/cuda_op_test_utils.h"
#include "test/providers/provider_test_utils.h"
#include "test/common/tensor_op_test_utils.h"

namespace onnxruntime {
namespace test {

static void SequencePoolingTest(
  const std::vector<float>& batch_input,
  const std::vector<int64_t>& batch_sequence_lengths,
  const std::vector<float>& output,
  const int batch_size,
  const int sequence_length_for_split,
  const int hidden_size,
  const int num_sequence) {

  OpTester tester("SequencePooling", 1, onnxruntime::kMSDomain);

  const int num_sequence_max = 4;
  //tester.AddInput<float>("batch_input_tensor", {batch_size, sequence_length_for_split, hidden_size}, batch_input);
  //tester.AddInput<int64_t>("batch_sentence_lengthes", {batch_size, num_sequence}, batch_sequence_lengths);
  //tester.AddOutput<float>("output", {batch_size, num_sequence_max, hidden_size}, output);
  //tester.AddOutput<float>("masks", {batch_size, num_sequence_max}, masks);
//
  //std::vector<std::unique_ptr<IExecutionProvider>> execution_providers;
  //execution_providers.push_back(DefaultCpuExecutionProvider());
  //tester.Run(OpTester::ExpectResult::kExpectSuccess, "", {}, nullptr, &execution_providers);

  if (HasCudaEnvironment(530 /*min_cuda_architecture*/)) {
    OpTester tester_1("SequencePooling", 1, onnxruntime::kMSDomain);

    tester_1.AddInput<MLFloat16>("batch_input_tensor", {batch_size, sequence_length_for_split, hidden_size}, ToFloat16(batch_input));
    tester_1.AddInput<int64_t>("batch_sentence_lengthes", {batch_size, num_sequence}, batch_sequence_lengths);
    tester_1.AddOutput<MLFloat16>("output", {batch_size, num_sequence_max, hidden_size}, ToFloat16(output));

    std::vector<std::unique_ptr<IExecutionProvider>> execution_providers_1;
    execution_providers_1.push_back(DefaultCudaExecutionProvider());
    tester_1.Run(OpTester::ExpectResult::kExpectSuccess, "", {}, nullptr, &execution_providers_1);
  }
}


TEST(SequencePoolingTest, Test_1) {
  std::vector<float> batch_input{1.0f, 3.0f, -3.0f, 3.0f, -5.0f,
                                 5.0f, -4.0f, -3.0f, 6.0f, 3.0f,
                                 -1.0f, -2.0f, 3.0f, -4.0f, 5.0f,
                                 -6.0f, 7.0f, 8.0f, -9.0f, 1.0f,
                                 1.0f, 5.0f, 3.0f, 8.0f, -2.0f,
                                 6.0f, 1.0f, 3.0f, 2.0f, 4.0f};
  std::vector<int64_t> batch_sequence_lengths{1, 1, 4};
  std::vector<float> output{1.0f, 3.0f, -3.0f, 3.0f, -5.0f,
                            5.0f, -4.0f, -3.0f, 6.0f, 3.0f,
                            6.0f, 7.0f, 8.0f, 8.0f, 5.0f,
                            0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

  SequencePoolingTest(batch_input, batch_sequence_lengths, output, 1, 6, 5, 3);
}

}  // namespace test
}  // namespace onnxruntime