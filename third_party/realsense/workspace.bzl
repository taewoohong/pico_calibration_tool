load("@bazel//tools/build/build_defs/repo:local.bzl", "local_repository")

local_repository(
  name = "realsense",
  path = "/usr/include/librealsense2",
  build_file = "//third_party/realsense",
)
