#include <unistd.h>

#include "./http_conn/http_conn.hpp"
#include "./thread_pool/thread_pool.hpp"

static constexpr int THREAD_CNT = 4;
static constexpr int MAX_TASK_CNT = 60000;

int main() {
  ThreadPool<HttpConn> *threadPool{
      new ThreadPool<HttpConn>(THREAD_CNT, MAX_TASK_CNT)};

  HttpConn *tasks{new HttpConn[MAX_TASK_CNT]};
  for (int i{0}; i < 1000; ++i) {
    threadPool->addTask(tasks + i);
  }

  sleep(3);
}
