#ifndef APP_STATE_HPP
#define APP_STATE_HPP

#include <atomic>

enum class SelectedMode {
  Infrared,
  Color,
  Both,
  Kill,
};

struct AppMode {
  std::atomic<SelectedMode> selected_mode {SelectedMode::Color};
};

extern AppMode curr_state;
#endif
