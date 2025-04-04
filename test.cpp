#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

#include "uasync.hpp"

unsigned long millis() {
  return duration_cast<milliseconds>(
    system_clock::now().time_since_epoch()
  ).count();
}

// void uasync_logger(string t, string x) {
//   cout << t << ": " << x << endl;
// }

int main() {
  setTimeout([]() {
      cout << "setTimeout - after 1000ms" << endl;
  }, 1000);

  setInterval([]() {
      cout << "setInterval - after 100ms" << endl;
  }, 100);

  while(true) {
    UAsync::LoopQueue::getInstance()->runAll();
  }
}
