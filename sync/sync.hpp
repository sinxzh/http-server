#ifndef HTTP_SERVER_SYNC_SYNC_HPP
#define HTTP_SERVER_SYNC_SYNC_HPP

#include <pthread.h>
#include <semaphore.h>

#include <exception>

// -----------------------------------------------------------------------
// 线程同步模块：
//    使用RAII技术封装线程同步工具，包括互斥量、条件变量和信号量
// -----------------------------------------------------------------------

class Mutex {
 public:
  Mutex() {
    if (pthread_mutex_init(&mutex_, nullptr) != 0) {
      throw std::exception();
    }
  }

  ~Mutex() { pthread_mutex_destroy(&mutex_); }

  bool lock() { return pthread_mutex_lock(&mutex_) == 0; }

  bool unlock() { return pthread_mutex_unlock(&mutex_) == 0; }

  pthread_mutex_t *getMutex() { return &mutex_; }

 private:
  pthread_mutex_t mutex_;
};

class Cond {
 public:
  Cond() {
    if (pthread_cond_init(&cond_, nullptr) != 0) {
      throw std::exception();
    }
  }

  ~Cond() { pthread_cond_destroy(&cond_); }

  bool wait(pthread_mutex_t *mutex) {
    return pthread_cond_wait(&cond_, mutex) == 0;
  }

  bool signal() { return pthread_cond_signal(&cond_) == 0; }

 private:
  pthread_cond_t cond_;
};

class Sem {
 public:
  Sem() {
    if (sem_init(&sem_, 0, 0) != 0) {
      throw std::exception();
    }
  }

  Sem(int val) {
    if (sem_init(&sem_, 0, val) != 0) {
      throw std::exception();
    }
  }

  ~Sem() { sem_destroy(&sem_); }

  bool wait() { return sem_wait(&sem_) == 0; }

  bool post() { return sem_post(&sem_) == 0; }

 private:
  sem_t sem_;
};

#endif
