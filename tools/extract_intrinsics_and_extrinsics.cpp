#include <iostream>
#include <librealsense2/h/rs_sensor.h>
#include <librealsense2/h/rs_types.h>
#include <librealsense2/hpp/rs_context.hpp>
#include <librealsense2/hpp/rs_frame.hpp>
#include <librealsense2/rs.hpp>
#include <opencv2/core.hpp>


void print_intrinsic_data(rs2_intrinsics data) {
  std::cout << "============================================" << std::endl;
  std::cout << "Width: " << data.width << std::endl;
  std::cout << "Height: " << data.height << std::endl;
  std::cout << "Focal length(fx, fy): " << data.fx << ", " << data.fy << std::endl;
  std::cout << "Principal Point (cx, cy): " << data.ppx << ", " << data.ppy << std::endl;
  std::cout << "Distortion Model: " << data.model << std::endl;
  std::cout << "============================================" << std::endl << std::endl;
return;
}

int main(int argc, char* argv[]){
  rs2::pipeline pipe;
  rs2::config cfg;

  cfg.enable_stream(RS2_STREAM_COLOR, -1);
  cfg.enable_stream(RS2_STREAM_INFRARED, 1);
  pipe.start(cfg);
  rs2::frameset frames = pipe.wait_for_frames();

  rs2::video_frame color_frame = frames.get_color_frame();
  rs2::video_frame ir_frame = frames.get_infrared_frame();
  
  rs2_intrinsics color_intrinsics = color_frame.get_profile().as<rs2::video_stream_profile>().get_intrinsics();
  rs2_intrinsics ir_intrinsics = ir_frame.get_profile().as<rs2::video_stream_profile>().get_intrinsics();

  //rs2_extrinsics extrinsics = color_frame.get_profile().as<rs2::video_stream_profile>().get_extrinsics_to(const stream_profile &to)
  
  std::cout << "COLOR STREAM" << std::endl;
  print_intrinsic_data(color_intrinsics); 
  
  std::cout << "IR STREAM" << std::endl;
  print_intrinsic_data(ir_intrinsics);

  return 0;
}
