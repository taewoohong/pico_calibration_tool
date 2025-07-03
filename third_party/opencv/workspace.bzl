load("@bazel//tools/build_defs/repo:local.blz", "local_repository")

local_repository(
  name = "opencv",
  path = "/opt/picoscan_tool/sysroot/opencv/include/opencv4",
  build_file = "//third_party/opencv"
)
