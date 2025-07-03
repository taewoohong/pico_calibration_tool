#pragma once

#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <opencv2/opencv.hpp>   // Include OpenCV API
#include <exception>
//#include <eigen3/Eigen/Dense>
#include <opencv2/core/mat.hpp>
#include <utility>
// Convert rs2::frame to cv::Mat
//
cv::Mat frame_to_mat(const rs2::frame& f);

cv::Mat depth_frame_to_meters(const rs2::depth_frame);

std::pair<cv::Mat, cv::Mat> compute_remap_maps(int width, int height);
