#include <gtest/gtest.h>

#include <private/dvr/eigen.h>
#include <private/dvr/pose.h>
#include <private/dvr/test/test_macros.h>

using PoseTypes = ::testing::Types<float, double>;

template <class T>
class PoseTest : public ::testing::TestWithParam<T> {
 public:
  using FT = T;
  using Pose_t = android::dvr::Pose<FT>;
  using quat_t = Eigen::Quaternion<FT>;
  using vec3_t = Eigen::Vector3<FT>;
  using mat4_t = Eigen::AffineMatrix<FT, 4>;
};

TYPED_TEST_CASE(PoseTest, PoseTypes);

// Check that the two matrix methods are inverses of each other
TYPED_TEST(PoseTest, SelfInverse) {
  using quat_t = typename TestFixture::quat_t;
  using vec3_t = typename TestFixture::vec3_t;
  using Pose_t = typename TestFixture::Pose_t;
  using mat4_t = typename TestFixture::mat4_t;
  using FT = typename TestFixture::FT;

  const auto tolerance = FT(0.0001);

  const quat_t initial_rotation(Eigen::AngleAxis<FT>(
      FT(M_PI / 3.0), vec3_t(FT(3.0), FT(4.0), FT(5.0)).normalized()));
  const vec3_t initial_position = vec3_t(FT(2.0), FT(10.0), FT(-4.0));
  const Pose_t initial_pose(initial_rotation, initial_position);

  auto result_pose = initial_pose.GetReferenceFromObjectMatrix() *
                     initial_pose.GetObjectFromReferenceMatrix();

  EXPECT_MAT4_NEAR(result_pose, mat4_t::Identity(), tolerance);
}

TYPED_TEST(PoseTest, TransformPoint) {
  using quat_t = typename TestFixture::quat_t;
  using vec3_t = typename TestFixture::vec3_t;
  using Pose_t = typename TestFixture::Pose_t;
  using FT = typename TestFixture::FT;

  const auto tolerance = FT(0.0001);

  const quat_t pose_rotation(
      Eigen::AngleAxis<FT>(FT(M_PI / 2.0), vec3_t(FT(0.0), FT(0.0), FT(1.0))));
  const auto pose_position = vec3_t(FT(1.0), FT(1.0), FT(2.0));

  const Pose_t test_pose(pose_rotation, pose_position);

  for (int axis = 0; axis < 3; ++axis) {
    vec3_t start_position = vec3_t::Zero();
    start_position[axis] = FT(1.0);
    const vec3_t expected_transformed =
        (pose_rotation * start_position) + pose_position;
    const vec3_t actual_transformed = test_pose.TransformPoint(start_position);
    EXPECT_VEC3_NEAR(expected_transformed, actual_transformed, tolerance);
  }
}

TYPED_TEST(PoseTest, TransformVector) {
  using quat_t = typename TestFixture::quat_t;
  using vec3_t = typename TestFixture::vec3_t;
  using Pose_t = typename TestFixture::Pose_t;
  using FT = typename TestFixture::FT;

  const auto tolerance = FT(0.0001);

  const quat_t pose_rotation(Eigen::AngleAxis<FT>(
      FT(M_PI / 6.0), vec3_t(FT(3.0), FT(4.0), FT(5.0)).normalized()));

  const auto pose_position = vec3_t(FT(500.0), FT(-500.0), FT(300.0));

  const Pose_t test_pose(pose_rotation, pose_position);

  for (int axis = 0; axis < 3; ++axis) {
    vec3_t start_position = vec3_t::Zero();
    start_position[axis] = FT(1.0);
    const vec3_t expected_rotated = pose_rotation * start_position;
    const vec3_t actual_rotated = test_pose.Transform(start_position);
    EXPECT_VEC3_NEAR(expected_rotated, actual_rotated, tolerance);
  }
}

TYPED_TEST(PoseTest, Composition) {
  using quat_t = typename TestFixture::quat_t;
  using Pose_t = typename TestFixture::Pose_t;
  using vec3_t = typename TestFixture::vec3_t;
  using FT = typename TestFixture::FT;

  const auto tolerance = FT(0.0001);

  const quat_t first_rotation(
      Eigen::AngleAxis<FT>(FT(M_PI / 2.0), vec3_t(FT(0.0), FT(0.0), FT(1.0))));
  const auto first_offset = vec3_t(FT(-3.0), FT(2.0), FT(-1.0));
  const quat_t second_rotation(Eigen::AngleAxis<FT>(
      FT(M_PI / 3.0), vec3_t(FT(1.0), FT(-1.0), FT(0.0)).normalized()));
  const auto second_offset = vec3_t(FT(6.0), FT(-7.0), FT(-8.0));

  const Pose_t first_pose(first_rotation, first_offset);
  const Pose_t second_pose(second_rotation, second_offset);

  const auto combined_pose(second_pose.Compose(first_pose));

  for (int axis = 0; axis < 3; ++axis) {
    vec3_t start_position = vec3_t::Zero();
    start_position[axis] = FT(1.0);
    const vec3_t expected_transformed =
        second_pose.TransformPoint(first_pose.TransformPoint(start_position));
    const vec3_t actual_transformed =
        combined_pose.TransformPoint(start_position);
    EXPECT_VEC3_NEAR(expected_transformed, actual_transformed, tolerance);
  }
}

TYPED_TEST(PoseTest, Inverse) {
  using quat_t = typename TestFixture::quat_t;
  using vec3_t = typename TestFixture::vec3_t;
  using Pose_t = typename TestFixture::Pose_t;
  using FT = typename TestFixture::FT;

  const auto tolerance = FT(0.0001);

  const quat_t pose_rotation(Eigen::AngleAxis<FT>(
      FT(M_PI / 2.0), vec3_t(FT(4.0), FT(-2.0), FT(-1.0)).normalized()));
  const auto pose_position = vec3_t(FT(-1.0), FT(2.0), FT(-4.0));

  Pose_t pose(pose_rotation, pose_position);
  const Pose_t pose_inverse = pose.Inverse();

  for (int axis = 0; axis < 3; ++axis) {
    vec3_t start_position = vec3_t::Zero();
    start_position[axis] = FT(1.0);
    const vec3_t transformed = pose.Transform(start_position);
    const vec3_t inverted = pose_inverse.Transform(transformed);
    EXPECT_VEC3_NEAR(start_position, inverted, tolerance);
  }

  Pose_t nullified_pose[2] = {
      pose.Compose(pose_inverse), pose_inverse.Compose(pose),
  };

  for (int i = 0; i < 2; ++i) {
    EXPECT_QUAT_NEAR(quat_t::Identity(), nullified_pose[i].GetRotation(),
                     tolerance);
    EXPECT_VEC3_NEAR(vec3_t::Zero(), nullified_pose[i].GetPosition(),
                     tolerance);
  }
}
