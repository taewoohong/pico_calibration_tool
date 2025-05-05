#include <iostream>
#include <thread>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>

#include "src/loop_control/loop_control.hpp"
#include "utils/app_state.hpp"
#include "src/handle_ui/handle_ui.hpp"

AppMode curr_state;

void set_thread_affinity(std::thread& thread, int core_id){
  cpu_set_t cpu_set;
  CPU_ZERO(&cpu_set);
  CPU_SET(core_id, &cpu_set);
  int res = pthread_setaffinity_np(thread.native_handle(), sizeof(cpu_set_t), &cpu_set);
  if(res != 0) {
    std::cerr << "failed to set thread affinity with error code: " << res << std::endl;
  }
  return;
}

void set_thread_priority(std::thread& thread, int policy, int priority) {
  pthread_t nativeHandle = thread.native_handle();
  struct sched_param param;
  param.sched_priority = priority;
  int ret = pthread_setschedparam(nativeHandle, policy, &param);

  if (ret != 0) {
    std::cerr << "Failed to set thread priority: " << ret << std::endl;
  }
}

void printAvailableCores() {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    pid_t pid = getpid();

    if (sched_getaffinity(pid, sizeof(cpu_set_t), &cpuset) == -1) {
        perror("sched_getaffinity");
        return;
    }

    std::cout << "Available CPU cores: ";
    for (int i = 0; i < CPU_SETSIZE; i++) {
        if (CPU_ISSET(i, &cpuset)) {
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
}

void* threadFunction(void* arg) {
    printAvailableCores();
    return nullptr;
}

int main() {
  cpu_set_t cpu_set;
  CPU_ZERO(&cpu_set);
  CPU_SET(4, &cpu_set);
  if (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu_set) != 0) {
    std::cerr << "Failed to set main thread affinity" << std::endl;
  }

  std::cout << "Starting CV thread..." << std::endl;
  std::thread backend_thread(intialize_loop);
  set_thread_affinity(backend_thread, 1);
  std::cout << "CV thread started on core 1." << std::endl;
  std::thread ui_thread(HandleUserInterface);
  set_thread_affinity(ui_thread, 0);
  backend_thread.join();
  ui_thread.join();

  // pthread_t thread;
  // int result = pthread_create(&thread, nullptr, threadFunction, nullptr);

  // pthread_join(thread, nullptr);
  return 0;
}
