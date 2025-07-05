#ifndef APP_STATE_HPP
#define APP_STATE_HPP

#include <atomic>

enum class SelectedMode {
  Infrared,
  Infrared_Enhanced,
  Color,
  //Both,
  Kill,
};

struct AppMode {
  std::atomic<SelectedMode> selected_mode {SelectedMode::Color};
  std::atomic<int> ir_threshold = 125;
};

extern AppMode curr_state;
#endif
