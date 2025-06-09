#include "utils/app_state.hpp"
#include "utils/cv_helper.hpp"
#include "capture_loops.hpp"

#include <librealsense2/hpp/rs_context.hpp>
#include <librealsense2/hpp/rs_frame.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <librealsense2/rs.hpp>
#include <opencv4/opencv2/core/types.hpp>
#include <opencv4/opencv2/imgproc.hpp>
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

void ir_enhance_loop(rs2::pipeline &pipe) {
  while (curr_state.selected_mode.load() == SelectedMode::Infrared_Enhanced) {
    rs2::frameset frames = pipe.wait_for_frames();
    rs2::frame ir_frame = frames.get_infrared_frame();
    cv::Mat ir_mat = frame_to_mat(ir_frame); // grayscale

    // Threshold to get binary mask
    cv::Mat binary;
    double threshold_value = 125;
    cv::threshold(ir_mat, binary, threshold_value, 255, cv::THRESH_BINARY);

    // Convert IR grayscale image to BGR so we can overlay color
    cv::Mat ir_bgr;
    cv::cvtColor(ir_mat, ir_bgr, cv::COLOR_GRAY2BGR);

    // Set red pixels where binary is white (i.e., above threshold)
    // (B, G, R) = (0, 0, 255)
    ir_bgr.setTo(cv::Scalar(0, 0, 255), binary);

    // Add overlay text
    cv::putText(ir_bgr, "Selected Mode: Enhanced IR", cv::Point(10, 30),
                cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(255, 255, 255), 2);

    // Show result
    cv::imshow("Calibration Tool", ir_bgr);
  }
  return;
}


//void ir_and_color_loop(const rs2::pipeline& pipe, const cv::Mat& map_x, const cv::Mat& map_y) {
//  while(curr_state.selected_mode.load() == SelectedMode::Both) {
//    rs2::frameset frames = pipe.wait_for_frames();
//    rs2::frame ir_frame = frames.get_infrared_frame();
//    rs2::frame color_frame = frames.get_color_frame();
//    cv::Mat ir_mat = frame_to_mat(ir_frame);
//    cv::Mat color_mat = frame_to_mat(color_frame);
//    cv::Mat binary;
//    double threshold_value = 100;
//    cv::threshold(ir_mat, binary, threshold_value, 255, cv::THRESH_BINARY);
//
//// Remap IR binary mask to align with color image
//    cv::Mat remapped_ir_mask;
//    cv::remap(binary, remapped_ir_mask, map_x, map_y, cv::INTER_NEAREST, cv::BORDER_CONSTANT, 0);
//
//// Overlay red on color image where remapped IR is active
//    cv::Mat red_overlay = cv::Mat::zeros(color_mat.size(), CV_8UC3);
//    red_overlay.setTo(cv::Scalar(0, 0, 255), remapped_ir_mask);
//    red_overlay.copyTo(color_mat, remapped_ir_mask);  // BGR: (Blue=0, Green=0, Red=255)
//
//    // Blend red overlay with color frame
//    cv::putText(color_mat, "Selected Mode: Both", cv::Point(10, 30), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(255, 0, 0), 2);
//    cv::imshow("Calibration Tool", color_mat);
//  }
//}
