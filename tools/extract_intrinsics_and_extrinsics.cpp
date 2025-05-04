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

void print_matrices(rs2_extrinsics data) {
  std::cout << "============================================" << std::endl;
  std::cout << "Extrinsics from color to IR: " << std::endl;
  std::cout << "============================================" << std::endl;
  std::cout << "Rotational matrix -" << std::endl;
  for (int i = 0; i < 3; i++) {
    std::cout << data.rotation[i] << " ";
    if ((i + 1) % 3 == 0) {
      std::cout << std::endl;
    }
  }
  
  std::cout << std::endl;
  std::cout << "Translational matrix -" << std::endl;
  for (int i = 0; i < 3; i++) {
    std::cout << data.translation[i] << " ";
    if ((i + 1) % 3 == 0) {
       std::cout << std::endl;
    }
  }
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
  
  rs2::video_stream_profile color_profile = color_frame.get_profile().as<rs2::video_stream_profile>();
  rs2::video_stream_profile ir_profile = ir_frame.get_profile().as<rs2::video_stream_profile>();
  
  rs2_extrinsics color_to_ir_extrin = color_profile.get_extrinsics_to(ir_profile);

  std::cout << "COLOR STREAM" << std::endl;
  print_intrinsic_data(color_profile.get_intrinsics()); 
  
  std::cout << "IR STREAM" << std::endl;
  print_intrinsic_data(ir_profile.get_intrinsics());

  print_matrices(color_to_ir_extrin);
  return 0;
}
