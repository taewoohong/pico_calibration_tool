#include <librealsense2/h/rs_option.h>
#include <librealsense2/h/rs_sensor.h>
#include <librealsense2/hpp/rs_pipeline.hpp>
#include <librealsense2/hpp/rs_sensor.hpp>
#include <librealsense2/rs.hpp>
#include <opencv2/core.hpp>
#include <iostream>
#include <opencv2/highgui.hpp>

#include "utils/cv_helper.hpp"
#include "utils/app_state.hpp"
#include "src/capture_loops/capture_loops.hpp"

void intialize_loop() {
  std::cout << "initializing loop" << std::endl;
  rs2::pipeline pipe;
  rs2::config cfg;

  cfg.enable_stream(RS2_STREAM_COLOR, -1);
  cfg.enable_stream(RS2_STREAM_INFRARED, 1);

  rs2::pipeline_profile selection = pipe.start(cfg);

  //disable laser
  auto depth_sensor = selection.get_device().first<rs2::depth_sensor>();
  depth_sensor.set_option(RS2_OPTION_LASER_POWER, 0.f);

  //disable auto exposure
  auto sensor = selection.get_device().first<rs2::sensor>();
  sensor.set_option(RS2_OPTION_ENABLE_AUTO_EXPOSURE, 0.f);
  sensor.set_option(RS2_OPTION_EXPOSURE, 20000.f);
  cv::Mat map_x, map_y;
  std::tie(map_x, map_y) = compute_remap_maps(1280, 720); 

  while(curr_state.selected_mode.load() != SelectedMode::Kill) {
    switch (curr_state.selected_mode.load()) {
      case SelectedMode::Color:
        color_loop(pipe);
        break;
      case SelectedMode::Infrared:
        ir_loop(pipe);
        break;
      /* case SelectedMode::Both:
        ir_and_color_loop(pipe, map_x, map_y);
        break; */
      case SelectedMode::Infrared_Enhanced:
        ir_enhance_loop(pipe);
        break;
      default:
        std::cout << "In default loop" << std::endl;
        break;
    }
  }
  cv::destroyAllWindows();
  return;
}
