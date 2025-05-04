#include <iostream>
#include <librealsense2/h/rs_sensor.h>
#include <librealsense2/hpp/rs_context.hpp>
#include <librealsense2/hpp/rs_frame.hpp>
#include <opencv2/core.hpp>
#include <librealsense2/rs.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include "utils/cv_helper.hpp"

int main(int argc, char* argv[]) {
  rs2::pipeline pipe;
  rs2::config cfg;

  cfg.enable_stream(RS2_STREAM_COLOR, -1);
  cfg.enable_stream(RS2_STREAM_INFRARED, 1);

  pipe.start(cfg);
  while (cv::waitKey(1) != 'q') {
    rs2::frameset frames = pipe.wait_for_frames();
    rs2::frame color_frame = frames.get_color_frame();
    rs2::frame ir_frame = frames.get_infrared_frame();
    cv::Mat cv_frame = frame_to_mat(color_frame);
    cv::imshow("testing", cv_frame);
  }
  cv::destroyAllWindows();
  return 0;
}
