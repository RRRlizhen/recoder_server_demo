#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <deque>
#include <functional>

namespace MYWORK {
    class ThreadPool {
        public:
            typedef std::function<void()> Task;
            ThreadPool(int threadNum = 100);
            virtual ~ThreadPool();
            size_t addTask(const Task &task);
            void stop();
            Task take();

        private:
            int createThreads();
            static void* threadFunc(void *threadData);
            ThreadPool& operator=(const ThreadPool&);
            ThreadPool(const ThreadPool&);


        private:
            volatile bool isRunning;
            int threadNum;
            pthread_t* threads;

            std::deque<Task> taskQueue;
            pthread_mutex_t mutex;
            pthread_cond_t condition;                        
    };
}

#endif
