#include "utils/cv_helper.hpp"
#include <librealsense2/hpp/rs_frame.hpp>

cv::Mat frame_to_mat(const rs2::frame& f)
{
    using namespace cv;
    using namespace rs2;

    auto vf = f.as<video_frame>();
    const int w = vf.get_width();
    const int h = vf.get_height();

    if (f.get_profile().format() == RS2_FORMAT_BGR8)
    {
        return Mat(Size(w, h), CV_8UC3, (void*)f.get_data(), Mat::AUTO_STEP);
    }
    else if (f.get_profile().format() == RS2_FORMAT_RGB8)
    {
        auto r_rgb = Mat(Size(w, h), CV_8UC3, (void*)f.get_data(), Mat::AUTO_STEP);
        Mat r_bgr;
        cvtColor(r_rgb, r_bgr, COLOR_RGB2BGR);
        return r_bgr;
    }
    else if (f.get_profile().format() == RS2_FORMAT_Z16)
    {
        return Mat(Size(w, h), CV_16UC1, (void*)f.get_data(), Mat::AUTO_STEP);
    }
    else if (f.get_profile().format() == RS2_FORMAT_Y8)
    {
        return Mat(Size(w, h), CV_8UC1, (void*)f.get_data(), Mat::AUTO_STEP);
    }
    else if (f.get_profile().format() == RS2_FORMAT_DISPARITY32)
    {
        return Mat(Size(w, h), CV_32FC1, (void*)f.get_data(), Mat::AUTO_STEP);
    }

    throw std::runtime_error("Frame format is not supported yet!");
}

// Converts depth frame to a matrix of doubles with distances in meters
cv::Mat depth_frame_to_meters( const rs2::depth_frame & f )
{
    cv::Mat dm = frame_to_mat(f);
    dm.convertTo( dm, CV_64F );
    dm = dm * f.get_units();
    return dm;
}

std::pair<cv::Mat, cv::Mat> compute_remap_maps(int width, int height) {
    // Intrinsics
    const double fx_ir = 899.606, fy_ir = 899.606;
    const double cx_ir = 634.354, cy_ir = 359.367;
    const double fx_rgb = 917.86, fy_rgb = 915.845;
    const double cx_rgb = 633.542, cy_rgb = 363.679;

    // Extrinsics color → IR, invert to IR → color
    cv::Mat R_ci = (cv::Mat_<double>(3, 3) <<
        0.999997, -0.0016934, -0.00174546,
        0.0016934,  0.999998, -0.000111,
        0.0017454,  0.000114,  0.999998);
    cv::Mat T_ci = (cv::Mat_<double>(3, 1) << -0.0150841, -0.000154767, 0.000190537);
    cv::Mat R_ic = R_ci.t();
    cv::Mat T_ic = -R_ic * T_ci;

    cv::Mat map_x(height, width, CV_32FC1);
    cv::Mat map_y(height, width, CV_32FC1);

    double Z = 1.0;  // 1 meter depth

    for (int v = 0; v < height; ++v) {
        for (int u = 0; u < width; ++u) {
            double x_ir = (u - cx_ir) * Z / fx_ir;
            double y_ir = (v - cy_ir) * Z / fy_ir;

            cv::Mat pt_ir = (cv::Mat_<double>(3,1) << x_ir, y_ir, Z);
            cv::Mat pt_rgb = R_ic * pt_ir + T_ic;

            double x = pt_rgb.at<double>(0);
            double y = pt_rgb.at<double>(1);
            double z = pt_rgb.at<double>(2);

            float u_rgb = static_cast<float>((x * fx_rgb / z) + cx_rgb);
            float v_rgb = static_cast<float>((y * fy_rgb / z) + cy_rgb);

            map_x.at<float>(v, u) = u_rgb;
            map_y.at<float>(v, u) = v_rgb;
        }
    }

    return std::make_pair(map_x, map_y);
}
