#include <iostream>
#include <librealsense2/h/rs_sensor.h>
#include <librealsense2/h/rs_types.h>
#include <librealsense2/hpp/rs_context.hpp>
#include <librealsense2/hpp/rs_frame.hpp>
#include <librealsense2/rs.hpp>
#include <opencv2/core.hpp>

int main(int argc, char* argv[]){
  rs2::pipeline pipe;
  pipe.start();
  rs2::frameset frames = pipe.wait_for_frames();

  rs2::video_frame color_frame = frames.get_color_frame();
  rs2_intrinsics intrinsics = color_frame.get_profile().as<rs2::video_stream_profile>().get_intrinsics();
  //rs2_extrinsics extrinsics = color_frame.get_profile().as<rs2::video_stream_profile>().get_extrinsics_to(const stream_profile &to)

  std::cout << "Width: " << intrinsics.width << std::endl;
  std::cout << "Height: " << intrinsics.height << std::endl;
  std::cout << "Focal length(fx, fy): " << intrinsics.fx << ", " << intrinsics.fy << std::endl;
  std::cout << "Principal Point (cx, cy): " << intrinsics.ppx << ", " << intrinsics.ppy << std::endl;
  std::cout << "Distortion Model: " << intrinsics.model << std::endl;
  return 0;
}
