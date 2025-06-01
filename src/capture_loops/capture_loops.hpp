#ifndef CAPTURE_LOOPS_HPP
#define CAPTURE_LOOPS_HPP
#include <librealsense2/hpp/rs_context.hpp>
#include <librealsense2/rs.hpp>
#include <opencv2/core.hpp>

void color_loop(rs2::pipeline& pipe);
void ir_loop(rs2::pipeline& pipe);
void ir_and_color_loop(const rs2::pipeline& pipe, const cv::Mat& map_x, const cv::Mat& map_y);

#endif
