#include "utils/app_state.hpp"
#include "utils/cv_helper.hpp"
#include "capture_loops.hpp"

#include <librealsense2/hpp/rs_context.hpp>
#include <librealsense2/hpp/rs_frame.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <librealsense2/rs.hpp>
#include <opencv4/opencv2/core/types.hpp>
#include <opencv4/opencv2/opencv.hpp>

void color_loop(rs2::pipeline& pipe) {
  while(curr_state.selected_mode.load() == SelectedMode::Color) {
    rs2::frameset frames = pipe.wait_for_frames();
    rs2::frame color_frame = frames.get_color_frame();
    cv::Mat color_mat = frame_to_mat(color_frame);
    cv::putText(color_mat, "Selected Mode: Color", cv::Point(10, 30), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(255, 0, 0), 2);
    cv::imshow("Calibration Tool", color_mat);
  }
  return;
}

void ir_loop(rs2::pipeline& pipe) {
  while(curr_state.selected_mode.load() == SelectedMode::Infrared) {
    rs2::frameset frames = pipe.wait_for_frames();
    rs2::frame ir_frame = frames.get_infrared_frame();
    cv::Mat ir_mat = frame_to_mat(ir_frame);
    cv::putText(ir_mat, "Selected Mode: Infrared", cv::Point(10, 30), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(255, 0, 0), 2);
    cv::imshow("Calibration Tool", ir_mat);
  }
  return;
}

void ir_and_color_loop(rs2::pipeline& pipe) {
  while(curr_state.selected_mode.load() == SelectedMode::Both) {
    rs2::frameset frames = pipe.wait_for_frames();
    rs2::frame ir_frame = frames.get_infrared_frame();
    rs2::frame color_frame = frames.get_color_frame();
    cv::Mat ir_mat = frame_to_mat(ir_frame);
    cv::Mat color_mat = frame_to_mat(color_frame);
    cv::Mat binary;
    double threshhold_value = 100;
    cv::threshold(ir_mat, binary, threshhold_value, 255, cv::THRESH_BINARY);
    
    cv::Mat binary_bgr;
    cv::cvtColor(binary, binary_bgr, cv::COLOR_GRAY2BGR);

    cv::Mat red_overlay = cv::Mat::zeros(binary_bgr.size(), CV_8UC3);
    red_overlay.setTo(cv::Scalar(0, 0, 255), binary);  // BGR: (Blue=0, Green=0, Red=255)

    // Blend red overlay with color frame
    cv::Mat blended;
    cv::addWeighted(color_mat, 1.0, red_overlay, 1.0, 0, blended);
    cv::putText(blended, "Selected Mode: Both", cv::Point(10, 30), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(255, 0, 0), 2);
    cv::imshow("Calibration Tool", blended);
  }
}
