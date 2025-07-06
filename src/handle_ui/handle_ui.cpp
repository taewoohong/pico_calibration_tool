#include "utils/app_state.hpp"
#include "handle_ui.hpp"

#include <chrono>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <thread>
#include <iostream>

void HandleUserInterface() {
  cv::namedWindow("Calibration Tool", cv::WINDOW_NORMAL);
  cv::createTrackbar("threshold", "Calibration Tool", reinterpret_cast<int*>(&curr_state.ir_threshold), 255);
  //cv::createTrackbar("exposure", "Calibration Tool", reinterpret_cast<int*>(&curr_state.exposure), 60000.f);
  cv::createTrackbar("exposure", "Calibration Tool", reinterpret_cast<int*>(&curr_state.exposure), 60000);
  while(curr_state.selected_mode != SelectedMode::Kill) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    char input = cv::waitKey(1);
    switch (input) {
      case '1':
        curr_state.selected_mode.store(SelectedMode::Color);
        break;
      case '2':
        curr_state.selected_mode.store(SelectedMode::Infrared);
        break;
      case '3':
        curr_state.selected_mode.store(SelectedMode::Infrared_Enhanced);
        break;
      case 'q':
        curr_state.selected_mode.store(SelectedMode::Kill);
        break;
      case 'r':
        curr_state.selected_mode.store(SelectedMode::Color);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        curr_state.selected_mode.store(SelectedMode::Infrared_Enhanced);
        break;
      default:
        break;
    }
    // if(input == '1') {
    //   curr_state.selected_mode.store(SelectedMode::Color);
    // }
    // else if (input == '2') {
    //   curr_state.selected_mode.store(SelectedMode::Infrared);
    // }
    // else if:wq (input == '3') {
    //   curr_state.selected_mode.store(SelectedMode::Both);
    // }
    // else if (input == 'q') {
    //   curr_state.selected_mode.store(SelectedMode::Kill);
    // }
  }
}
