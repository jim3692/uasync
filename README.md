# Micro Async Library for Arduino

## Demo Code

```c++
#include "uasync.hpp"

int pinStatus = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  setInterval([]() {
    digitalWrite(LED_BUILTIN, pinStatus);
    pinStatus = !pinStatus;
  }, 100);
}

void loop() {
  UAsync::LoopQueue::getInstance()->runAll();
}
```
