#ifndef __UASYNC_H__
#define __UASYNC_H__

#ifdef UASYNC_LOGGER
  #include <cstdio>
  #include <string>
  void uasync_logger(std::string t, std::string x);
  #define __UASYNC_DEBUG(x) uasync_logger("[DEBUG]", x)
#else
  #define __UASYNC_DEBUG(x)
#endif

unsigned long millis();

namespace UAsync {

  template <typename T>
  class Queue {};

  template <typename T>
  class QueueNode {
    public:
      T* data;
      QueueNode<T>* next;
      QueueNode(): data(0), next(0) {}
      ~QueueNode();
  };

  class LoopAction {
    protected:
      void (*cb)();
      unsigned long lastMs;
      unsigned long ms;
    public:
      int id;
      LoopAction(void (*cb)(), unsigned long ms);
      virtual void run() = 0;
  };

  class IntervalAction : public LoopAction {
    using LoopAction::LoopAction;
    public: void run() override;
  };

  class TimeoutAction : public LoopAction {
    using LoopAction::LoopAction;
    public: void run() override;
  };

  class LoopQueue : public Queue<LoopAction> {
    private:
      LoopQueue();
      QueueNode<LoopAction>* head;
      QueueNode<LoopAction>* tail;
      int counter;
    public:
      static LoopQueue* getInstance();
      int add(LoopAction* action);
      void remove(int id);
      void runAll();
  };

}

int setInterval(void (*cb)(), unsigned long ms);
void clearInterval(int id);

int setTimeout(void (*cb)(), unsigned long ms);
void clearTimeout(int id);

#endif
