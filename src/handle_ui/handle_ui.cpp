#include "utils/app_state.hpp"
#include "handle_ui.hpp"

#include <chrono>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <thread>


void HandleUserInterface() {
  cv::namedWindow("Calibration Tool", cv::WINDOW_NORMAL);
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
      default:
        break;
    }
    // if(input == '1') {
    //   curr_state.selected_mode.store(SelectedMode::Color);
    // }
    // else if (input == '2') {
    //   curr_state.selected_mode.store(SelectedMode::Infrared);
    // }
    // else if (input == '3') {
    //   curr_state.selected_mode.store(SelectedMode::Both);
    // }
    // else if (input == 'q') {
    //   curr_state.selected_mode.store(SelectedMode::Kill);
    // }
  }
}
