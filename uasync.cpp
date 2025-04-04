#ifndef __UASYNC_CPP__
#define __UASYNC_CPP__

#include "uasync.hpp"

/* #region UAsync */

namespace UAsync {

/* #region QueueNode */

  template <typename T>
  QueueNode<T>::~QueueNode() {
    if (this->data) delete this->data;
    if (this->next) delete this->next;
  }

/* #endregion */

/* #region LoopAction */

  LoopAction::LoopAction(void (*cb)(), unsigned long ms) {
    this->cb = cb;
    this->ms = ms;
    this->lastMs = millis();
  }

  void IntervalAction::run() {
    auto now = millis();
    if (this->ms > now - this->lastMs) return;
    this->lastMs = now;

    this->cb();
  }

  void TimeoutAction::run() {
    auto now = millis();
    if (this->ms > now - this->lastMs) return;
    this->lastMs = now;

    this->cb();

    auto queue = LoopQueue::getInstance();
    queue->remove(id);
  }

/* #endregion */

/* #region LoopQueue */

  LoopQueue* LoopQueue::getInstance() {
    static LoopQueue* instance = 0;
    if (instance == 0) instance = new LoopQueue();
    return instance;
  }

  LoopQueue::LoopQueue() {
    this->counter = 1;
    this->head = new QueueNode<LoopAction>();
    this->tail = this->head;
  }

  int LoopQueue::add(LoopAction* action) {
    action->id = this->counter++;
    auto node = new QueueNode<LoopAction>();
    node->data = action;

    this->tail->next = node;
    this->tail = node;

    return action->id;
  }

  void LoopQueue::remove(int id) {
    auto node = this->head;
    while (node->next != 0 && node->next->data->id != id) {
      node = node->next;
    }

    auto toDelete = node->next;

    if (toDelete == this->tail) {
      this->tail = node;
      this->tail->next = 0;
    }

    node->next = toDelete->next;
    toDelete->next = 0;
    delete toDelete;
  }

  void LoopQueue::runAll() {
    int i = 0;
    auto node = this->head;
    while (node && node->next != 0) {
      node->next->data->run();
      node = node->next;
    }
  }

/* #endregion */

}

/* #endregion */

/* #region Exported Functions */

int setInterval(void (*cb)(), unsigned long ms) {
  auto queue = UAsync::LoopQueue::getInstance();
  auto action = new UAsync::IntervalAction(cb, ms);
  return queue->add(action);
}

void clearInterval(int id) {
  auto queue = UAsync::LoopQueue::getInstance();
  queue->remove(id);
}

int setTimeout(void (*cb)(), unsigned long ms) {
  auto queue = UAsync::LoopQueue::getInstance();
  auto action = new UAsync::TimeoutAction(cb, ms);
  return queue->add(action);
}

void clearTimeout(int id) {
  auto queue = UAsync::LoopQueue::getInstance();
  queue->remove(id);
}

/* #endregion */

#endif
