#ifndef HTTP_SERVER_THREAD_POOL_THREAD_POOL_HPP
#define HTTP_SERVER_THREAD_POOL_THREAD_POOL_HPP

#include <pthread.h>

#include <iostream>
#include <queue>
#include <vector>

#include "../sync/sync.hpp"

// -----------------------------------------------------------------------
// 线程池模块：
//    创建工作线程和工作队列，封装工作线程的执行函数，工作队列的入队和出队函数
// -----------------------------------------------------------------------

template <class T>
class ThreadPool {
 public:
  ThreadPool(int threadCnt, int maxTaskCnt);
  ~ThreadPool() = default;

  bool addTask(T *task);

 private:
  static void *work(void *arg);
  void run();

 private:
  int threadCnt_;
  std::vector<pthread_t> threadIds_;

  int maxTaskCnt_;
  std::queue<T *> taskQue_;

  Mutex mutex_;
  Sem sem_;
};

template <class T>
ThreadPool<T>::ThreadPool(int threadCnt, int maxTaskCnt)
    : threadCnt_{threadCnt}, maxTaskCnt_{maxTaskCnt} {
  if (threadCnt <= 0 || maxTaskCnt <= 0) {
    throw std::exception();
  }

  threadIds_.resize(threadCnt);

  for (auto &threadId : threadIds_) {
    if (pthread_create(&threadId, nullptr, work, this)) {
      throw std::exception();
    }
  }

  for (auto &threadId : threadIds_) {
    if (pthread_detach(threadId)) {
      throw std::exception();
    }
  }
}

template <class T>
bool ThreadPool<T>::addTask(T *task) {
  if (static_cast<int>(taskQue_.size()) >= maxTaskCnt_) {
    return false;
  }

  taskQue_.emplace(task);
  sem_.post();
  return true;
}

template <class T>
void *ThreadPool<T>::work(void *arg) {
  ThreadPool<T> *threadPool{reinterpret_cast<ThreadPool<T> *>(arg)};
  threadPool->run();
  return nullptr;
}

template <class T>
void ThreadPool<T>::run() {
  std::cout << "Thread created success: " << pthread_self() << std::endl;
  while (true) {
    sem_.wait();

    mutex_.lock();
    T *task{taskQue_.front()};
    taskQue_.pop();
    mutex_.unlock();

    task->process();
    std::cout << pthread_self() << std::endl;
  }
}

#endif
