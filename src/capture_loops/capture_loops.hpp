#ifndef CAPTURE_LOOPS_HPP
#define CAPTURE_LOOPS_HPP
#include <librealsense2/hpp/rs_context.hpp>
#include <librealsense2/rs.hpp>

void color_loop(rs2::pipeline& pipe);
void ir_loop(rs2::pipeline& pipe);
void ir_and_color_loop(rs2::pipeline& pipe);

#endif
